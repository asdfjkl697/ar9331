#include "udpbroadcast.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int udpbroadcast_init(uint16_t port)
{ 
	int err;
	struct sockaddr_in sin;
	int udpBufferSize = UDP_BUFFER_SIZE;

	SOCKET socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == socket_fd)
	{
#ifdef WIN32
		printf("socket error! error code is %d/n", WSAGetLastError());
#else
		printf("socket error! error code is %d/n", errno);
#endif
		return -1;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port); 
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	int bOpt = 1;
    	setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));
	setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&udpBufferSize, sizeof(int));
	setsockopt(socket_fd, SOL_SOCKET, SO_SNDBUF, (const char*)&udpBufferSize, sizeof(int));

	err = bind(socket_fd, (struct sockaddr*)&sin, sizeof(struct sockaddr));
	if (SOCKET_ERROR == err)
	{
#ifdef WIN32
		printf("socket error! error code is %d/n", WSAGetLastError());
#else
		printf("socket error! error code is %d/n", errno);
#endif
		return -1;
	}

	return socket_fd;
}


int udpbroadcast_send(int socketfd, struct sockaddr_in *addr, char *buffer,int size)
{
	int nSendSize = sendto(socketfd, buffer, size, 0, (struct sockaddr*)addr, sizeof(struct sockaddr)); 
    if(SOCKET_ERROR == nSendSize)  
    {
         printf("sendto error!, error code is %d\r\n", errno);  
         return -1;
	}
	return 0;
}

int udpbroadcast_read(int socketfd,struct sockaddr_in *addr, char *buffer,int size)
{
	int addrin = sizeof(struct sockaddr);
	int ret = recvfrom(socketfd, buffer, size, 0, (struct sockaddr*)addr, (socklen_t *)&addrin);
	if(SOCKET_ERROR == ret){
         printf("sendto error!, error code is %d\r\n", errno);  
         return -1;
	}
    return ret;  
}

void udpbroadcast_close(int socketfd)
{
	close(socketfd);
}
