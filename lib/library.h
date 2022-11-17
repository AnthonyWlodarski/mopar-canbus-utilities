#ifndef LIBRARY
#define LIBRARY

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define SINGLE_FRAME 0x0
#define FIRST_FRAME 0x1
#define CONSECUTIVE_FRAME 0x2
#define FLOW_CONTROL_FRAME 0x3

#define POWERTRAIN 0x0
#define CHASSIS 0x1
#define BODY 0x2
#define NETWORK 0x3

int create_can_socket(char *);
void apply_filter_to_can_socket(int, int);

#endif
