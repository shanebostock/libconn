#include "connection.h"


status_e linker_test_func(int32_t num){
    printf("I am a test function for the library. Number sent: %d\n", num);
    return STATUS_OK_E;

}

void get_msg(char* pbuf){

    printf("Message to Send: ");
    fgets (pbuf, MAXBUFLEN, stdin);

}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


