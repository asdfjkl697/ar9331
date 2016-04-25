#include "hardware.h"
#include "rs232.h"
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "client.h"
#include "obsolete.h"


static void onPacketRead(serial_packet *data)
{
	int size;
	uint8_t temp[BUFFER_SIZE];
	packet pkt;
	clients *cl = client_first();	
    //package
	pkt.id = 0;
	pkt.command = CMD_SERIAL;
	pkt.ack = ACK_NOP;
	pkt.length = data->read_size;
	pkt.data = data->data;
	
	size = packet_readBuffer(&pkt, temp);

	while(cl != NULL){
		if(cl->logged){
		    bufferevent_write(cl->buf_ev, temp, size);
		}else if(cl->serv->service_type){
			obsolete_send_packet(cl, data);
		}
		cl = cl->next;
	}
}



void hardware_close(BOARD *board)
{
#ifdef USESERIAL	
	event_del(board->serial_evt);
	event_free(board->serial_evt);
	
    RS232_CloseComport(SERIAL_PORT);
#endif
}

int hardware_send(uint8_t *data, int size)
{
#ifdef USESERIAL	
    return RS232_SendBuf(SERIAL_PORT, data, size);
#endif
}

#ifndef OS_UBUNTU
void hardware_soft_version(uint8_t *soft_version)
{
	FILE *stream;
	char buf[64];

	memset(buf,'\0',sizeof(buf));
	stream = popen("opkg list gsbox","r");
	fread(buf,sizeof(char),sizeof(buf),stream);
	memcpy(soft_version,&buf[8],10); //gsbox 

	pclose(stream);	
}

#endif

