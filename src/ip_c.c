#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#if 1

void other_test(void)
{
    char          buf[1024];
    struct ifconf ifc;
    struct ifreq *ifr;
    int           sck;
    int           nInterfaces;
    int           i;

/* Get a socket handle. */
    sck = socket(AF_INET, SOCK_DGRAM, 0);
    if(sck < 0)
    {
        perror("socket");
        exit(1);
    }

/* Query available interfaces. */
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
    {
        perror("ioctl(SIOCGIFCONF)");
        exit(1);
    }

/* Iterate through the list of interfaces. */
    ifr         = ifc.ifc_req;
    nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
    for(i = 0; i < nInterfaces; i++)
    {
        struct ifreq *item = &ifr[i];

    /* Show the device name and IP address */
        printf("%s: IP %s",
               item->ifr_name,
               inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));


    /* Get the broadcast address */
        if(ioctl(sck, SIOCGIFBRDADDR, item) >= 0)
            printf(", BROADCAST %s", inet_ntoa(((struct sockaddr_in *)&item->ifr_broadaddr)->sin_addr));
        printf("\n");
    }

}
#endif

int main(void)
{
 other_test();
 return 0;
}