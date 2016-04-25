#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_

#include "typedef.h"
#include <stdint.h>


void taskmanager_init(TASK *task);
void taskmanager_free(TASK *task);
void taskmanager_add_socket(TASK *task, int socket_fd, task_socket_callback *callback);
void taskmanager_remove_socket(TASK *task);
void taskmanager_add_timer(TASK *task, TASK_TIME *handler, uint32_t millisecond);
void taskmanager_remove_timer(TASK_TIME *handler);
//void taskmanager_add_task(TASK *task, task_callback *callback, void *handler);
//void taskmanager_remove_task(TASK *task, void *handler);
void taskmanager_run(TASK *task);
void taskmanager_close(TASK *task);

#endif