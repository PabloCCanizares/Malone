#!/bin/bash

echo "executing mpi!!\n"

mpirun -np 4 ./malone -a 4 -e env_first.ini >>  test_img_0.txt
mpirun -np 4 ./malone -a 4 -e env_first_3.ini >>  test_img_3.txt
mpirun -np 4 ./malone -a 4 -e env_first_5.ini >>  test_img_5.txt
mpirun -np 4 ./malone -a 4 -e env_first_7.ini >>  test_img_7.txt
echo "end!!\n"
