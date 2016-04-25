#include "client.h"
#include <stdlib.h>
#include <string.h>

static clients *client_list = NULL;

clients *client_create()
{
	clients *client = client_list;
	if (client == NULL){
		client = calloc(1, sizeof(clients));
		if (client == NULL){
			printf("malloc failed");
			return NULL;
		}

		client_list = client;
	}
	else{
		while (client->next != NULL){
			client = client->next;
		}

		client = client->next = malloc(sizeof(clients));
	}

	memset(client, 0, sizeof(clients));

	client->next = NULL;
	return client;
}

void client_remove(clients *client)
{
	clients *cl = client_list;
	if (cl == client){
		if(client->prve_packet){
	       packet_free_serial(client->prve_packet);
		   free(client->prve_packet);
	    }
		
		if (cl->next != NULL){
			client_list = cl->next;
		}
		else
		{
			client_list = NULL;
		}
		free(client);
		return;
	}

	while (cl->next != client){
		cl = cl->next;
	}

	if (client->next != NULL){
		cl->next = client->next;
	}
	else{
		cl->next = NULL;
	}
	
    if(client->prve_packet){
	    packet_free_serial(client->prve_packet);
		free(client->prve_packet);
	}
	free(client);
}

clients *client_first()
{
	return client_list;
}
