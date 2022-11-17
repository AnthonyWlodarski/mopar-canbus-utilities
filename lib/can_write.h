#ifndef CAN_WRITE
#define CAN_WRITE

#include <linux/can.h>
#include <unistd.h>
#include <string.h>

ssize_t can_write_simple(int, int, char *);

#endif
