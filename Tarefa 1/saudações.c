#include <stdio.h>
#include <mpi.h>
#include <unistd.h> // <- Adicionado para gethostname

int main(int argc, char *argv[]) {
    int id, p;
    char hostname[256];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    gethostname(hostname, 256);

    printf("Process %d sends greetings from machine %s!\n", id, hostname);

    MPI_Finalize();
    return 0;
}



/*
mpicc -o saudações saudações.c
mpirun -np 4 ./saudações

*/