#include "connection.h"


/* Public */
Connection::Connection(const conn_param_s &params){
	
    m_params = params;


}

Connection::Connection(const Connection &con){
    
    this->m_params = con.m_params;
    this->m_sockfd = con.m_sockfd;
}

Connection::~Connection(){}

status_e Connection::startconnection(){
/*  
    the difference between client and server is the starting state. 
    the server server starts by listening for connections.
    the client starts by initializing connections.
*/
    if (m_params.type == CONN_TYPE_SERVER_E){
        printf("I am a server.\n");
        printf("Listening on port: %s.\n", m_params.port);
        printf("My IP is: %s\n", m_params.node);
        start_server();
    }
    else if (m_params.type == CONN_TYPE_CLIENT_E){
        printf("I am a client.\n"); 
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
void Connection::start_server(){

    int sockfd, newfd;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    char s[INET_ADDRSTRLEN];

    printf("Starting server...\n");
    listen_for_connection(sockfd, m_params);
    printf("waiting to recvfrom...\n");

    sin_size = sizeof their_addr;
    while(1){

        newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if(newfd == -1){
            perror("accept");
            continue;
        }
        
        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("got connection from %s\n", s);

        //FORK
        if(!fork()){
            //start child code
            close(sockfd);
            if(conneciton_handler(newfd, their_addr) == STATUS_FAIL_E){
                perror("connection_handler");
            }
            //end child code
        }
        //parent code continued
        close(newfd);
        

    }
    printf("Terminating Program.\n");
    close(sockfd);

}

status_e Connection::conneciton_handler(int newfd, struct sockaddr_storage their_addr){
    char buf[MAXBUFLEN];
    int numbytes;
    socklen_t sin_size;
    sin_size = sizeof their_addr;
    char s[INET_ADDRSTRLEN];

    while(1){
        if ((numbytes = recvfrom(newfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("recvfrom");
            return STATUS_FAIL_E;
        }
        
        buf[numbytes-1] = '\0'; // sendto() adds '\n' replace with '\0'
        printf("recvd packet from %s\n",
        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));
        printf("recvd packet is %d bytes long\n", numbytes);
        printf("recvd packet contains %s\n", buf);
        if (strcmp("End", buf) == 0 || strcmp("end", buf) == 0){
            break;
        }
    }

    memset(&buf, 0, MAXBUFLEN);
    close(newfd);
    return STATUS_OK_E;
}

void Connection::start_client(){

    int sockfd;
    struct addrinfo ret;
    int numbytes;
    char buf[MAXBUFLEN];
    // char node[MAXBUFLEN];

    printf("Start of Application\n");
    open_connection(sockfd, ret, m_params);
    while(1){
        printf("Message to Send: ");
        fgets (buf, MAXBUFLEN, stdin);
        
        if ((numbytes = sendto(sockfd, buf, strlen(buf), 0,
                ret.ai_addr, ret.ai_addrlen)) == -1) {
            perror("host: sendto");
            exit(1);
        }
        buf[numbytes-1] = '\0'; // sendto() adds '\n' replace with '\0'
        if (strcmp("End", buf) == 0 || strcmp("end", buf) == 0){
            break;
        }
        memset(&buf, 0, MAXBUFLEN);
    }
    
    printf("host: Terminating Program.\n");
    close(sockfd);

}

void* Connection::get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);

}

status_e Connection::open_connection(int &sockfd, struct addrinfo &ret, const conn_param_s &params){
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(params.node, params.port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return STATUS_FAIL_E;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "failed to bind socket\n");
        return STATUS_FAIL_E;
    }
    memcpy(&ret,p,sizeof(addrinfo));
    freeaddrinfo(servinfo);

    return STATUS_OK_E;    

}

status_e Connection::listen_for_connection(int &sockfd, const conn_param_s &params){
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int yes = 1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    
    if ((rv = getaddrinfo(NULL, params.port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return STATUS_FAIL_E;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        /*
         &yes is a value >0 denoting 'yes' to the setoptions variable. It requires *int
         hence the definition of "int yes = 1".

         see man(2) setsockopt
         */
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "failed to bind socket\n");
        return STATUS_FAIL_E;
    }
    freeaddrinfo(servinfo);

    return STATUS_OK_E;

}

#if 0

status_e Connection::waiting_for_connection(int &sockfd){
    printf("fixed_station: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    while(1){
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
            (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(1);
        }
        buf[numbytes-1] = '\0'; // sendto() adds '\n' replace with '\0'
        printf("fixed_station: got packet from %s\n",
            inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *)&their_addr),
                s, sizeof s));
        printf("fixed_station: packet is %d bytes long\n", numbytes);
        printf("fixed_station: packet contains %s\n", buf);
        if (strcmp("End", buf) == 0 || strcmp("end", buf) == 0){
            break;
        }
        memset(&buf, 0, MAXBUFLEN);
    }
    printf("fixed_station: Terminating Program.\n");
    close(sockfd);

}

status_e Connection::start_listening(int &sockfd){
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return STATUS_FAIL_E;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("fixed_station: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("fixed_station: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "fixed_station: failed to bind socket\n");
        return STATUS_FAIL_E;
    }
    freeaddrinfo(servinfo);

    return STATUS_OK_E;

}

#endif