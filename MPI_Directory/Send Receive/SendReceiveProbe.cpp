//
// Created by mohit on 2/3/17.
//

/**
 * Probing is similar to receive. It does everything similar to Receive but receiving the messages.
 * MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status)
 * It has to be done before recv. status can be used to get the message count.
 * */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "random"

int main(){
    MPI_Init(NULL, NULL);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_size != 2){
        fprintf(stderr, "Must use two processes for this example!");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number_amount;
    if(world_rank == 0){
        const int MAX_NUMBERS = 100;
        int numbers[MAX_NUMBERS];
        srand(time(NULL));
        number_amount = (rand()/(float )RAND_MAX)*MAX_NUMBERS;
        MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if(world_rank == 1){
        MPI_Status status;
        // probe and get the status.
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        // get the count from the status.
        MPI_Get_count(&status, MPI_INT, &number_amount);
        // recv the messages.
        int *number_buf = (int *)malloc(sizeof(int)*number_amount);
        MPI_Recv(number_buf, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("1 received %d dynamically from 0", number_amount);
        free(number_buf);
    }
    MPI_Finalize();

    return 0;
}