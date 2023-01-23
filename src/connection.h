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

#define MAXBUFLEN 64
#define MYPORT "4750"

typedef enum conn_type {
	CONN_TYPE_SERVER_E = 0,
	CONN_TYPE_CLIENT_E,
	/* LAST */
	NUM_CONN_TYPES_E
} conn_type_e;

typedef struct connection_parameters {
    conn_type_e type = NUM_CONN_TYPES_E;
    char port[64];
    char node[64];
} conn_param_s;


class Connection
{
private:

	int m_sockfd = 0;
	conn_param_s m_params;
	std::thread mt_recv;
	std::thread mt_send;
	std::thread mt_mngr;
	char m_buf[MAXBUFLEN];

    /* Should be 3 threads in the Connection. Send Thread. Rec Thread. Manager Thread. */
	#if 0
	
	struct addrinfo hints, *servinfo, *p;
	void *get_in_addr(struct sockaddr *sa);
	status_e initiate_connection(int &sockfd, struct addrinfo &ret, const char *node);
	status_e open_connection(int &sockfd);

	int numbytes;
	struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET_ADDRSTRLEN];
	
	#endif

	void start_server();
	void start_client();
	status_e conneciton_handler(int newfd, struct sockaddr_storage their_addr);
	void* get_in_addr(struct sockaddr *sa);
	status_e open_connection(int &sockfd, struct addrinfo &ret, const conn_param_s &params);
	status_e listen_for_connection(int &sockfd, const conn_param_s &params);

public:

	Connection(const conn_param_s &params);
	Connection(const Connection &con);
	~Connection();
	
	status_e startconnection();

/*
	status_e listen();
	status_e accept();
	status_e send();
	status_e recv();
	status_e connect();
*/

};

#endif /* CONNECTION_H */

/*
	New connection Server or Client
	if Server:
		get socket
		listen for connections on port
			If connection:
				launch connection handler in child process
				go back to listening on port
	if Client
		get socket
		open connection to ip:port

*/