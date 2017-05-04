//
// Created by mohit on 4/5/17.
//


/**
 * Pipe used for interprocess communication.
 * Kernel sets up two file descriptors for use by the pipe.
 * One descriptor is used to allow a path of input into the pipe (write).
 * Other is used to obtain data from the pipe (read).
 *
 * ~ close the unnecessary file descriptors. (close the end of the pipe that they are not concerned with).
 * ~ we use write() and read() system call.
 * */
#include <cstdlib>
#include <cstring>
#include "basic1.h"

int main (void)
{
    int fd[2], nbytes;
    pid_t childpid;
    char strings[] = "Hello, world!\n";
    char readBuffer[80];

    pipe(fd);

    if ((childpid = fork()) == -1)
    {
        perror ("fork");
        exit(1);
    }

    if (childpid == 0)
    {
        /**
         * Child Process closes up input side of pipe.
         * */
        close (fd[0]);

        /**
         * Send String though the output side of pipe.
         * */
        write (fd[1], strings, strlen(strings)+1);
        exit(0);
    }
    else
    {
        close (fd[1]);

        // read from pipe.
        nbytes = read(fd[0], readBuffer, sizeof(readBuffer));
        printf("Received String : %s\n", readBuffer);
    }

    return 0;
}