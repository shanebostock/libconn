#include "connection.h"

int32_t talker(char* msg){
    
    char* serverport = SERVERPORT;
    char* hostname = SERVER;
    // char* msg = (char*)_msg;


    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    // if (argc != 3) {
    //     fprintf(stderr,"usage: talker hostname message\n");
    //     exit(1);
    // }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // set to AF_INET to use IPv4
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(hostname, serverport, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }

    if ((numbytes = sendto(sockfd, msg, strlen(msg), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);

    printf("talker: sent %d bytes to %s\n", numbytes, hostname);
    close(sockfd);
    return 0;


}