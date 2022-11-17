#ifndef CAN_READ
#define CAN_READ

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <linux/can.h>
#include "library.h"

#define FLOW_CONTINUE 0x0
#define FLOW_WAIT 0x1
#define FLOW_ABORT 0x2

char * can_read(int);

#endif
