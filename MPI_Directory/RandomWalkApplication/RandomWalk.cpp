//
// Created by mohit on 2/3/17.
//

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

/**
 * Given Min and Max and random walker W, W makes S random walks of arbitrary length to the right.
 * If the process goes out of bound, it wraps back around. W can only move one unit.
 *
 * Each process determines their part of the domain.
 *
 * */

typedef struct {
    int location;
    int num_steps_left_in_walk;
} Walker;

void decompose_domain(int domain_size, int world_rank, int world_size, int *subdomain_start, int *subdomain_size){
    if(world_size > domain_size){
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    *subdomain_start = domain_size/world_size * world_rank;
    *subdomain_size = domain_size/world_size;
    if(world_rank == world_size - 1){
        *subdomain_size += domain_size%world_size;
    }

}

void initialise_walkers(int number_walkers_per_process, int max_walk_size,
                        int subdomain_start, int subdomain_size, vector<Walker> *incoming_walkers) {
    Walker walker;
    for(int i=0; i<number_walkers_per_process; i++){
        walker.location = subdomain_start;
        walker.num_steps_left_in_walk = (rand()/(float) RAND_MAX) * max_walk_size;
        incoming_walkers.push_back(walker);
    }
}


void walk(Walker *walker, int subdomain_start, int subdomain_size,
          int domain_size, vector<Walker> *outgoing_walkers) {
    while (walker->num_steps_left_in_walk > 0) {
        if (walker->location == subdomain_start + subdomain_size) {
            if(walker->location == domain_size)
                walker->location = 0;
            outgoing_walkers.push_back(walker);
            break;
        }else {
            walker->num_steps_left_in_walk-- ;
            walker->location++;
        }
    }
}

void send_outgoing_walkers(vector<Walker> *outgoing_walkers, int world_rank, int world_size){
    MPI_Send((void *) outgoing_walkers->data(), outgoing_walkers->size()*sizeof(Walker), MPI_BYTE, (world_rank+1)%world_size, 0, MPI_COMM_WORLD);

    outgoing_walkers->clear();
}

void receive_incoming_walkers(vector<Walker> *incoming_walkers, int world_rank, int world_size){
    MPI_Status status;

    int incoming_rank = (world_rank == 0) ? world_size - 1 : world_rank - 1;
    // Probe and get the size.
    MPI_Probe(incoming_rank, 0, MPI_COMM_WORLD, &status);
    int incoming_walkers_size;
    MPI_Get_count(&status, MPI_BYTE, &incoming_walkers_size);
    incoming_walkers->resize(incoming_walkers_size/sizeof(Walker));
    MPI_Recv((void *)incoming_walkers->data(), incoming_walkers_size, MPI_BYTE, incoming_rank, 0,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}