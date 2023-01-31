#include "connection.h"


/* Public */
Connection::Connection(const conn_param_s &params){
	
    m_params = params;
    if(m_params.port[0] == '\0'){
        printf("Port is null using default value %d\n", atoi(MYPORT));
        long unsigned int ret = snprintf(m_params.port,sizeof(m_params.port),"%s",MYPORT);
        if(ret > sizeof(m_params.port)){
            printf("Truncated\n");
        }
    }
    if(m_params.node[0] == '\0'){
        printf("Node is null getting Server IP\n");
        get_ip_addr();
    }

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
        printf("Listening on port: %s\n", m_params.port);
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
            exit(0);
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

void Connection::get_ip_addr(){


    FILE *f;
    char line[100] , *p , *c;
    
    f = fopen("/proc/net/route" , "r");
    
    while(fgets(line , 100 , f))
    {
        p = strtok(line , " \t");
        c = strtok(NULL , " \t");
        
        if(p!=NULL && c!=NULL)
        {
            if(strcmp(c , "00000000") == 0)
            {
                //printf("Default interface is : %s \n" , p);
                break;
            }
        }
    }
    
    //which family do we require , AF_INET or AF_INET6
    int fm = AF_INET;
    struct ifaddrs *ifaddr, *ifa;
    int family , s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    //Walk through linked list, maintaining head pointer so we can free list later
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL)
        {
            continue;
        }

        family = ifa->ifa_addr->sa_family;

        if(strcmp( ifa->ifa_name , p) == 0)
        {
            if (family == fm) 
            {
                s = getnameinfo( ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6) , host , NI_MAXHOST , NULL , 0 , NI_NUMERICHOST);
                
                if (s != 0) 
                {
                    printf("getnameinfo() failed: %s\n", gai_strerror(s));
                    exit(EXIT_FAILURE);
                }
                
                //printf("address: %s\n", host);
                long unsigned int ret = snprintf(m_params.node,sizeof(m_params.node),"%s",host);
                if ( ret > sizeof(m_params.node)){
                    printf("Truncated snprintf()");
                    exit(0);
                }
            }
            //printf("\n");
        }
    }

    freeifaddrs(ifaddr);
    
    //return 0;

/*    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&inet_addr;
    struct in_addr ipAddr = pV4Addr->sin_addr;

    //If you then want the ip address as a string then do the following:

    
    inet_ntop( AF_INET, &ipAddr, m_params.node, INET_ADDRSTRLEN );
    printf("IP Address is %s\n" ,m_params.node);
*/
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