//
// Created by mohit on 1/3/17.
//

#include <mpi.h>
#include <iostream>
#include <stdio.h>

using namespace std;

int main(){
    const int MAX_PING_PONG_LIMIT = 10;

    MPI_Init(NULL, NULL);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(world_size!=2){
        fprintf(stderr, "World Size must be two!");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int ping_pong_count = 0;
    int partner_rank = (world_rank + 1)%2;
    while(ping_pong_count < MAX_PING_PONG_LIMIT){
        if(world_rank == ping_pong_count%2){
            // Send event.
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            printf("%d sent and incremented ping pong count %d to %d\n", world_rank, ping_pong_count, partner_rank);
        }else{
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d Received Ping pong count %d from %d\n", world_rank, ping_pong_count, partner_rank);
        }
    }

    MPI_Finalize();
    return 0;
}
