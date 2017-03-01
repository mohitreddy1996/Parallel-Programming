//
// Created by mohit on 1/3/17.
//

#include "SendReceive.h"

/**
 * MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, tag, MPI_Comm communicator)
 * data = buffer.
 * count = number of elements in the buffer.
 * datatype = type of the data in the buffer.
 * destination = rank/id of the destination process.
 * tag = id/ rank given to the message being sent to the destination.
 * communicator = Environment where process will communicate.
 *
 * MPI_Recv(void *data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)
 * same as Send.
 * status = which will keep
 * */

int main(){

    // Initialize the MPI Environment
    MPI_Init(NULL, NULL);

    int world_rank, world_size;
    // Find out rank and size of the world.
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_size < 2){
        fprintf(stderr, "World Size must be greater than 1.");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int number;
    /**
     * Consider one of the process as a master.
     *
     * */
    if(world_rank == 0){
        number = -1;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }else if(world_rank == 1){
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 Received Number %d from process 0\n", number);
    }

    MPI_Finalize();

    return 0;
}