#ifndef _HARDWARE_H
#define _HARDWARE_H

#include "typedef.h"

#define USESERIAL 1
#define SERIAL_BAND 115200 
#define SERIAL_PORT 15
//#define SERIAL_PORT 16 ///dev/ttyUSB0

//int hardware_init(BOARD *board);
void hardware_close(BOARD *board);
int hardware_send(uint8_t *data, int size);

#ifndef OS_UBUNTU
void hardware_soft_version(uint8_t *soft_version);
#endif

#endif
