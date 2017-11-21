#include <stdio.h>
#include <mpi.h>

const double pi_ref = 3.141592653589793;

void main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int my_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int master_rank = 0;
    if (my_rank == master_rank) {

        int n_steps = 1000000000;
        double step = 1.0/n_steps;
        double sum = 0.0;

        // distribute work
        for (int i_rank = 1; i_rank < world_size; i_rank++) {
            // compute index range for worker i
            int n_steps_per_proc = n_steps/(world_size - 1);
            int i_start = (i_rank - 1)*n_steps_per_proc;
            int i_end = i_start + n_steps_per_proc;

            // adjust in case n_steps is not divisible by number of workers
            if (i_rank == world_size - 1) {
                i_end = n_steps;
            }

            // send index range and step length to worker i
            MPI_Send(&i_start, 1, MPI_INT, i_rank, 100+i_rank, MPI_COMM_WORLD);
            MPI_Send(&i_end, 1, MPI_INT, i_rank, 200+i_rank, MPI_COMM_WORLD);
            MPI_Send(&step, 1, MPI_DOUBLE, i_rank, 300+i_rank, MPI_COMM_WORLD);
        }

        // gather result
        for (int i_rank = 1; i_rank < world_size; i_rank++) {
            // recieve contribution from worker i
            double loc_sum = 0.0;
            MPI_Recv(&loc_sum, 1, MPI_DOUBLE, i_rank, 400+i_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += loc_sum;
        }

        double pi = 4.0*sum;
        printf("Rank %d:%21.15f\n", my_rank, pi);

    } else {

        // recieve index range and step length from master
        double step;
        int i_start, i_end;

        MPI_Recv(&i_start, 1, MPI_INT, master_rank, 100+my_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&i_end, 1, MPI_INT, master_rank, 200+my_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&step, 1, MPI_DOUBLE, master_rank, 300+my_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // compute my contribution to the sum
        double loc_sum = 0.0;
        for (int i = i_start; i < i_end; i++) {
	    double x = (i + 0.5)*step;
	    double f_x = 1.0/(1.0+x*x);
            loc_sum += step*f_x;
        }

        // send my contrubution to master
        MPI_Send(&loc_sum, 1, MPI_DOUBLE, master_rank, 400+my_rank, MPI_COMM_WORLD);

    }

    MPI_Finalize();
}
