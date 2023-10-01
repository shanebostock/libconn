#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "status.h"


#define MYPORT "4950"	// the port I am listening on
#define SERVERPORT "4950" // the port I am talking to
#define SERVER "127.0.0.1" // the server I am talking to

#define MAXBUFLEN 100

/* helperfuncs.c */
status_e linker_test_func(int32_t num);
void *get_in_addr(struct sockaddr *sa);
void get_msg(char* pbuf);

/* listener.c */
int32_t listener();

/* talker.c */
int32_t talker(char* msg);

#endif /* CONNECTION_H */