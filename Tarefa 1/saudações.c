#include <stdio.h>
#include <mpi.h>

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
mpicc -o greetings greetings.c
mpirun -np 4 ./greetings

saida 

Process 0 sends greetings from machine node1!
Process 1 sends greetings from machine node1!
Process 2 sends greetings from machine node2!
Process 3 sends greetings from machine node2!
*/