#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAXRECVSTRING 255

int main(int argc, char *argv[]){
    int socket;
    struct sockaddr_in multicastAddr;
    char *multicastIP;
    unsigned short multicastPort;
    char recvString[MAXRECVSTRING+1];
    int recvStringLen;
    struct ip_mreq multicastRequest; // multicast address join structure.

    multicastIP = argv[1];
    multicastPort = atoi(argv[2]);

    if((socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("socket() failed");
    }
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_port = htons(multicastPort);
    multicastAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind it.
    if(bind(socket, (struct sockaddr *)&multicastAddr, sizeof(multicastAddr)) < 0){
        perror("bind() failed");
        exit(1);
    }

    // specify the multicast group.
    multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastIP);
    // accept multicast from any interface.
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    // join the multicast address.
    if(setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&multicastRequest, sizeof(multicastRequest)) < 0){
        perror("setsockopt() failed");
        exit(1);
    }

    // receive
    if((recvStringLen = recvfrom(socket, recvString, MAXRECVSTRING, 0, NULL, 0)) < 0){
        perror("recvfrom() failed");
        exit(1);
    }

    recvString[recvStringLen] = '\0';

    printf("Received %s", recvString);

    close(socket);
    return 0;
}