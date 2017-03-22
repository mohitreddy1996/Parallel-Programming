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
    struct sockaddr_in fromAddr; // source address of echo.
    unsigned short echoServPort;
    unsigned  int fromSize;

    char *serverIP; // IP address of server.
    char *echoString; // string which has to be sent to the server.
    char echoBuffer[ECHOMAX+1];
    int echoStringLen;
    int respStringLen; // length of the received response;

    // argument list check

    serverIP = argv[1];
    echoString = argv[2];
    if((echoStringLen = strlen(echoString)) > ECHOMAX){
        perror("Echo string too long");
        exit(1);
    }

    echoServPort = atoi(argv[3]);

    // create a socket.
    if((socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("socket() failed");
        exit(1);
    }

    memset(&echoServerAddr, 0, sizeof(echoServerAddr));
    echoServerAddr.sin_family = AF_INET;
    echoServerAddr.sin_port = htons(echoServPort);
    echoServerAddr.sin_addr.s_addr = inet_addr(serverIP);

    // send it to server;
    if(sendto(socket, echoString, echoStringLen, 0, (struct sockaddr *) &echoServerAddr, sizeof(echoServerAddr)) != echoStringLen){
        perror("sendto didnt work");
        exit(1);
    }

    // receive the echo response.
    fromSize = sizeof(fromAddr);
    if((respStringLen = recvfrom(socket, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &fromAddr, &fromSize)) != echoStringLen){
        perror("recvfrom() failed");
        exit(1);
    }

    if(echoServerAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr){
        perror("received packet is the packet sent out/");
        exit(1);
    }

    echoBuffer[respStringLen] = '\0';
    printf("%s", echoBuffer);

    close(socket);
    return 0;

}