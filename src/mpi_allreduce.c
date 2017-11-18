#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const double pi_ref = 3.1415926535897932384626433832795;

void main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int my_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (my_rank == 0) {
        printf("\nComputing pi using %d processes\n", world_size);
    }

    int n_steps = 1000000000;
    double step = 1.0/n_steps;

    // compute my index range
    int n_steps_per_proc = n_steps/world_size;
    int i_start = my_rank*n_steps_per_proc;
    int i_end = i_start + n_steps_per_proc;

    // adjust in case n_steps is not divisible by world_size
    if (my_rank == world_size - 1) {
        i_end = n_steps;
    }

    // compute my contribution to the sum
    double loc_sum = 0.0;
    for (int i = i_start; i < i_end; i++) {
	double x = (i + 0.5)*step;
	loc_sum += 1.0/(1.0+x*x);
    }

    // gather contributions from all ranks (all-to-all)
    double tot_sum = 0.0;
    MPI_Allreduce(&loc_sum, &tot_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    double pi = 4.0*step*tot_sum;
    printf("Rank %d:%25.15f\n", my_rank, pi);

    MPI_Finalize();
}
