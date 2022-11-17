#include "library.h"

int create_can_socket(char * identifier) {
	int can_socket = -1;
	struct sockaddr_can addr;
	struct ifreq ifr;

	can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	
	if (-1 == can_socket) {
		// errno should be set
		int curr_error = errno;
		perror("socket");
		exit(curr_error);		
	}

	// based on the header there is a maximum of 16 bytes allowed here
	int length = strlen(identifier);
	if (length >= IF_NAMESIZE) {
		strncpy(ifr.ifr_name, identifier, 15);
		ifr.ifr_name[IF_NAMESIZE] = 0x0;
	} else {
		strncpy(ifr.ifr_name, identifier, length);
		ifr.ifr_name[length+1] = 0x0;
	}

	if (-1 == ioctl(can_socket, SIOCGIFINDEX, &ifr)) {
		// errno should be set
		int curr_error = errno;
		perror("ioctl");
		exit(curr_error);
	}

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (-1 == bind(can_socket, (struct sockaddr *)&addr, sizeof(addr))) {
		// errno should be set
		int curr_error = errno;
		perror("bind");
		exit(curr_error);
	}

	return can_socket;
}

void apply_filter_to_can_socket(int can_socket, int filter_service) {
	struct can_filter rfilter;
	rfilter.can_id = filter_service;
	rfilter.can_mask = filter_service;
	setsockopt(can_socket, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
}
