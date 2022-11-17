#include <stdio.h>
#include <endian.h>

#include "../lib/library.h"
#include "../lib/can_read.h"
#include "../lib/can_write.h"

#define DST_SERVICE 0x7df
#define RES_SERVICE 0x7e8

int request_pids_from_service_9(int);
void call_available_pids_onservice_9(int, int);
void print_individual_pids(int, int);
void service_9_query_vin(int);
void service_9_query_calid(int);
void service_9_query_calvn(int);
void service_9_query_ecuname(int);

int main(int argc, char * argv[]) {
	if (argc != 2) {
		printf("Usage: %s can_interface\n", argv[0]);
		return 0;
	}
	// create a can socket
	int can_socket = create_can_socket(argv[1]);
	apply_filter_to_can_socket(can_socket, RES_SERVICE);

	int available_pids_on_service_9 = request_pids_from_service_9(can_socket);	
	call_available_pids_onservice_9(can_socket, available_pids_on_service_9); 

	close(can_socket);
	return 0;
}

void call_available_pids_onservice_9(int can_socket, int available_pids) {
	int base = 0x0;
	
	for (unsigned int mask = 0x80000000; mask > 0; mask = mask >> 1) {
		base += 1;
		if (available_pids & mask) {
			switch(base) {
				case 0x2:
					printf("Querying VIN.\n");
					service_9_query_vin(can_socket);	
					break;
				case 0x4:
					printf("Querying Calibration ID.\n");
					service_9_query_calid(can_socket);	
					break;
				case 0x6:
					printf("Querying Calibration Verification Numbers.\n");
					service_9_query_calvn(can_socket);	
					break;
				case 0x8:
					printf("Querying performance tracking.\n");
					break;
				case 0xa:
					printf("Querying ECU name.\n");
					service_9_query_ecuname(can_socket);	
					break;
			}
		}
	}

	printf("\n");
}

void service_9_query_calvn(int can_socket) {
	char command[3] = { 0x2, 0x9, 0x6 };
	char * data = NULL;
	can_write_simple(can_socket, DST_SERVICE, command);
	data = can_read(can_socket);
	if (data[0] == 0x49 && data[1] == 0x6 && data[2] == 0x1) {
		int id = 0;
		memcpy(&id, data+3, 4);
		printf("Calibration VN: %X\n", htobe32(id));
	}
	free(data);
}

void service_9_query_calid(int can_socket) {
	char command[3] = { 0x2, 0x9, 0x4 };
	char * data = NULL;
	can_write_simple(can_socket, DST_SERVICE, command);
	data = can_read(can_socket);
	if (data[0] == 0x49 && data[1] == 0x4 && data[2] == 0x1) {
		printf("Calibration ID: %s\n", data+3);
	}
	free(data);	
}

void service_9_query_ecuname(int can_socket) {
	char command[3] = { 0x2, 0x9, 0xa };
	char * data = NULL;
	can_write_simple(can_socket, DST_SERVICE, command);
	data = can_read(can_socket);
	if (data[0] == 0x49 && data[1] == 0xa && data[2] == 0x1) {
		printf("ECU name: %s\n", data+3);
	}
	free(data);	
}

void service_9_query_vin(int can_socket) {
	char command[3] = { 0x2, 0x9, 0x2 };
	char * data = NULL;
	can_write_simple(can_socket, DST_SERVICE, command);
	data = can_read(can_socket);
	if (data[0] == 0x49 && data[1] == 0x2 && data[2] == 0x1) {
		printf("VIN: %s\n", data+3);
	}
	free(data);	
}

int request_pids_from_service_9(int can_socket) {
	int supported_pids = 0x0;
	char command[2] = { 0x2, 0x9 };
	char * data = NULL;
	can_write_simple(can_socket, DST_SERVICE, command);
	data = can_read(can_socket);
	if (data[0] == 0x49 && data[1] == 0x0) {
		// copy the four bytes into the integer and return
		memcpy(&supported_pids, data+2, 0x4);
		supported_pids = htobe32(supported_pids);
		print_individual_pids(0x0, supported_pids);
	}
	free(data);	
	
	return supported_pids;
}

void print_individual_pids(int base, int supported_pids) {

	printf("Supported pids: ");

	for (unsigned int mask = 0x80000000; mask > 0; mask = mask >> 1) {
		base += 1;
		if (supported_pids & mask) {
			printf("0x%02x ", base);
		}
	}

	printf("on service 0x9\n");
}
