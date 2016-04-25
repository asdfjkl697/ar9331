#ifndef _CLIENT_H
#define _CLIENT_H

#include "typedef.h"

clients *client_create();
void client_remove(clients *client);
clients *client_first();

#endif
