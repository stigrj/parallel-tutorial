#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

const double pi_ref = 3.1415926535897932384626433832795;

void main(int argc, char* argv[]) {
    printf("\nComputing pi using %d threads\n\n", omp_get_max_threads());

    int n_steps = 1000000000;
    double step = 1.0/n_steps;
    double sum = 0.0;

#pragma omp parallel firstprivate(n_steps, step) shared(sum)
{
    int count = 0;
#pragma omp for
    for (int i = 0; i < n_steps; i++) {
	double x = (i + 0.5)*step;
	sum += 1.0/(1.0+x*x);
        count++;
    }
    printf("Thread nr %d: %d steps\n", omp_get_thread_num(), count);
}

    double pi = 4.0*step*sum;
    printf("\nPi:%25.15f\n\n", pi);
}
