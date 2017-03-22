//
// Created by mohit on 2/2/17.
//

#include "Matrix_multiplication.h"


int main(){

    int A[N][N];
    int B[N][N];
    int C[N][N];
    double wtime;
    int i, j, k;

    cout<<"\nMatrix Multiplication using Multiple Threads.\n";

    int threads = omp_get_max_threads();
    int processors_avail = omp_get_num_procs();

    cout<<"\nNumber of Processors Available : "<<processors_avail<<"\n";
    cout<<"\nNumber of Threads : "<<threads<<"\n";

    wtime = omp_get_wtime();

#pragma omp parallel shared (A, B, C) private (i, j, k)
    {
#pragma omp for
        for(i = 0; i<N; i++){
            for(j=0; j<N; j++){
                A[i][j] = i+j+1;
            }
        }

        // B <= A
#pragma omp for
        for(i=0; i<N; i++){
            for(j=0; j<N; j++){
                B[i][j] = A[i][j];
            }
        }

        // Compute C = A*B.
#pragma omp for
        for(i=0; i<N; i++){
            for(j=0; j<N; j++){
                C[i][j] = 0;
                for(k=0; k<N; k++){
                    C[i][j] += A[i][k] + B[k][j];
                }
            }
        }
    }

    wtime = omp_get_wtime() - wtime;
    cout<<"\nTime Elapsed : "<<wtime<<"\n";

    return 0;
}