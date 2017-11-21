# [Introduction to parallel programming](http://cicero.xyz/v2/remark/github/stigrj/parallel-programming-talk/narvik/talk.mkd/)
Talk given in Narvik, November 2017

### Building and running the serial code:
```bash
$ gcc serial.c -o serial.x
$ time ./serial.x
```

### Building and running the OpenMP code:
```bash
$ gcc -fopenmp omp.c -o omp.x
$ time OMP_NUM_THREADS=4 ./omp.x
```

### Building and running the MPI domain decomposition code:
```bash
$ mpicc mpi_domain.c -o mpi_domain.x
$ time mpirun -np 4 ./mpi_domain.x
```

### Building and running the MPI master/slave code:
```bash
$ mpicc mpi_master_slave.c -o mpi_master_slave.x
$ time mpirun -np 4 ./mpi_master_slave.x
```
