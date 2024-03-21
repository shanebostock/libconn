#include "connection.h"

int32_t default_talker(char* msg){
    
    const char* serverport = DEFAULT_SERVERPORT;
    const char* hostname = DEFAULT_SERVER;
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
    do{
    	get_msg(msg);
	    if ((numbytes = sendto(sockfd, msg, strlen(msg), 0,
	             p->ai_addr, p->ai_addrlen)) == -1) {
	        perror("talker: sendto");
	        exit(1);
	    }
	    printf("talker: sent %d bytes to %s\n", numbytes, hostname);
	} while(1);

    freeaddrinfo(servinfo);

    
    close(sockfd);
    return 0;


}

int32_t talker(conn_params_s *params){
    
    const char* serverport = params->serverport;
    const char* hostname = DEFAULT_SERVER;
    char buf[MAXBUFLEN];
	char* pbuf = buf;
	//get_msg(p_buf);

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
    do{
    	get_msg(pbuf);
	    if ((numbytes = sendto(sockfd, pbuf, strlen(pbuf), 0,
	             p->ai_addr, p->ai_addrlen)) == -1) {
	        perror("talker: sendto");
	        exit(1);
	    }
	    printf("talker: sent %d bytes to %s\n", numbytes, hostname);
	    sleep(1);
	} while(1);

    freeaddrinfo(servinfo);

    
    close(sockfd);
    return 0;


}