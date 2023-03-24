#ifndef CONNECTION_H
#define CONNECTION_H

#include "status.h"
#include "stdint.h"

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
#include <thread>
#include <ifaddrs.h>

#define MAXBUFLEN 1024
#define MYPORT "4750"
#define BACKLOG 10

typedef enum conn_type {
	CONN_TYPE_SERVER_E = 0,
	CONN_TYPE_CLIENT_E,
	/* LAST */
	NUM_CONN_TYPES_E
} conn_type_e;

typedef struct connection_parameters {
    conn_type_e type = NUM_CONN_TYPES_E;
    char port[64] = {'\0'}; // port to connect to
    char node[64] = {'\0'}; // ipv4 to connect to
    char my_port[64] = {'\0'}; // my port to listen on
    char my_node[64] = {'\0'}; // my ipv4 to listen on
} conn_param_s;


class Connection
{
private:

	int m_sockfd = 0;
	conn_param_s m_params;
	std::thread mt_listener;
	std::thread mt_talker;
	std::thread mt_mngr;
	char m_buf[MAXBUFLEN];
	struct addrinfo *m_servinfo, *m_p;

    /* Should be 3 threads in the Connection. Send Thread. Rec Thread. Manager Thread. */

	void start_server();
	void start_client();
	void* get_in_addr(struct sockaddr *sa);
	void receiver();
	void sender();
	
public:

	Connection(const conn_param_s &params);
	Connection(const Connection &con);
	~Connection();
	
	status_e startconnection();

};

#endif /* CONNECTION_H */

