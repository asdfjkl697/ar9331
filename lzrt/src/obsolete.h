#ifndef _OBSOLETE_H
#define _OBSOLETE_H

#include "typedef.h"

 /*Obsolete function definition */
void obsolete_connection_read(struct bufferevent *bev, void *arg);

void obsolete_send_packet(clients *client, serial_packet *packet);

#endif
