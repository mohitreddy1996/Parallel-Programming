//
// Created by mohit on 1/3/17.
//

/**
 * Use MPI_Status for more dynamic features of the message being transferred.
 * Additional Information is received on MPI_Recv -> 1) Rank of the sender. 2) Tag of the message. 3) The length of the message
 *
 * Ex : MPI_Get_Count( MPI_Status* status, MPI_Datatype datatype, int *count);
 * The count variable is the total number of the datatype elements that were received.
 *
 * MPI_Recv can take MPI_ANY_SOURCE and MPI_ANY_TAG, in this case MPI_Status is the way to find out the actual sender and
 * the tag of the message.
 * */

#include <mpi.h>
#include <stdio.h>

int main(){

    MPI_Init(NULL, NULL);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_size != 2){
        fprintf(stderr, "World Size has to be 2.");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    const int MAX_NUMBER = 100;
    int numbers[MAX_NUMBER];
    int number_amount;
    if(world_rank == 0){
        // random number of integers to send them to process 1.
        srand(time(NULL));
        number_amount = (rand()/(float)RAND_MAX)*MAX_NUMBER;
        MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("0 sent %d numbers to 1\n", number_amount);
    }else if(world_rank == 1){
        MPI_Status status;
        // receive atmost MAXNUMBERs
        MPI_Recv(numbers, MAX_NUMBER, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        // use MPI_status to get the number of messages/size of the message received.
        MPI_Get_count(&status, MPI_INT, &number_amount);
        // status consists of source, tag and error.
        printf("1 Received %d Numbers from 0. Source = %d, Tag = %d\n", number_amount, status.MPI_SOURCE, status.MPI_TAG);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}