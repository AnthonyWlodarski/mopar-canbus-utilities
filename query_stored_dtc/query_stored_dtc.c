#include <stdio.h>

#include "../lib/library.h"
#include "../lib/can_read.h"
#include "../lib/can_write.h"

int STORED_DTC_SERVICE = 0x3;
int PENDING_DTC_SERVICE = 0x7;
int PERMANENT_DTC_SERVICE = 0xA;

#define DST_SERVICE 0x7df
#define RES_SERVICE 0x7e8

void get_dtcs(int);
void get_dtcs_from_service(int, int);
void process_dtc_buffer(char *);

int main(int argc, char * argv[]) {
	if (argc != 2) {
		printf("Usage: %s can_interface\n", argv[0]);
		return 0;
	}
	// create a can socket
	int can_socket = create_can_socket(argv[1]);
	apply_filter_to_can_socket(can_socket, RES_SERVICE);

	get_dtcs(can_socket);

	close(can_socket);
	return 0;
}

void get_dtcs(int can_socket) {
	printf("Checking for stored DTCs.\n");
	get_dtcs_from_service(can_socket, STORED_DTC_SERVICE);
	printf("Checking for pending DTCs.\n");
	get_dtcs_from_service(can_socket, PENDING_DTC_SERVICE);
	printf("Checking for permanent DTCs.\n");
	get_dtcs_from_service(can_socket, PERMANENT_DTC_SERVICE);
}

void get_dtcs_from_service(int can_socket, int service) {
	char * data = NULL;
	char command[3] = { 0x2, service, 0x0 };
	can_write_simple(can_socket, DST_SERVICE, command);
	data = can_read(can_socket);	
	process_dtc_buffer(data);
	free(data);
}

void process_dtc_buffer(char * dtc_buffer) {
	if (dtc_buffer != NULL) {
		int dtc_count = (int)dtc_buffer[1];
		printf("Found %d DTCs\n", dtc_count);

		// advance the pointer to the beggining of the dtc codes, two bytes per code
		dtc_buffer += 2;
		char code[6];
		
		while (dtc_count > 0) {
			unsigned int first = ((dtc_buffer)[0] & 0b11000000) >> 6;
			if (first == POWERTRAIN)
				code[0] = 'P';	
			if (first == CHASSIS)
				code[0] = 'C';
			if (first == BODY)
				code[0] = 'B';
			if (first == NETWORK)
				code[0] = 'U';
			
			unsigned int second = ((dtc_buffer)[0] & 0b00110000) >> 4;
			code[1] = (char)(0x30 + second);

			unsigned int third = ((dtc_buffer)[0] & 0b00001111);
			sprintf(&code[2], "%X", third);  

			unsigned int fourth = ((dtc_buffer)[1] & 0b11110000) >> 4;
			sprintf(&code[3], "%X", fourth);  

			unsigned int fifth = ((dtc_buffer)[1] & 0b00001111);
			sprintf(&code[4], "%X", fifth);  

			printf("%s\n", code);

			// reset the buffer
			memset(&code, 0, 6);
			// advance pointer
			dtc_buffer += 2;
			dtc_count--;
		}	
	}
}
