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
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define MAXBUFLEN 1024
#define PORT "4750"
//#define IP "10.0.0.181"
#define IP "127.0.0.1"

typedef enum conn_type {
	CONN_TYPE_RECEIVER_E = 0,
	CONN_TYPE_SENDER_E,
	/* LAST */
	NUM_CONN_TYPES_E
} conn_type_e;

typedef struct connection_parameters {
    conn_type_e type = NUM_CONN_TYPES_E;
    char port[64] = {'\0'}; // port to connect to
    char node[64] = {'\0'}; // ipv4 to connect to
} conn_param_s;



class Connection
{
private:

	int m_sockfd = 0;
	conn_param_s m_params;
	char m_buf[MAXBUFLEN];
	struct addrinfo *m_servinfo, *m_p;

    /* Should be 3 threads in the Connection. Send Thread. Rec Thread. Manager Thread. */

	void start_server();
	void start_client();
	void* get_in_addr(struct sockaddr *sa);
	// void get_iface_addr(void);
	void receiver();
	void m_sender();
	
public:

	Connection(const conn_param_s &params);
	Connection(const Connection &con);
	~Connection();
	
	status_e startconnection();
	void sender();
	void sender(char* msgbuf);

};

#endif /* CONNECTION_H */

