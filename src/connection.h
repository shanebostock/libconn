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


#define DEFAULT_MYPORT "4950"	// the port I am listening on
#define DEFAULT_SERVERPORT "4950" // the port I am talking to
#define DEFAULT_SERVER "127.0.0.1" // the server I am talking to

#define MAXBUFLEN 100

/* helperstruct */
typedef struct connection_parameters {
  char myport[64];
  char serverport[64];

} conn_params_s;

/* helperfuncs.c */
status_e linker_test_func(int32_t num);
void *get_in_addr(struct sockaddr *sa);
void get_msg(char* pbuf);

/* listener.c */
int32_t default_listener();
int32_t listener(conn_params_s *params);

/* talker.c */
int32_t default_talker(char* msg);
int32_t talker(conn_params_s *params);

#endif /* CONNECTION_H */