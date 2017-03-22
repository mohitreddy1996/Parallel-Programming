#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define RECVBUFSIZE 32

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in echoServerAddr;
    unsigned short echoServerPort;
    char *serverIP;
    char *echoString;
    char echoBuffer[RECVBUFSIZE];
    unsigned int echoStringLen;
    int bytesRcvd, totalBytesRcvd;

    if((argc < 3) || (argc > 4)){
        perror("Arguments less.");
        exit(1);
    }

    serverIP = argv[1];
    echoString = argv[2];

    if(argc == 4)
        echoServerPort = atoi(argv[3]);
    else
        echoServerPort = 7;

    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        perror("Socket Creation error");

    memset(&echoServerAddr, 0, sizeof(echoServerAddr));

    echoServerAddr.sin_family = AF_INET;
    echoServerAddr.sin_addr.s_addr = inet_addr(serverIP);
    echoServerAddr.sin_port = htons(echoServerPort);

    // establish a connection with the server.
    if(connect(sock, (struct sockaddr *) &echoServerAddr, sizeof(echoServerAddr)) < 0)
        perror("Connect() failed");

    echoStringLen = strlen(echoString);

    // send the string to server.
    if(send(sock, echoString, echoStringLen, 0) != echoStringLen)
        perror("Send sent different number of bytes than expected.");

    totalBytesRcvd = 0;
    while (totalBytesRcvd < echoStringLen){
        if((bytesRcvd = recv(sock, echoBuffer, RECVBUFSIZE-1, 0)) <= 0)
            perror("Recv() failed. close permanently.");
        totalBytesRcvd += bytesRcvd;
        echoBuffer[bytesRcvd] = '\0';
        cout<<echoBuffer<<endl;
    }

    close(sock);
    return 0;
}