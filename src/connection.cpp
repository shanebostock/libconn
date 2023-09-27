#include "connection.h"

/* Public */
Connection::Connection(){}

Connection::~Connection(){
    
    if(m_sockfd != 0){
        close(m_sockfd);
        freeaddrinfo(m_servinfo);
    }


}

status_e Connection::startconnection(const conn_param_s &params){

    if (params.type == CONN_TYPE_SERVER_E){
        m_lparams = params;

        printf("I am a server.\n");
        printf("Listening on port: %s\n", m_lparams.port);
        printf("My IP is: %s\n", m_lparams.node);
        
        start_server();
        t_listen(listen);
        return STATUS_OK_E;
    }
    else if (params.type == CONN_TYPE_CLIENT_E){
        m_tparams = params;
        printf("I am a client.\n"); 
        printf("Connecting to port: %s.\n", m_tparams.port);
        printf("At IP: %s\n", m_tparams.node);
        return STATUS_OK_E;
        start_client();
    }
    else {
        return STATUS_FAIL_E;
    }

    return STATUS_OK_E;
}

void Connection::start_server(){

    //int numbytes;
    struct addrinfo hints;

    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //Options are AF_INET (ipv4) AF_INET6 (ipv6) or AF_UNSPEC (either ipv4 or ipv6)
    hints.ai_socktype = SOCK_DGRAM; // Options are SOCK_DGRAM or SOCK_STREAM
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, m_lparams.port, &hints, &m_servinfo)) != 0) {
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

    //listen();

}

void Connection::start_client(){

    //int numbytes;  
    struct addrinfo hints;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(m_tparams.node, m_tparams.port, &hints, &m_servinfo)) != 0) {
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

void Connection::listen(){
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

void Connection::talk(){

    int numbytes;
    printf("Message to Send: ");
    fgets (m_buf, MAXBUFLEN, stdin);

    if ((numbytes = sendto(m_sockfd, m_buf, MAXBUFLEN-1, 0, m_p->ai_addr, m_p->ai_addrlen)) == -1) {
        perror("client: sendto");
        exit(1);
    }
}

// void Connection::sender(char* msgbuf){

//     int numbytes;
//     if ((numbytes = sendto(m_sockfd, msgbuf, MAXBUFLEN-1, 0, m_p->ai_addr, m_p->ai_addrlen)) == -1) {
//         perror("client: sendto");
//         exit(1);
//     }
// }

// Helper methods
void* Connection::get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/*
void Connection::get_iface_addr(void){
    char          buf[1024];
    struct ifconf ifc;
    struct ifreq *ifr;
    int           sck;
    int           nInterfaces;
    int           i;


    sck = socket(AF_INET, SOCK_DGRAM, 0);
    if(sck < 0)
    {
        perror("socket");
        exit(1);
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
    {
        perror("ioctl(SIOCGIFCONF)");
        exit(1);
    }

    ifr         = ifc.ifc_req;
    nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
    for(i = 0; i < nInterfaces; i++)
    {
        struct ifreq *item = &ifr[i];

        printf("%s: IP %s",
               item->ifr_name,
               inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));
    }

}

*/