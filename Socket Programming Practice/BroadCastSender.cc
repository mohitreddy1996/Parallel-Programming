#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
    int socket;
    struct sockaddr_in broadcastAddr;
    char *broadcastIP;
    unsigned short broadcastPort;
    char *sendString;
    int broadcastPermission;
    unsigned int sendStringLen;

    // argument check;

    broadcastIP = argv[1];
    broadcastPort = atoi(argv[2]);
    sendString = atoi(argv[3]);

    // create socket for sending and receiving datagrams.
    if((socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("socket() failed");
        exit(1);
    }

    broadcastPermission = 1;
    if(setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0){
        perror("setsockopt() failed");
        exit(1);
    }

    memset(&broadcastAddr, 0, sizeof(broadcastAddr));

    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(broadcastPort);
    broadcastAddr.sin_addr.s_addr = htonl(broadcastAddr);

    sendStringLen = strlen(sendString);
    // send broadcast every 5 seconds;
    for(;;){
        if(sendto(socket, sendString, sendStringLen, 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) != sendStringLen){
            perror("sendto() failed");
            exit(1);
        }
        sleep(5);
    }

}