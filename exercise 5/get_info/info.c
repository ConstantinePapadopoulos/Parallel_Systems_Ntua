#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int rank, size;
    char hostname[256];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    gethostname(hostname, 256);

    printf("Rank %d is running on node %s\n", rank, hostname);

    MPI_Finalize();
    return 0;
}

