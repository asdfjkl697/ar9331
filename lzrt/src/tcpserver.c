#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <shadow.h>
#include <unistd.h>
#include <crypt.h>

#include "event2/bufferevent.h"
#include "event2/buffer.h"

#include "tcpserver.h"
#include "hardware.h"
#include "taskmanager.h"
#include "client.h"

#include "obsolete.h"

#define _XOPEN_SOURCE

static int checklogin(SERVICE *serv, const char *user, const char *pass)
{
	if(serv->logged){
		return (strcmp(user, serv->username) == 0 && strcmp(pass, serv->password) == 0);
	}else{		
#ifdef OS_UBUNTU	
		memcpy(serv->username, "admin", strlen("admin"));
		memcpy(serv->password, "123456", strlen("123456"));
		serv->logged = 1;
#else
		struct spwd *spw;
		char key_buf[48];
		char *key=key_buf;
		char salt[12];

		memset(key,'\0',sizeof(key));
		memset(salt,'\0',sizeof(salt));
  
		spw=getspnam(user);
		if(spw==NULL)
		{
			printf("getspnam err\n");
			return 0;
		}

		//printf("spw->sp_pwdp:%s\n",spw->sp_pwdp);
		memcpy(salt,spw->sp_pwdp,12);
		//printf("salt:%s\n",salt);
		key=crypt(pass, salt);
		//printf("key:%s\n",key);
		if(!memcmp(key,spw->sp_pwdp, strlen(key))) //login err
		{
			memcpy(serv->username, user, strlen(user));
			memcpy(serv->password, pass, strlen(pass));

			serv->logged = 1;

			return 1;
		}
#endif
	}
	return 0;
}


static void onPacketRead(clients *client, packet *pkt)
{
	uint8_t temp[260] = {0};
	
	if(client->logged == 0 && pkt->command != CMD_LOGIN){
		uint8_t buf[128];
		uint8_t *enc = buf;

		enc = packet_encodeBoolean(enc, 0x0);
		enc = packet_encodeString(enc, "not login");
		
		pkt->id = 0;
	    pkt->command = CMD_LOGIN;
		pkt->ack = pkt->ack == ACK_ASK ? ACK_REPLY : ACK_NOP;
		pkt->length = enc - buf;
		pkt->data = buf;

		int size = packet_readBuffer(pkt, temp);

		bufferevent_write(client->buf_ev, temp, size);
		return;
	}
	
	switch(pkt->command)
	{
		case CMD_SERIAL:
		{
			uint8_t *enc = temp;
			uint8_t len = pkt->length;

			*enc++= 0xBC;
			*enc++= 0xAC;
			*enc++= len;
			memcpy(enc, pkt->data, len);
			enc+= len;
			*enc++= 0xAC;
			//printf("send %d\n",len);
			hardware_send(temp, len+4);
			break;
		}
		case CMD_DEVICE_INFO:
		{
			uint8_t *enc;
			uint8_t version[20] = {0};
			struct timeval tv;
			gettimeofday(&tv,NULL);

			#ifndef OS_UBUNTU
				hardware_soft_version(&version); //jyc20150926add
    			#endif

			enc = pkt->data;
			enc = packet_encodeString(enc, version); //jyc20150926modify
			enc = packet_encodeString(enc, VERSION_HARDWARE);
			enc = packet_encodeInt32(enc, tv.tv_sec);
			            
			pkt->length = enc - pkt->data;
		    pkt->ack = pkt->ack == ACK_ASK ? ACK_REPLY : ACK_NOP;	
		    int size = packet_readBuffer(pkt, temp);
		    bufferevent_write(client->buf_ev, temp, size);
			break;
		}
		case CMD_SCHEDULE_TASK:
		{
			break;
		}
		case CMD_TRIGGER:
		{
			break;
		}
		case CMD_LOGIN:
		{
			char temp_user[50] = {0};
			char temp_pass[50] = {0};
			str_st username = {0};
			str_st password = {0};

			uint8_t *enc = pkt->data;

			enc = packet_decodeString(enc, &username);
			enc = packet_decodeString(enc, &password);

			memcpy(temp_user, username.str, username.len);
			memcpy(temp_pass, password.str, password.len);

			enc = pkt->data;
			
			if(checklogin(client->serv, temp_user, temp_pass)==1){
				client->logged = 0x1;
				enc = packet_encodeBoolean(enc, 0x1);
		        enc = packet_encodeString(enc, "logged");
			}else{ 
				client->logged = 0x0;
				enc = packet_encodeBoolean(enc, 0x0);
		        enc = packet_encodeString(enc, "username or password error");
			}

			pkt->length = enc - pkt->data;
		    pkt->ack = pkt->ack == ACK_ASK ? ACK_REPLY : ACK_NOP;	
		    int size = packet_readBuffer(pkt, temp);
		    bufferevent_write(client->buf_ev, temp, size);
			break;
		}
	    case CMD_SYSTEM:
		{
			uint8_t *enc = pkt->data;
			uint8_t type = *enc++;
			if(type == DATA_KEYVALUE){
			    char temp_key[50] = {0};
			    char temp_val[50] = {0};
				str_st key = {0};
				str_st val = {0};
				enc = packet_decodeString(enc, &key);
				enc = packet_decodeString(enc, &val);
				
                memcpy(temp_key, key.str, key.len);
			    memcpy(temp_val, val.str, val.len);
				
				if(strcmp(temp_key, "system")==0){
					
					if(strcmp(temp_val, "reboot")==0){
			             enc = pkt->data;
				         enc = packet_encodeBoolean(enc, 0x1);
		                 enc = packet_encodeString(enc, "reboot");
						
			             system("reboot");
					}else if(strcmp(temp_val, "update")==0){
						 type = *enc++;
						 if(type == DATA_KEYVALUE){
				             enc = packet_decodeString(enc, &key);
				             enc = packet_decodeString(enc, &val);
							 memcpy(temp_key, key.str, key.len);
							 memcpy(temp_val, val.str, val.len);
							 if(strcmp(temp_key, "url")==0){
								 //do something,if needed to use remote url
							 }
						 }
						
			             enc = pkt->data;
				         enc = packet_encodeBoolean(enc, 0x1);
		                 enc = packet_encodeString(enc, "update");
								
			             system("/root/upgsbox.sh");	
						
					}else if(strcmp(temp_val, "update-firmware")==0){
						 type = *enc++;
						 if(type == DATA_KEYVALUE){
				             enc = packet_decodeString(enc, &key);
				             enc = packet_decodeString(enc, &val);
							 memcpy(temp_key, key.str, key.len);
							 memcpy(temp_val, val.str, val.len);
							 if(strcmp(temp_key, "url")==0){
								 //do something,if needed to use remote url
							 }
						 }
						
			             enc = pkt->data;
				         enc = packet_encodeBoolean(enc, 0x1);
		                 enc = packet_encodeString(enc, "update-firmware");
						
						 system("/root/upfirmware.sh");	
					}	
					
			        pkt->length = enc - pkt->data;
		            pkt->ack = pkt->ack == ACK_ASK ? ACK_REPLY : ACK_NOP;	
		            int size = packet_readBuffer(pkt, temp);
		            bufferevent_write(client->buf_ev, temp, size);
				}
			}
			break;
		}
	}

	if(pkt->ack == ACK_ASK){		
		pkt->ack = ACK_REPLY;
		pkt->length = 0;
		
		int size = packet_readBuffer(pkt, temp);
		bufferevent_write(client->buf_ev, temp, size);
	}
}

static void connection_read(struct bufferevent *bev, void *arg)
{
	int size;
	char buffer[BUFFER_SIZE];
	packet pkt;	
	clients *client = (clients *)arg;
	struct evbuffer *input = bufferevent_get_input(bev);
	
	while ((size = evbuffer_remove(input, buffer, BUFFER_SIZE)) > 0) {
		int read_size = 0;
		uint8_t *enc = (uint8_t *)buffer;

		while(size > 0){
			read_size = packet_decode(&pkt, enc, size);
		    if(read_size <= 0 || read_size > size)
		    {				
				break;
		    }

			onPacketRead(client, &pkt);
			
		    enc += read_size;
			size -= read_size;
		}
	}
}

static void connection_event(struct bufferevent *bev, short what, void *arg)
{
	if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
		bufferevent_free(bev);
	}

	client_remove((clients *)arg);
}

void connection_accept(struct evconnlistener *listener,
	evutil_socket_t fd, struct sockaddr *address, int socklen,
	void *ctx)
{
	/* We got a new connection! Set up a bufferevent for it. */
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(
		base, fd, BEV_OPT_CLOSE_ON_FREE);

	SERVICE *_service = (SERVICE *)ctx;

	clients *client = client_create();
	client->prve_packet = NULL;
	client->serv = _service;
	client->buf_ev = bev;

	if(_service->service_type){
		bufferevent_setcb(bev, obsolete_connection_read, NULL, connection_event, client);
	}else{
		bufferevent_setcb(bev, connection_read, NULL, connection_event, client);
	}
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void connection_accept_error(struct evconnlistener *listener, void *ctx)
{
	struct event_base *base = evconnlistener_get_base(listener);
	int err = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Got an error %d (%s) on the listener. "
		"Shutting down.\n", err, evutil_socket_error_to_string(err));

	event_base_loopexit(base, NULL);
}
