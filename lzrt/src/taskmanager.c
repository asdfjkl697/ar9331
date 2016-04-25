#include "taskmanager.h"
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>


void Socket_Read(evutil_socket_t fd, short what, void *arg)
{
	int ret;
	struct sockaddr_in sin_from;
	int addr_len = sizeof(struct sockaddr);
	char buffer[UDP_BUFFER_SIZE];
	TASK_SOCK *sock = arg;

	ret = recvfrom(fd, buffer, UDP_BUFFER_SIZE, 0, (struct sockaddr *)&sin_from, (socklen_t*)&addr_len);
	if (SOCKET_ERROR == ret){
		printf("sendto error!, error code is %d\r\n", errno);
		return; // -1;
	}

	sock->callback(sock, (uint8_t *)buffer, ret, &sin_from);
}






void TaskManager_OnTimer(evutil_socket_t fd, short what, void *arg)
{
	TASK_TIME *handler = (TASK_TIME *)arg;
	handler->callback(handler->arg);
}



void taskmanager_free(TASK *task)
{
	TASK_QUEUE *next;
	TASK_QUEUE *queue = task->queue;
	while (queue){
		next = queue->next;
		free(queue);
		queue = next;
	}

	//event_del(task->timeout_event);
	//event_free(task->timeout_event);

	taskmanager_remove_socket(task);
	event_base_free(task->evbase);
}

void taskmanager_add_socket(TASK *task, int socket_fd, task_socket_callback *callback)
{
	TASK_SOCK *sock = &task->sock;
	sock->socket_fd = socket_fd;
	sock->callback = callback;

	sock->evt = event_new(task->evbase, socket_fd, EV_READ | EV_PERSIST, Socket_Read, sock);
	event_add(sock->evt, NULL);
}

void taskmanager_remove_socket(TASK *task)
{
	event_del(task->sock.evt);
	event_free(task->sock.evt);
}

void taskmanager_add_timer(TASK *task, TASK_TIME *handler, uint32_t millisecond)
{
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = millisecond * 1000;

	handler->evt = event_new(task->evbase, -1, EV_PERSIST, TaskManager_OnTimer, handler);
	event_add(handler->evt, &tv);
}

void taskmanager_remove_timer(TASK_TIME *handler)
{
	struct event *evt = handler->evt;
	event_del(evt);
	event_free(evt);
}

/*
void taskmanager_add_task(TASK *task, task_callback *callback, void *handler)
{
	TASK_QUEUE *queue = task->queue;
	if (queue == NULL){
		task->queue = malloc(sizeof(TASK_QUEUE));
		task->queue->callback = callback;
		task->queue->handler = handler;
		task->queue->next = NULL;
		return;
	}

	while (queue != NULL)
	{
		if (queue->next == NULL){
			queue->next = malloc(sizeof(TASK_QUEUE));
			queue->next->callback = callback;
			queue->next->handler = handler;
			queue->next->next = NULL;
			break;
		}
		queue = queue->next;
	}
}

void taskmanager_remove_task(TASK *task, void *handler)
{
	TASK_QUEUE *queue = task->queue;

	if (queue->handler == handler){
		if (queue->next == NULL){
			free(task->queue);
			task->queue = NULL;
		}
		else{
			TASK_QUEUE *next = queue->next;
			free(queue);
			task->queue = next;
		}
		return;
	}

	while (queue->next!=NULL)
	{
		if (queue->next->handler == handler){
			if (queue->next->next != NULL){
				TASK_QUEUE *next = queue->next->next;
				free(queue->next);
				queue->next = next;
			}
			else{
				free(queue->next);
				queue->next = NULL;
			}
			break;
		}
		queue = queue->next;
	}
}

*/

void taskmanager_run(TASK *task)
{
	event_base_loop(task->evbase, 0);
}

void taskmanager_close(TASK *task)
{
     event_base_loopbreak(task->evbase);
}