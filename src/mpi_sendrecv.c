#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const double pi_ref = 3.1415926535897932384626433832795;

void main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int my_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int n_steps = 1000000000;
    double step = 1.0/n_steps;

    int master_rank = 0;
    if (my_rank == master_rank) {
        printf("\nComputing pi using %d workers\n", world_size - 1);

        int n_steps_per_proc = n_steps/(world_size - 1);
        for (int i_rank = 1; i_rank < world_size; i_rank++) {
            // compute index range for worker i
            int i_idx[2];
            i_idx[0] = (i_rank - 1)*n_steps_per_proc;
            i_idx[1] = i_idx[0] + n_steps_per_proc;

            // adjust in case n_steps is not divisible by number of workers
            if (i_rank == world_size - 1) {
                i_idx[1] = n_steps;
            }

            // send index range to worker i
            MPI_Send(i_idx, 2, MPI_INT, i_rank, 100+i_rank, MPI_COMM_WORLD);
        }

        double tot_sum = 0.0;
        for (int i_rank = 1; i_rank < world_size; i_rank++) {
            // recieve contribution from worker i
            double loc_sum = 0.0;
            MPI_Recv(&loc_sum, 1, MPI_DOUBLE, i_rank, 200+i_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            tot_sum += loc_sum;
        }

        double pi = 4.0*step*tot_sum;
        printf("Pi:%25.15f\n", pi);
    } else {
        // recieve index range from master
        int i_idx[2];
        MPI_Recv(i_idx, 2, MPI_INT, master_rank, 100+my_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // compute my contribution to the sum
        double loc_sum = 0.0;
        for (int i = i_idx[0]; i < i_idx[1]; i++) {
	    double x = (i + 0.5)*step;
	    loc_sum += 1.0/(1.0+x*x);
        }

        // send my contrubution to master
        MPI_Send(&loc_sum, 1, MPI_DOUBLE, master_rank, 200+my_rank, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
