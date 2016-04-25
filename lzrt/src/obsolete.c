#include "obsolete.h"
#include "hardware.h"
#include "taskmanager.h"
#include "client.h"

#include "event2/bufferevent.h"
#include "event2/buffer.h"

#include <stdlib.h>
#include <string.h>


static int receiver_freq = 0;
static int transmit_freq = 0;


static void obsolete_getFreq()
{
	uint8_t data[8], buffer[64];
	uint8_t *enc = data;

	*enc++= 0x65; 
	*enc++= SYSTEM_SET; 
	*enc++= RF_Receiver; //module
	*enc++= SYSTEM_READ;
	
	enc = packet_to_hardware(buffer, data, enc - data);
	hardware_send(buffer, enc - buffer);
}


static void delayHardwareCall(void *arg)
{
	clients *client = (clients *)arg;
    if(client->prve_packet != NULL && client->prve_packet->next != NULL){		 
	     uint8_t buffer[BUFFER_SIZE];
	     uint8_t *enc;
         serial_packet *prve = client->prve_packet->next;

		 enc = packet_to_hardware(buffer, prve->data, prve->data_size);
	     hardware_send(buffer, enc - buffer);

		 free(prve);
		 client->prve_packet->next = NULL;
	}

	taskmanager_remove_timer(client->serv->timetask);
}

static void real_packet_transform(serial_packet *pkt, clients *client)
{
	uint8_t buffer[BUFFER_SIZE];
	uint8_t *enc;
	uint8_t type = pkt->data[1];
	uint32_t freq;

	switch(type)
	{
		case OLD_RXB8_315:
			pkt->data[1] = RF_Receiver;
			freq = 315000000;
			break;
	    case OLD_RXB8_433:
			pkt->data[1] = RF_Receiver;
			freq = 433920000;
			break;
		case OLD_TX315_RAW:
			pkt->data[1] = RF_Transmit;
			freq = 315000000;
			break;
	    case OLD_TX433_RAW:
			pkt->data[1] = RF_Transmit;
			freq = 433920000;
			break;
	    case OLD_CC1101_433:
			pkt->data[1] = CC1101;
			break;
	    case OLD_CAN_Chip:
			pkt->data[1] = CAN;
			break;
	    case OLD_RS485:
			pkt->data[1] = RS485;
			break;
	    case OLD_GSM:			
			return;
	    case OLD_Module_Set:
			pkt->data[1] = SYSTEM_INFO;
			break;
	    case SYSTEM_SET:
			break;
	    case 0x3a:
			break;
	    case 0x3b:
			break;
	    default:
			return;
    }

	if(pkt->data[1] == RF_Receiver){
	     enc = packet_setfreq(buffer, RF_Receiver, freq);;
	     hardware_send(buffer, enc - buffer);
	}else if(pkt->data[1] == RF_Transmit && transmit_freq != freq){
		 SERVICE *serv = client->serv;
	     enc = packet_setfreq(buffer, RF_Transmit, freq);;
	     hardware_send(buffer, enc - buffer);
		
		 client->prve_packet->next = malloc(sizeof(serial_packet));
		 memcpy(client->prve_packet->next, pkt, sizeof(serial_packet));
				
	     if(serv->timetask == NULL){
			serv->timetask = malloc(sizeof(TASK_TIME));
		    serv->timetask->callback = delayHardwareCall;
		 }				
		 serv->timetask->arg = client;
		 taskmanager_add_timer(serv->task, serv->timetask, 100);
	}else{
		 enc = packet_to_hardware(buffer, pkt->data, pkt->data_size);
	     hardware_send(buffer, enc - buffer);
	}
}

void obsolete_connection_read(struct bufferevent *bev, void *arg) 
{
	int size;
	char buffer[BUFFER_SIZE];
	clients *client = (clients *)arg;	
	serial_packet *prve;
	
	
	
	struct evbuffer *input = bufferevent_get_input(bev);

	if(client->prve_packet==NULL){
		prve = client->prve_packet = malloc(sizeof(serial_packet));
		packet_init_serial(prve);
	}else{
		prve = client->prve_packet;
	}
	
	while ((size = evbuffer_remove(input, buffer, BUFFER_SIZE)) > 0) {
		int index = 0;
		uint8_t *data = (uint8_t *)buffer;
		
        for(; index< size; index++){
	       if(prve->data_size == 0){
		      if(data[index] == 0xBC && data[index+1]==0xAC){  //double size chack
				 prve->head = data[index++];
				 prve->ack = data[index++];
				 prve->data_size = data[index];
			  }
	       }else if(prve->read_size < prve->data_size){
			  prve->data[prve->read_size++] = data[index];
		   }else{
			  prve->end_ack = data[index];
			  if(prve->end_ack == prve->ack){
			/*printf("obssend %d\n",prve->read_size);
			uint16_t i;
			for(i=0;i<prve->read_size;i++)
				printf("%x ",prve->data[i]);
			printf("\n");*/
                  real_packet_transform(prve, client);
			  }
			  prve->data_size = 0;
			  prve->read_size = 0;
		   }
	    }	
	}
}

void obsolete_send_packet(clients *client, serial_packet *packet)
{
	uint8_t *enc;
	uint8_t type = packet->data[1];

	switch(type){
	    case CC1101:
			type = OLD_CC1101_433;
			break;
	    case CAN:
			type = OLD_CAN_Chip;
			break;
	    case RS485:
			type = OLD_RS485;
			break;
	    case SYSTEM_INFO:
			type = OLD_Module_Set;
			break;
		case SYSTEM_RETURN:
		{
			uint8_t module, act;
			enc = packet->data;
			enc+=2;
			module = *enc++;			
			act    = *enc++;

			if(act == SYSTEM_READ || (act == SYSTEM_WIRTE)){
			   switch(module)
			   {
				  case RF_Receiver:
				  {
					 receiver_freq = packet_readInt32(enc);
					 break;
				  }
				  case RF_Transmit:
				  {
					 transmit_freq = packet_readInt32(enc);
					 break;
				  }
			   }
			}
			break;
			//return;
		}
	}

	if(type == RF_Receiver){
		uint8_t ready = 0;
		if(receiver_freq > 0){
			if(receiver_freq == 315000000){
				//type  = OLD_RXB8_315;
				type  = OLD_TX315_RAW;
				ready = 1;
			}
			if(receiver_freq == 433920000){
				//type  = OLD_RXB8_433;
				type  = OLD_TX433_RAW;
				ready = 1;
			}
		}
		
		if(!ready){
		   obsolete_getFreq();
		   return;
		}
	}
		
	uint8_t temp[BUFFER_SIZE];
	enc = temp;

	packet->data[1] = type;

	*enc++= packet->head;
	*enc++= packet->ack;
	*enc++= packet->data_size;
	memcpy(enc, packet->data, packet->data_size);
	enc+= packet->data_size;
	*enc++= packet->end_ack;
	
	bufferevent_write(client->buf_ev, temp, enc - temp);
}
