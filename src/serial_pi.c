#include <stdio.h>
#include <stdlib.h>

const double pi_ref = 3.1415926535897932384626433832795;

void main(int argc, char* argv[]) {
    printf("\nComputing pi using 1 thread\n");

    int n_steps = 1000000000;
    double step = 1.0/n_steps;
    double sum = 0.0;

    for (int i = 0; i < n_steps; i++) {
	double x = (i + 0.5)*step;
	sum += 1.0/(1.0+x*x);
    }

    double pi = 4.0*step*sum;
    printf("Pi:%25.15f\n\n", pi);
}
