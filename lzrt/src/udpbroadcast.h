#ifndef _UDPBROADCAST_H_
#define _UDPBROADCAST_H_

#include "typedef.h"
#include <stdint.h>
#include <arpa/inet.h>


#define GETDEVICEINFO 0x10

int udpbroadcast_init(uint16_t port);
int udpbroadcast_send(int socketfd, struct sockaddr_in *addr, char *buffer,int size);
void udpbroadcast_close(int socketfd);


#endif