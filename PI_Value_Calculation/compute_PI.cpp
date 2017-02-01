//
// Created by mohit on 30/1/17.
//


/**
 * Calculation of PI done using approximation of integration of 1/(1+x*x) -> pi/4.
 * Number of Processors = Number of threads available = 4.
 *
 * */

#include "compute_PI.h"

double abs_val(double time){
    double value;
    if(0.0 <= time){
        value = time;
    }else{
        value = -time;
    }
    return value;
}

double pi_est_omp(int n){
    double h, estimate;
    int i;
    double sum2, x;

    h = 1.0/(double) (2*n);

    sum2 = 0.0;

# pragma omp parallel \
    shared(h, n) \
    private(i, x)

# pragma omp for reduction ( + : sum2)
    for(i = 1; i<=n; i++){
        x = h*(double)(2*i - 1);
        sum2 += (1.0/(1.0 + x*x));
    }
    estimate = 4.0*sum2/(double) (n);

    return estimate;
}

double pi_est_seq(int n){
    double h, estimate, sum2, x;
    int i;

    h = 1.0/(double) (2*n);

    sum2 = 0.0;

    for(i = 1; i<=n; i++){
        x = h*(double) (2*i - 1);
        sum2 += (1.0/(1.0 + x*x));
    }
    estimate = 4.0 * sum2/(double) (n);
    return estimate;
}

void test(int logn_max){
    double error;
    double estimate;
    int logn;
    char mode[4];
    int n;
    double pi = 3.141592653589793;
    double wtime;
    cout<<"\n N = number of terms computed and added: \n";
    cout<<"\nMode = SEQ for sequential code\n";
    cout<<"\nMode = OMP for open MP enabled code\n(Performance depends on whether openMP is used and how many processes are available\n";
    cout<<"\nESTIMATE = The computed estimate of PI : \n";
    cout<<"\nError = (The computed estimate of PI - PI):\n";
    cout<<"\nTime = elapsed wall clock time;\n";

    n = 1;
    for(logn = 1; logn<=logn_max; logn++){
        // sequential calculation.
        strcpy(mode, "SEQ");
        wtime = omp_get_wtime();
        estimate = pi_est_seq(n);
        wtime = omp_get_wtime() - wtime;
        error = abs_val(estimate - pi);

        cout<<" "<<n<<" "<<mode<<" "<<estimate<<" "<<error<<" "<<wtime<<"\n";

        strcpy(mode, "OMP");
        wtime = omp_get_wtime();
        estimate = pi_est_omp(n);
        wtime = omp_get_wtime() - wtime;
        error = abs_val(estimate - pi);

        cout<<" "<<n<<" "<<mode<<" "<<estimate<<" "<<error<<" "<<wtime<<"\n";

        n = n*10;
    }
    return;
}



int main(){
    int logn_max = 10;
    cout<<" \n\n*** Processor Details ***\n\n ";
    cout<<"Number of processors available : "<<omp_get_num_procs()<<"\n";
    cout<<"Number of Threads : "<<omp_get_max_threads()<<"\n";
    cout<<"\n\n*** Compute PI ***\n\n";
    test(logn_max);

    return 0;
}