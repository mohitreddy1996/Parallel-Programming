//
// Created by mohit on 2/2/17.
//

#include "Prime_Numbers.h"

int prime_numbers(int n){
    int i, j, prime, total_primes = 0;

#pragma omp parallel shared (n) private (i, j, prime)
#pragma omp for reduction (+ : total_primes)
    for(i=2; i<=n; i++){
        prime = 1;
        for(j=2; j<i; j++){
            if(i%j == 0){
                prime = 0;
                break;
            }
        }
        total_primes+=prime;
    }
    return total_primes;
}

void get_prime_numbers(int low, int high, int factor){
    int init = low;
    double wtime;
    while(init <= high){
        wtime = omp_get_wtime();

        int x = prime_numbers(init);
        wtime  = omp_get_wtime() - wtime;
        cout<<"\nTime Elapsed for N : "<<init<<" Number of Primes : "<<x<<" Time: "<<wtime<<"\n";

        init = init*factor;
    }
}

int main(){
    int threads = omp_get_max_threads();
    int processors_avail = omp_get_num_procs();

    cout<<"\nNumber of Processors Available : "<<processors_avail<<"\n";
    cout<<"\nNumber of Threads : "<<threads<<"\n";

    get_prime_numbers(1, 1400000, 10);

    return 0;
}