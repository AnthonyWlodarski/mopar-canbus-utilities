#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <endian.h>
#include "../lib/library.h"
#include "../lib/can_write.h"
#include "../lib/can_read.h"

#define DST_SERVICE 0x7df
#define RES_SERVICE 0x7e8

void print_individual_pids(int, int);
void print_supported_pids(int);

int main(int argc, char * argv[]) {
	if (argc != 2) {
		printf("Usage: %s can_interface\n", argv[0]);
		return 0;
	}
	// create a can socket
	int can_socket = create_can_socket(argv[1]);
	apply_filter_to_can_socket(can_socket, RES_SERVICE);

	printf("Checking for supported PIDS\n");
	print_supported_pids(can_socket);

	close(can_socket);
	return 0;
}

void print_supported_pids(int can_socket) {
	for (int x = 0x0; x < 0xC1; x += 0x20) {
		char * buffer = NULL;
		char command[3];
		command[0] = 0x2;
		command[1] = 0x1;
		command[2] = x;
		// query service 0x1 on function 0 and get all supported pids after
		can_write_simple(can_socket, DST_SERVICE, command);
		buffer = can_read(can_socket);

		// IAW with the protocol we should have a six byte buffer back, the first byte
		// should be the service + 0x40 which means a succesfull call, then the function
		// number which in this instance should be 0x0, then four bytes of bit encoding
		// to tell us which pids it supports
		int pids = 0x0;
		if (buffer[0] == 0x41 && buffer[1] == x) {
			memcpy(&pids, buffer+2, 0x4);
			pids = htobe32(pids);
			print_individual_pids(x, pids);			
		}
		free(buffer);
		
		if ((pids & 0x1) == 0x0) {
			break;
		}

	}
}

void print_individual_pids(int base, int supported_pids) {

	printf("Supported pids: ");

	for (unsigned int mask = 0x80000000; mask > 0; mask = mask >> 1) {
		base += 1;
		if (supported_pids & mask) {
			printf("0x%02x ", base);
		}
	}

	printf("\n");
}
