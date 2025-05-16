/*
    Name: Custom MPI_Bcast Implementation
    Author: Student
    Date: 16/05/25
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void custom_bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
    int id, p, i;
    MPI_Status status;
    MPI_Comm_rank(comm, &id);
    MPI_Comm_size(comm, &p);

    if (id == root) {
        for (i = 0; i < p; i++) {
            if (i != root) {
                MPI_Send(buffer, count, datatype, i, 0, comm);
            }
        }
    } else {
        MPI_Recv(buffer, count, datatype, root, 0, comm, &status);
    }
}

int main(int argc, char *argv[]) {
    int id, p, size;
    double *array, custom_time, mpi_time;

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
    if (!id) for (int i = 0; i < size; i++) array[i] = i;

    // Test Custom Bcast
    MPI_Barrier(MPI_COMM_WORLD);
    custom_time = -MPI_Wtime();
    custom_bcast(array, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    custom_time += MPI_Wtime();

    // Test MPI_Bcast
    MPI_Barrier(MPI_COMM_WORLD);
    mpi_time = -MPI_Wtime();
    MPI_Bcast(array, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    mpi_time += MPI_Wtime();

    if (!id) {
        printf("Array Size= %d doubles, N Processes= %d\n", size, p);
        printf("Custom Bcast Time= %12.6f sec\n", custom_time);
        printf("MPI_Bcast Time= %12.6f sec\n", mpi_time);
        printf("Speedup (Custom/MPI)= %.2f\n", custom_time / mpi_time);
    }

    free(array);
    MPI_Finalize();
    return 0;
}