/*
    Name: Broadcast vs Individual Sends
    Author: Student
    Date: 16/05/25
    Description: Compares performance of MPI_Bcast vs individual MPI_Send/Recv.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int id, p, i, size;
    double bcast_time, send_time, *array;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (argc != 2) {
        if (!id) printf("Command line: %s <array-size-doubles>\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }
    size = atoi(argv[1]);
    array = (double *)malloc(size * sizeof(double));
    if (!array) {
        if (!id) printf("Memory allocation failed\n");
        MPI_Finalize();
        exit(1);
    }
    if (!id) for (i = 0; i < size; i++) array[i] = i;

    // Test MPI_Bcast
    MPI_Barrier(MPI_COMM_WORLD);
    bcast_time = -MPI_Wtime();
    MPI_Bcast(array, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    bcast_time += MPI_Wtime();

    // Test Individual Sends
    MPI_Barrier(MPI_COMM_WORLD);
    send_time = -MPI_Wtime();
    if (!id) {
        for (i = 1; i < p; i++)
            MPI_Send(array, size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(array, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    send_time += MPI_Wtime();

    if (!id) {
        printf("Array Size= %d doubles, N Processes= %d\n", size, p);
        printf("MPI_Bcast Time= %12.6f sec\n", bcast_time);
        printf("Individual Send Time= %12.6f sec\n", send_time);
        printf("Speedup (Send/Bcast)= %.2f\n", send_time / bcast_time);
    }

    free(array);
    MPI_Finalize();
    return 0;
}