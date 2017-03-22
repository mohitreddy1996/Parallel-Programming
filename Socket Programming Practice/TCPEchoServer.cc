#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXPENDING 5


int main(int argc, char *argv[]){
    int servSock;
    int clientSock; // socket descriptor for client.
    struct sockaddr_in echoServAddr;
    struct sockaddr_in echoClientAddr;
    unsigned short echoServerPort;
    unsigned int clientLen;

    if(argc != 2){
        exit(1);
    }

    echoServerPort = atoi(argv[1]);

    if((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("Error while creating a socket.");
    }

    memset(&echoServAddr, 0, sizeof(echoServAddr));

    echoServAddr.sin_family = AF_INET; // Internet address family.
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Internet address family
    echoServAddr.sin_port = htons(echoServerPort); // local port.

    // bind to local address
    if(bind(servSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)))
        perror("Bind() Failed");

    // mark the socket to listen for incoming connections.
    if(listen(servSock, MAXPENDING) < 0)
        perror("listen() Failed");

    for(;;)
    {
        // set the size of in-out parameter.
        clientLen = sizeof(echoClientAddr);
        if((clientSock = accept(servSock, (struct sockaddr *)&echoClientAddr, &clientLen)) < 0)
            perror("Accept() failed");
        // client socket is connected to a client.
        printf("Handling client %s\n", inet_ntoa(echoClientAddr.sin_addr));
    }
    return 0;
}