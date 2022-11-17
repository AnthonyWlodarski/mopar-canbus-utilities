#include "can_read.h"
#include "can_write.h"

char * can_read(int can_socket) {
	char * buffer = NULL;
	struct can_frame read_frame;
	ssize_t nbytes = read(can_socket, &read_frame, sizeof(struct can_frame));
	
	if (-1 == nbytes) {
		// errno should be set
		int curr_error = errno;
		perror("read");
		exit(curr_error);
	}

	if (nbytes < sizeof(struct can_frame)) {
		fprintf(stderr, "read: incomplete CAN read_frame\n");
	} else {
		// we have a few generic situations to deal with here
		// single frame
		if (read_frame.data[0] >> 4 == SINGLE_FRAME) {
			// some single frames are just a simpler protocol
			// with nothing behind it, it is up to the caller to inspect the
			// data coming back
			buffer = malloc(read_frame.data[0]);
			if (NULL == buffer) {
				int curr_error = errno;
				perror("malloc");
				exit(curr_error);
			}
			
			memcpy(buffer, read_frame.data+1, read_frame.data[0]);			
		} else if (read_frame.data[0] >> 4 == FIRST_FRAME) {
			// in the first frame we will have two bytes that make up the first frame data
			int buffer_size = (read_frame.data[0] & 0b1111) << 8;
			int bytes_copied = 0;
			buffer_size += read_frame.data[1];
			buffer = malloc(buffer_size);
			if (NULL == buffer) {
				int curr_error = errno;
				perror("malloc");
				exit(curr_error);
			}
			
			// to start copy the six bytes sent as part of the first frame
			memcpy(buffer+bytes_copied, read_frame.data+2, 6);
			bytes_copied += 6;

			// now we must send a flow control frame so we receive the continuation frames
			char command[3] = { FLOW_CONTROL_FRAME << 4, 0x0, 0x0 };
			can_write_simple(can_socket, read_frame.can_id - 0x8, command);
			do {
				char * temp_buffer = NULL;
				int bytes_to_copy = ((buffer_size - bytes_copied) >= 7) ?  7 : buffer_size - bytes_copied; 	
				temp_buffer = can_read(can_socket);
				memcpy(buffer+bytes_copied, temp_buffer, bytes_to_copy);
				bytes_copied += bytes_to_copy;
				free(temp_buffer);
				
			} while (bytes_copied < buffer_size); 
		
		} else if (read_frame.data[0] >> 4 == CONSECUTIVE_FRAME) {
			// have a consecutive frame, copy all bytes back and return
			int buffer_size = 0x7;
			buffer = malloc(buffer_size);
			if (NULL == buffer) {
				int curr_error = errno;
				perror("malloc");
				exit(curr_error);
			}
			
			// to start copy the six bytes sent as part of the first frame
			memcpy(buffer, read_frame.data+1, 0x7);
		}	
	}

	return buffer;
}
