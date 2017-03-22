#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    int socket;
    struct sockaddr_in multicastAddr;
    char *multicastIP;
    unsigned short multicastPort;
    char *sendString;
    unsigned char multicastTTL; // TTL of multicast packets.
    unsigned int sendStringLen;

    multicastIP = argv[1];
    multicastPort = atoi(argv[2]);
    sendString = argv[3];
    multicastTTL = argv[4];
    if((socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("socket() failed");
        exit(1);
    }

    if(setsockopt(socket, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&multicastTTL, sizeof(multicastTTL)) < 0){
        perror("setsockopt() failed");
        exit(1);
    }

    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_port = htons(multicastPort);
    multicastAddr.sin_addr.s_addr = inet_addr(multicastIP);

    sendStringLen = strlen(sendString);

    for(;;){
        if(sendto(socket, sendString, sendStringLen, 0, (struct sockaddr *) &multicastAddr, sizeof(multicastAddr)) != sendStringLen){
            perror("sendto() failed. different sizes sent.");
            exit(1);
        }
        sleep(5);
    }

    return 0;
}