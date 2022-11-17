#include "can_write.h"

ssize_t can_write_simple(int can_socket, int service, char * buffer) {
	struct can_frame write_frame;
	write_frame.can_id = service;
	write_frame.can_dlc = 0x8;
	memset(write_frame.data, 0x0, 0x8);
	memcpy(write_frame.data, buffer, strlen(buffer));
	ssize_t nbytes = write(can_socket, &write_frame, sizeof(struct can_frame));
	return nbytes;
}
