/*
    Name: Send and Receive Message (Modified for Latency and Bandwidth)
    Copyright: 2021-22
    Author: Prof. Joao Costa (Modified by Student)
    Date: 16/05/25
    Description: Measures latency and bandwidth using MPI ring communication.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Status status;
    int id, p, i, rounds, size;
    double secs, latency, bandwidth;
    char *buffer;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (argc != 3) {
        if (!id) printf("Command line: %s <n-rounds> <message-size-bytes>\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }
    rounds = atoi(argv[1]);
    size = atoi(argv[2]);
    buffer = (char *)malloc(size);
    if (!buffer) {
        if (!id) printf("Memory allocation failed\n");
        MPI_Finalize();
        exit(1);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    secs = -MPI_Wtime();

    for (i = 0; i < rounds; i++) {
        if (!id) {
            MPI_Send(buffer, size, MPI_CHAR, 1, i, MPI_COMM_WORLD);
            MPI_Recv(buffer, size, MPI_CHAR, p-1, i, MPI_COMM_WORLD, &status);
        } else {
            MPI_Recv(buffer, size, MPI_CHAR, id-1, i, MPI_COMM_WORLD, &status);
            MPI_Send(buffer, size, MPI_CHAR, (id+1)%p, i, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    secs += MPI_Wtime();

    if (!id) {
        latency = (size <= 1) ? secs / (2.0 * rounds * p) : 0;
        bandwidth = (size > 1) ? (size * rounds * p) / (secs * 1e6) : 0; // MB/s
        printf("Rounds= %d, N Processes= %d, Message Size= %d bytes, Time= %12.6f sec\n",
               rounds, p, size, secs);
        if (size <= 1) printf("Latency= %6.2f us\n", latency * 1e6);
        if (size > 1) printf("Bandwidth= %6.2f MB/s\n", bandwidth);
    }

    free(buffer);
    MPI_Finalize();
    return 0;
}