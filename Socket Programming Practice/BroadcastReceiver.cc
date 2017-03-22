#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXRECVSTRING 255

int main(int argc, char *argv[]){
    int socket;
    struct sockaddr_in broadcastAddr;
    unsigned short broadcastPort;
    char recvString[MAXRECVSTRING+1];
    int recvStringLen;

    broadcastPort = atoi(argv[1]);

    // create a socket;
    if((socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("socket() failed");
        exit(1);
    }

    // broadcast Addr.
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(broadcastPort);
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socket, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) < 0){
        perror("bind() failed");
        exit(1);
    }

    if((recvStringLen = recvfrom(socket, recvString, MAXRECVSTRING, 0, NULL, 0)) < 0){
        perror("recvfrom() failed");
        exit(1);
    }

    recvString[recvStringLen] = '\0';
    printf("Received : %s", recvString);

    close(socket);
    return 0;
}