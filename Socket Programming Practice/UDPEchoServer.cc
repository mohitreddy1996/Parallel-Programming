#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ECHOMAX 255

int main(int argc, char *argv[]){
    int socket;
    struct sockaddr_in echoServerAddr;
    struct sockaddr_in echoClientAddr;
    unsigned int clientAddrLen;
    char echoBuffer[ECHOMAX];
    unsigned short echoServerPort;
    int recvMsgeSize;

    if(argc != 2){
        exit(1);
    }

    echoServerPort = atoi(argv[1]);

    // create socket for sending and receiving.
    if((socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("socket() failed");
        exit(1);
    }

    // make the server struct attributes to 0.
    memset(&echoServerAddr, 0, sizeof(echoServerAddr));

    echoServerAddr.sin_family = AF_INET;
    echoServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServerAddr.sin_port = htons(echoServerPort);

    // bind to the local address.
    if(bind(socket, (struct sockaddr *) &echoServerAddr, sizeof(echoServerAddr)) < 0){
        perror("bind() failed");
        exit(1);
    }

    for(;;){
        clientAddrLen = sizeof(echoClientAddr);

        if((recvMsgeSize = recvfrom(socket, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &echoClientAddr, &clientAddrLen)) < 0){
            perror("recvfrom() failed");
            exit(1);
        }

        printf("Handling Client %s\n", inet_ntoa(echoClientAddr.sin_addr));

        // send received datagram back to the client.
        if(sendto(socket, echoBuffer, recvMsgeSize, 0, (struct sockaddr *) &echoClientAddr, sizeof(echoClientAddr)) != recvMsgeSize){
            perror("sendto() failed");
            exit(1);
        }
    }

    return 0;
}