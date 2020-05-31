#!/bin/bash

echo "executing mpi!!\n"

mpirun -np 4 ./malone -a 4 -e env_server.ini

echo "end!!\n"
