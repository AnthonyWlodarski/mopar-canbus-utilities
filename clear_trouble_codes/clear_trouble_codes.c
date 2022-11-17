#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <endian.h>
#include "../lib/library.h"
#include "../lib/can_write.h"

#define DST_SERVICE 0x7df
#define RES_SERVICE 0x7e8

void clear_trouble_codes(int);

int main(int argc, char * argv[]) {
	if (argc != 2) {
		printf("Usage: %s can_interface\n", argv[0]);
		return 0;
	}
	// create a can socket
	int can_socket = create_can_socket(argv[1]);
	apply_filter_to_can_socket(can_socket, RES_SERVICE);

	printf("Clearing trouble codes.\n");
	clear_trouble_codes(can_socket);

	close(can_socket);
	return 0;
}

void clear_trouble_codes(int can_socket) {
	char command[2];
	command[0] = 0x1;
	command[1] = 0x4;
	can_write_simple(can_socket, DST_SERVICE, command);
}
