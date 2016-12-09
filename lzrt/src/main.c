/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
* main.c
shell-init: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
chdir: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
* Copyright (C) 2014 gsj0791 <gsj0791@163.com>
*
* 串口转tcp程序 使用libevent库  协议待定
* 单线程串口不能和其他进程复用
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "packet.h"
#include "udpbroadcast.h"
#include "tcpserver.h"
#include "taskmanager.h"
#include "hardware.h"
#include "rs232.h"

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>

#include "libxml/xmlmemory.h"
#include "libxml/parser.h"
//#include <libxml/parser.h>
//#include <libxml/tree.h>

//#define DEVICE_ID "TW001809"
#define DEV_CONFIG_FILE "dev.xml"

char *dev_id;
struct bufferevent *bev;
uint8_t send_err_cnt=0;
uint8_t reboot_cnt=0;

typedef struct
{
	uint8_t state;
	uint8_t old_state;
	uint8_t cmd;
	uint8_t num;
	uint16_t len;
	uint16_t runtime;
}dev_data;
dev_data glq;


int16_t packet_s_readInt16(uint8_t *data)
{
	return (data[1] & 0xFF) | (data[0] << 8);
}

void readXml(){
	xmlDocPtr doc=NULL;
	xmlNodePtr cur;
	doc = xmlParseFile(DEV_CONFIG_FILE);
	if (doc == NULL) return;
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL){
		xmlFreeDoc(doc);
	}
	if (!xmlStrEqual(cur->name, BAD_CAST"dev")){
		xmlFreeDoc(doc);
		return;
	}
	if (xmlHasProp(cur, BAD_CAST"id")){
		dev_id = (char *)xmlGetProp(cur, BAD_CAST"id");
	}
	xmlFreeDoc(doc);
}

void saveXml(char *id){
	xmlDocPtr doc;
	xmlNodePtr curNode = NULL;
	char temp[64];
	doc = xmlNewDoc(BAD_CAST "1.0");
	curNode = xmlNewNode(NULL, BAD_CAST "dev");
	xmlSetProp(curNode, BAD_CAST"id", BAD_CAST id);
	xmlDocSetRootElement(doc, curNode);
	xmlSaveFileEnc(DEV_CONFIG_FILE, doc, "UTF-8");
	xmlFreeDoc(doc);
}

void sendtoserver()
{
	uint8_t test[34];
	uint8_t randata=rand();
	uint8_t ret;
	
	readXml();

	glq.len=32;
	test[0]=glq.len>>8;
	test[1]=glq.len&0xff;
	memcpy(&test[2],dev_id,8);	
		
	test[10]=0x30+glq.num/10;
	test[11]=0x30+glq.num%10;

	glq.cmd=0x3a;
	test[12]=glq.cmd;	

	uint16_t temprature=160+randata%15;
	test[13]=0x30+temprature/100;
	test[14]=0x30+(temprature%100)/10;
	test[15]=0x30+temprature%10;

	uint16_t humidity=600+randata%20;
	test[16]=0x30+humidity/100;
	test[17]=0x30+(humidity%100)/10;
	test[18]=0x30+humidity%10;

	uint16_t press=300+randata%11;
	test[19]=0x30;
	test[20]=0x30+press/100;
	test[21]=0x30+(press%100)/10;
	test[22]=0x30+press%10;

	uint16_t ppm=1200+randata%100;
	test[23]=0x30+ppm/1000;
	test[24]=0x30+(ppm%1000)/100;
	test[25]=0x30+(ppm%100)/10;
	test[26]=0x30+ppm%10;

	uint16_t wind=50+randata%50;
	test[27]=0x30+wind/100;
	test[28]=0x30+(wind%100)/10;
	test[29]=0x30+wind%10;

	if(glq.state>10){
		if(glq.runtime>30){
			glq.state++;
			glq.runtime=0;
		}
		if(glq.state>15)glq.state=0;
	}
	else if(glq.state){
		if(glq.runtime>999){
			glq.runtime=0;
		}
	}
	test[30]=0x30+glq.runtime/100;
	test[31]=0x30+(glq.runtime%100)/10;
	test[32]=0x30+glq.runtime%10;
	test[33]=0x30+glq.state%10;
	
	bufferevent_write(bev, test, 34);
}

void read_callback(struct bufferevent *bev, void *ctx) {
	struct evbuffer *input = bufferevent_get_input(bev);
	int size = evbuffer_get_length(input);
	uint8_t buf[255],i;
	char dev_id_modify[8]="TW001801";
	evbuffer_remove(input,buf,size);
	//memcpy(bufp,buf,size);
	//printf("%s\n",&buf[2]);	

	uint16_t len=packet_s_readInt16(buf);
	//printf("%d\n",len);
	reboot_cnt=0;

	if((size==len+2)&&(memcmp(&buf[2],dev_id,8)==0)){
		if(buf[13]!=65)return;
		//printf("operation...\n");
		glq.old_state=glq.state;

		if(glq.state==0){
			if(++glq.num>99)glq.num=1;
		}
				
		if(buf[14]==65&&buf[15]==65){
			glq.state=11;
		}
		else if(buf[14]==65&&buf[15]==66){
			glq.state=1;
		}
		else if(buf[14]==65&&buf[15]==67){
			glq.state=2;
		}	
		else if(buf[14]==65&&buf[15]==68){
			glq.state=3;
		}
		else if(buf[14]==65&&buf[15]==69){
			glq.state=4;
		}	
		else if(buf[14]==65&&buf[15]==70){
			glq.state=5;
		}
		else if(buf[14]==65&&buf[15]==71){
			glq.state=0;
		}
		else if(buf[14]==65&&buf[15]==90){
			sendtoserver();
		}
		else if(buf[14]==69){ //更改id
			dev_id_modify[5]=buf[15];
			dev_id_modify[6]=buf[16];
			dev_id_modify[7]=buf[17];
			//printf("%s\n",dev_id_modify);
			saveXml(dev_id_modify);			
		}
		if(glq.state!=glq.old_state){
			glq.runtime=0;
			sendtoserver();
		}
	}	
	//evbuffer_write(input, 1);
}

void event_callback(struct bufferevent *bev, short events, void *ctx) {
	if (events & BEV_EVENT_CONNECTED) {
		printf("connected");
	} else if (events & BEV_EVENT_EOF) {
		printf("Connection closed.\n");
	} else if (events & BEV_EVENT_ERROR) {
		printf("Got an error on the connection: %s\n",
				strerror(errno));
	}
}

static void createTcpClient(TASK *task){
	//struct event_base *base;
	struct sockaddr_in sin;
	//struct bufferevent *bev;

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(10002);
	inet_aton("112.124.115.184", &sin.sin_addr);

	memset(sin.sin_zero, 0x00, 0);

	bev = bufferevent_socket_new(task->evbase, -1, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, read_callback, NULL, event_callback, NULL);
	if (bufferevent_socket_connect(bev, 
				(struct sockaddr *)&sin, sizeof(sin)) < 0) {
		fprintf(stderr, "Could not connect!\n");
		bufferevent_free(bev);	
		return;
	}
	bufferevent_enable(bev, EV_READ|EV_WRITE); 
}

void TaskManager_Timeout(evutil_socket_t fd, short what, void *arg)
{
	char chars[100];	
	time_t timep;
    time (&timep);
	
	if(glq.state)glq.runtime++;	
	sendtoserver();
	
	if(++reboot_cnt<5)return;
	//system("reboot");
	sprintf(chars,"echo 'exit %s'>>exit.log",asctime(gmtime(&timep)));
	system(chars);
	exit(0);
}

void taskmanager_init(TASK *task)
{
	struct timeval tv;

	task->evbase = event_base_new();

	tv.tv_sec = 60;
	tv.tv_usec = 0;

	task->timeout_event = event_new(task->evbase, -1, EV_PERSIST, TaskManager_Timeout, task);
	event_add(task->timeout_event, &tv);

	task->queue = NULL;
}


int main(void)
{
	serial_packet prev_packet = {0};
	TASK task = {0};
	BOARD _board = {0};
	
	srand(time(0));

	_board.prve_packet = &prev_packet;
	_board.task = &task;
	
	taskmanager_init(&task);


	createTcpClient(&task);

	taskmanager_run(&task); //loop until error
	taskmanager_close(&task);

	hardware_close(&_board);

	return (0);
}
