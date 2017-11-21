#include <stdio.h>
#include <mpi.h>

const double pi_ref = 3.141592653589793;

void main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int my_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int n_steps = 1000000000;
    double step = 1.0/n_steps;
    double sum = 0.0;

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
	double f_x = 1.0/(1.0+x*x);
        loc_sum += step*f_x;
    }

    // gather contributions from all ranks (all-to-all)
    MPI_Allreduce(&loc_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    double pi = 4.0*sum;
    printf("Rank %d:%21.15f\n", my_rank, pi);

    MPI_Finalize();
}
