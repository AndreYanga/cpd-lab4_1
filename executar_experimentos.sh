#!/bin/bash

# Compile all programs
mpicc -o greetings greetings.c
mpicc -o sendReceive sendReceive.c
mpicc -o sendReceive_modified sendReceive_modified.c
mpicc -o bcast_vs_send bcast_vs_send.c
mpicc -o custom_bcast custom_bcast.c

# Run experiments
echo "Running greetings"
mpirun -np 4 ./greetings | tee greetings_output.txt

echo "Running sendReceive"
mpirun -np 4 ./sendReceive 1000 | tee sendReceive_4_1000.txt
mpirun -np 4 ./sendReceive 10000 | tee sendReceive_4_10000.txt
mpirun -np 8 ./sendReceive 1000 | tee sendReceive_8_1000.txt

echo "Running sendReceive_modified"
mpirun -np 4 ./sendReceive_modified 1000 1 | tee sendReceive_mod_4_1byte.txt
mpirun -np 4 ./sendReceive_modified 100 1000000 | tee sendReceive_mod_4_1MB.txt

echo "Running bcast_vs_send"
mpirun -np 4 ./bcast_vs_send 1000000 | tee bcast_vs_send_4.txt
mpirun -np 8 ./bcast_vs_send 1000000 | tee bcast_vs_send_8.txt

echo "Running custom_bcast"
mpirun -np 4 ./custom_bcast 1000000 | tee custom_bcast_4.txt
mpirun -np 8 ./custom_bcast 1000000 | tee custom_bcast_8.txt