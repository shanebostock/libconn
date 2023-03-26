#include "connection.h"

/* Public */
Connection::Connection(const conn_param_s &params){
	
    m_params = params;
    if(m_params.port[0] == '\0'){
        // printf("Port is null using default value %d\n", atoi(PORT));
        long unsigned int ret = snprintf(m_params.port,sizeof(m_params.port),"%s",PORT);
        if(ret > sizeof(m_params.my_port)){
            printf("Truncated\n");
        }
    } 
    if(m_params.node[0] == '\0'){
        // do nothing for now
        // printf("Node is null getting My Local IP\n");
        // set_my_node();
    }

    startconnection();

}

Connection::Connection(const Connection &con){
    
    this->m_params = con.m_params;
    this->m_sockfd = con.m_sockfd;
}

Connection::~Connection(){
    
    close(m_sockfd);
    freeaddrinfo(m_servinfo);

}

status_e Connection::startconnection(){

    if (m_params.type == CONN_TYPE_RECEIVER_E){
        printf("I am a recevier.\n");
        printf("Listening on port: %s\n", m_params.port);
        // printf("My IP is: %s\n", m_params.node);
        start_server();
    }
    else if (m_params.type == CONN_TYPE_SENDER_E){
        printf("I am a sender.\n"); 
        printf("Connecting to port: %s.\n", m_params.port);
        printf("At IP: %s\n", m_params.node);
        start_client();
    }
    else {
        return STATUS_FAIL_E;
    }

    return STATUS_OK_E;
}


/* Private */
// Core Business Function Method
/*

The difference between a sever and client is their starting state: waiting for a message or sending a message.
If you are waiting you are a sender. 
If you are sending you are a client.  

*/


void Connection::start_server(){

    //int numbytes;
    struct addrinfo hints;

    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //Options are AF_INET (ipv4) AF_INET6 (ipv6) or AF_UNSPEC (either ipv4 or ipv6)
    hints.ai_socktype = SOCK_DGRAM; // Options are SOCK_DGRAM or SOCK_STREAM
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, m_params.port, &hints, &m_servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(0);
    }

    // loop through all the results and bind to the first we can
    for(m_p = m_servinfo; m_p != NULL; m_p = m_p->ai_next) {
        if ((m_sockfd = socket(m_p->ai_family, m_p->ai_socktype,
                m_p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (bind(m_sockfd, m_p->ai_addr, m_p->ai_addrlen) == -1) {
            close(m_sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(m_servinfo); // all done with this structure

    if (m_p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    receiver();

}

void Connection::start_client(){

    //int numbytes;  
    struct addrinfo hints;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(m_params.node, m_params.port, &hints, &m_servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(0);
    }

    // loop through all the results and connect to the first we can
    for(m_p = m_servinfo; m_p != NULL; m_p = m_p->ai_next) {
        if ((m_sockfd = socket(m_p->ai_family, m_p->ai_socktype,
                m_p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(m_sockfd, m_p->ai_addr, m_p->ai_addrlen) == -1) {
            close(m_sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (m_p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(0);
    }

    inet_ntop(m_p->ai_family, get_in_addr((struct sockaddr *)m_p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);
    
}

void Connection::receiver(){
    int numbytes;
    socklen_t addr_len;
    char s[INET_ADDRSTRLEN];
    struct sockaddr_storage their_addr;

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        addr_len = sizeof their_addr;
        if((numbytes = recvfrom(m_sockfd, m_buf, MAXBUFLEN-1, 0 , (struct sockaddr*)&their_addr, &addr_len)) == -1){
            perror("recvfrom");
            exit(1);
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got packet from: %s\n", s);
        m_buf[numbytes] = '\0'; // set last index to null char.
        printf("server: packet contains: %s",m_buf);

        //
        break;
        //

    }
    //close(m_sockfd);
}

void Connection::sender(){

    int numbytes;
    printf("Message to Send: ");
    fgets (m_buf, MAXBUFLEN, stdin);

    if ((numbytes = sendto(m_sockfd, m_buf, MAXBUFLEN-1, 0, m_p->ai_addr, m_p->ai_addrlen)) == -1) {
        perror("client: sendto");
        exit(1);
    }
}

void Connection::sender(char* msgbuf){

    int numbytes;
    if ((numbytes = sendto(m_sockfd, msgbuf, MAXBUFLEN-1, 0, m_p->ai_addr, m_p->ai_addrlen)) == -1) {
        perror("client: sendto");
        exit(1);
    }
}

// Helper methods
void* Connection::get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
