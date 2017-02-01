//
// Created by mohit on 1/2/17.
//

#include "dijkstra.h"

void initialise_weights(int weights[VERTEX][VERTEX]){
    for(int i=0; i<VERTEX; i++){
        for(int j=0; j<VERTEX; j++){
            if(i==j){
                weights[i][j] = 0;
            }else{
                weights[i][j] = INF;
            }
        }
    }

    // initialise according to your
    weights[0][1] = weights[1][0] = 40;
    weights[0][2] = weights[2][0] = 15;
    weights[1][2] = weights[2][1] = 20;
    weights[1][3] = weights[3][1] = 10;
    weights[1][4] = weights[4][1] = 25;
    weights[2][3] = weights[3][2] = 100;
    weights[1][5] = weights[5][1] = 6;
    weights[4][5] = weights[5][4] = 8;

    return;

}

void print_distance_matrix(int weights[VERTEX][VERTEX]){
    for (int i = 0; i < VERTEX; ++i) {
        for (int j = 0; j < VERTEX; ++j) {
            if(weights[i][j] == INF){
                cout<<" INF ";
            }else{
                cout<<weights[i][j]<<" ";
            }
        }
        cout<<"\n";
    }
    return;
}



void find_nearest(int s, int e, int min_dist[VERTEX], bool connected[VERTEX], int *d, int *v){
    *d = INF;
    *v = -1;
    for(int i=s; i<=e; i++){
        if (!connected[i] && min_dist[i] < *d) {
            *d = min_dist[i];
            *v = i;
        }
    }
    return;
}



void update_minimum_distance(int s, int e, int mv, bool connected[VERTEX], int weights[VERTEX][VERTEX], int min_dist[VERTEX]){
    for(int i=s; i<=e; i++){
        if(!connected[i]){
            if(weights[mv][i] < INF){
                if(min_dist[mv] + weights[mv][i] < min_dist[i]){
                    min_dist[i] = min_dist[mv] + weights[mv][i];
                }
            }
        }
    }
    return;
}

void print_distance_from_zero(int *minimum_distance){
    for(int i=0; i<VERTEX; i++){
        cout<<"i: "<<i<<" distance: "<<minimum_distance[i]<<"\n";
    }
    return;
}


int *dijkstra_distance(int weights[VERTEX][VERTEX]){
    /**
     * Implement multi threaded Dijkstra.
     * => use omp single -> whenever execution of a single thread among
     * the team is required.
     * => use omp critical -> Similar to critical section. Only one thread can
     * execute at that particular time.
     * => use omp barrier -> Collection point of all threads after execution
     * among the team. Proceed only when all threads this statement.
     * */
    bool *connected;
    int my_first, my_id, my_md, my_mv, mv_step, md, mv, nth, my_last;
    int *minimum_distance;

    connected = new bool[VERTEX];
    // mark only the first node (itself) i.e = 0 as connected.
    connected[0] = true;
    // mark all other vertices are not connected.
    for(int i=1; i<VERTEX; i++){
        connected[i] = false;
    }
    // Initialise Minimum Distance to INF if there does not exist
    // a edge else weight of the edge.
    minimum_distance = new int[VERTEX];
    for(int i=0; i<VERTEX; i++){
        minimum_distance[i] = weights[0][i];
    }

#pragma omp parallel private (my_first, my_id, my_last, my_md, my_mv, mv_step) \
    shared (connected, minimum_distance, weights, md, mv, nth)
    {
        my_id = omp_get_thread_num();
        nth = omp_get_num_threads();
        my_first = (my_id*VERTEX)/nth;
        my_last = ((my_id+1)*VERTEX)/nth - 1;

        // Single thread should execute the following block. First coming
        // thread executes this.
    #pragma omp single
        {
            cout<<"\n P"<<my_id<<": Parallel region begins with "<<nth<<" threads.\n";
            cout<<"\n";
        }
        cout<<"\n P"<<my_id<<": First="<<my_first<<" Last="<<my_last<<"\n";

        // attach one more node on each iteration.
        for(mv_step = 1; mv_step < VERTEX; mv_step++) {
            // set md and mv values. Only thread should do this.
#pragma omp single
            {
                md = INF;
                mv = -1;
            }

            // each thread finds the nearest unconnected node in its part of the graph.
            find_nearest(my_first, my_last, minimum_distance, connected, &my_md, &my_mv);

            // check the minimum distance and moves. Each thread should enter this block one at a time.
#pragma omp critical
            {
                if (my_md < md) {
                    md = my_md;
                    mv = my_mv;
                }
            }
            // checkpoint. Wait till all threads finish their execution.
#pragma omp barrier
            // if mv is -1 no unconnected node.
            // Do not break from the parallel code. So execute and keep mv condition to each statement.
#pragma omp single
            {
                if (mv != -1) {
                    connected[mv] = true;
                    cout << " P" << my_id << ": Connecting node " << mv << "\n";
                }
            }
#pragma omp barrier
            if (mv != -1) {
                update_minimum_distance(my_first, my_last, mv, connected, weights, minimum_distance);
            }
#pragma omp barrier

        }
#pragma omp single
        {
            cout<<"\n P"<<my_id<<" : Exiting Parallel region\n";
        }
    }

    delete [] connected;
    return minimum_distance;
}


int main(){
    int i, j;
    int *minimum_distance, weights[VERTEX][VERTEX];

    // initialise the weights
    initialise_weights(weights);

    // print the distance Matrix.
    print_distance_matrix(weights);

    // Calculate Minimum Distance
    minimum_distance = dijkstra_distance(weights);

    // Print the results. Minimum distances from node 0;
    print_distance_from_zero(minimum_distance);

    // free the memory.
    delete []minimum_distance;

    cout<<"\nEnding Execution\n";

    return 0;
}