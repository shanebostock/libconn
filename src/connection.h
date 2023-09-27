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

#define MAXBUFLEN 100

status_e linker_test_func(int32_t num);
void *get_in_addr(struct sockaddr *sa);
int32_t listener();
int32_t talker(int argc, char* argv[]);

#endif /* CONNECTION_H */