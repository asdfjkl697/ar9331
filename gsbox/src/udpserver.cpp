#include "udpserver.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "netpacket.h"

#ifndef OS_UBUNTU
static void getNetworkValue(char *szBuf, uint8_t *value)
{
    int i,j;
	uint8_t len;

	for(i =0;i < strlen(szBuf);i++)
    {              
		if(*(szBuf+i)==0x27)
		{			
			memset(value,'\0',18);
			for(j=0;j<18;j++)
			{
				i++;				
				if(*(szBuf+i)==0x27)
				{
					return;
				}
				*value++=*(szBuf+i);
			}
		}
    }
}

static void getNetworkInfo(NETWORK_ADAPTER *adapter,char *interface)
{
    char szBuf[64];
	uint8_t interface_flag=0;
	FILE *fp = NULL;

    if((fp=fopen("/etc/config/network", "r"))==NULL)            
    {
        printf( "Can 't   open   file!\n"); 
        return;
    }
    while(fgets(szBuf,128,fp))                    
    {                         
		if(interface_flag==0)  //jyc20150813add
		{		
			if(strstr(szBuf,interface) != NULL)  
				interface_flag=1;
			continue;
		}  
		if(strstr(szBuf, "ipaddr") != NULL)
        {
			if((++interface_flag)>=3)break; //jyc20151028add	
			getNetworkValue(szBuf, adapter->ipaddr);			
        }
		if(strstr(szBuf, "netmask") != NULL)
        {
			getNetworkValue(szBuf, adapter->netmask);
        }
		if(strstr(szBuf, "gateway") != NULL)
        {
			getNetworkValue(szBuf, adapter->gateway);
        }
		if(strstr(szBuf, "dns") != NULL)
        {
			getNetworkValue(szBuf, adapter->dns);
        }
		if(strstr(szBuf, "macaddr") != NULL)
        {
			memset(adapter->mac, '\0', sizeof(adapter->mac));
			getNetworkValue(szBuf, adapter->mac);
        }
    }			
    fclose(fp);
}
#endif

static uint8_t get_three(char *addr)
{
	uint8_t i,j=0;
	for(i=0;i<strlen(addr);i++)
	{
		if(addr[i]=='.')
			j++;
		if(j==3)
			return i;
	}
}

static uint8_t cmp_route_addr(char *addr1,char *addr2)
{
	uint8_t len;
	len = get_three(addr1);
	if(memcmp(addr1,addr2,len)==0)
		return 0;
	else
		return 1;
}


UDPServer::UDPServer(uint16_t port)
:_port(port)
{
#ifndef OS_UBUNTU
	getNetworkInfo(&adapter,"'lan'");
	getNetworkInfo(&adapter2,"'wwan'");
#else
	_version = (uint8_t *)malloc(sizeof(20));
	const char* mac = "00:11:cc:dd:33:ff";
	memset(adapter.mac, '\0', sizeof(adapter.mac));
	memcpy(adapter.mac, mac, strlen(mac));
	memcpy(_version, VERSION_SOFT, sizeof(VERSION_SOFT));
#endif
}

UDPServer::~UDPServer(void)
{
#ifdef OS_UBUNTU
	free(_version);
#endif
}

void UDPServer::PacketRead(uint8_t *data, int size, struct sockaddr_in *addr)
{
    NetPacket pkt(data, size);	
	if(pkt.Decode()){
		switch(pkt.GetCommand()){
			case CMD_BROADCAST_INFO:
			{
				NETWORK_ADAPTER *adp;
				char *route_addr;
				route_addr=inet_ntoa(addr->sin_addr);
				if((cmp_route_addr(route_addr, (char *)adapter2.ipaddr)==0)&&cmp_route_addr(route_addr, (char *)adapter.ipaddr))
					adp = &adapter2;
				else
					adp = &adapter;

				pkt.ResetToSend();
				pkt.EncodeString((const char *)adp->ipaddr); //ipaddress, replace with a real one
				pkt.EncodeString((const char *)adp->netmask);//mask, replace with a real one
				pkt.EncodeString((const char *)adp->gateway);//gateway, replace with a real one
				pkt.EncodeString((const char *)adp->dns);//DNS1, replace with a real one
				pkt.EncodeString("");//DNS2, replace with a real one
				pkt.EncodeInt16(80); //WEB PORT,replace with a real one
				pkt.EncodeInt16(DATA_SERVER_PORT); //COM PORT,replace with a real one
				pkt.EncodeInt16(SERVER_PORT); //DATA PORT
				pkt.EncodeString((const char *)adp->mac); //MAC
				pkt.EncodeString(DEVICE_NAME); //DEVICE NAME
				pkt.EncodeBoolean(0); //USE DHCP,replace with a real one
				pkt.EncodeString(DEVICE_ID); //DEVICE ID
				pkt.EncodeString((const char *)_version); //VERSION VERSION_SOFT 
				//enc = packet_encodeString(enc, VERSION_SOFT);

				pkt.AutoAck();
				pkt.EncodeBuffer();				
				this->Send(addr, (char *)pkt.getBuffer(), pkt.getLength());
				break;
			}
		}
	}
}

bool UDPServer::Open()
{
	int err;
	struct sockaddr_in sin;
	int udpBufferSize = UDP_BUFFER_SIZE;

	socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == socket_fd)
	{
#ifdef WIN32
		printf("socket error! error code is %d/n", WSAGetLastError());
#else
		printf("socket error! error code is %d/n", errno);
#endif
		return false;
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(_port); 
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
		return false;
	}

	return true;
}

int UDPServer::Send(struct sockaddr_in *addr, char *buffer,int size)
{
	int nSendSize = sendto(socket_fd, buffer, size, 0, (struct sockaddr*)addr, sizeof(struct sockaddr)); 
    if(SOCKET_ERROR == nSendSize)  
    {
         printf("sendto error!, error code is %d\r\n", errno);  
         return -1;
	}
	return nSendSize;
}

int UDPServer::Read(struct sockaddr_in *addr, char *buffer,int size)
{
	int addrin = sizeof(struct sockaddr);
	int ret = recvfrom(socket_fd, buffer, size, 0, (struct sockaddr*)addr, (socklen_t *)&addrin);
	if(SOCKET_ERROR == ret){
         printf("sendto error!, error code is %d\r\n", errno);  
         return -1;
	}
    return ret;  
}

void UDPServer::Close()
{
	close(socket_fd);
}
