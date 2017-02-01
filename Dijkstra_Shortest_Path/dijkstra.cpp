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

int *dijkstra_distance(int weights[VERTEX][VERTEX]){
    /**
     * Implement multi threaded Dijkstra.
     * */
}

void print_distance_from_zero(int *minimum_distance){
    for(int i=0; i<VERTEX; i++){
        cout<<"i: "<<i<<" distance: "<<minimum_distance[i]<<"\n";
    }
    return;
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