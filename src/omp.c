#include <stdio.h>
#include <omp.h>

const double pi_ref = 3.141592653589793;

void main(int argc, char* argv[]) {
    int n_steps = 1000000000;
    double step = 1.0/n_steps;
    double sum = 0.0;

#pragma omp parallel firstprivate(n_steps, step) reduction(+:sum)
{
#pragma omp for schedule(guided)
    for (int i = 0; i < n_steps; i++) {
	double x = (i + 0.5)*step;
        double f_x = 1.0/(1.0+x*x);
        sum += step*f_x;
    }
}

    double pi = 4.0*sum;
    printf("Pi: %24.15f\n", pi);
    printf("Ref:%24.15f\n", pi_ref);
}
