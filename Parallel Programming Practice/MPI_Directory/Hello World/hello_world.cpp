//
// Created by mohit on 1/3/17.
//

#include "hello_world.h"

int main(int argc, char** argv){
    // Initialize the MPI Environment. Two arguments to MPI Init are not
    // used.
    MPI_Init(NULL, NULL);

    // Get the number of processes.
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process.
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor.
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_length;
    MPI_Get_processor_name(processor_name, &name_length);

    printf("Hello world! Processor : %s, Rank : %d out of %d processors\n", processor_name, world_rank, world_size);

    // Finalize. No More MPI Calls after this.
    MPI_Finalize();

    return 0;
}