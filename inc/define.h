

#ifndef MAIN_H
#define MAIN_H

/* TODO: Change the file name as suggested in server file. */

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* TODO: Update with file name. */
#define MAX_CLIENTS 5
#define BUFF_SIZE 128
#define CLIENT_LIST_SIZE 15
#define RCV_MSG_SIZE 30
/* TODO: What is backlog? The name itself explain the functionality or usecase. */
#define BACKLOG 2


#include "server.h"
#include "client.h"


#endif 
