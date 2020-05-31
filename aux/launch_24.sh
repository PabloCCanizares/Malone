#!/bin/bash

echo "Starting the execution\n"

#echo "Executing with 32 workers\n"
#time mpirun -np 64 -f Malone/machinefiles ./Malone/malone -a 4 -e env_server.ini >> job_1_64_nomem_local.txt
#echo "Executing with 32 workers\n"
#time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 4 -e env_server.ini >> job_1_32_nomem_local.txt
echo "----------------------------------------------\n"
echo "Executing heuristic 0\n"
echo "Executing with 4 workers\n"
time mpirun -np 4 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_0.ini >> job_1_4_img_heur_0.txt
echo "Executing with 2 workers\n"
time mpirun -np 2 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_0.ini >> job_1_2_img_heur_0.txt

echo "----------------------------------------------\n"
echo "Executing heuristic 1\n"

echo "Executing with 4 workers\n"
time mpirun -np 4 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_1.ini >> job_1_4_img_heur_1.txt
echo "Executing with 2 workers\n"
time mpirun -np 2 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_1.ini >> job_1_2_img_heur_1.txt

echo "----------------------------------------------\n"
echo "Executing heuristic 2\n"
echo "Executing with 4 workers\n"
time mpirun -np 4 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_2.ini >> job_1_4_img_heur_2.txt
echo "Executing with 2 workers\n"
time mpirun -np 2 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_2.ini >> job_1_2_img_heur_2.txt

echo "----------------------------------------------\n"
echo "Executing heuristic 3\n"
echo "Executing with 4 workers\n"
time mpirun -np 4 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_3.ini >> job_1_4_img_heur_3.txt
echo "Executing with 2 workers\n"
time mpirun -np 2 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_3.ini >> job_1_2_img_heur_3.txt

echo "----------------------------------------------\n"
echo "Executing heuristic 4\n"
echo "Executing with 4 workers\n"
time mpirun -np 4 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_4.ini >> job_1_4_img_heur_4.txt
echo "Executing with 2 workers\n"
time mpirun -np 2 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_4.ini >> job_1_2_img_heur_4.txt

echo "===========================================================\n"

echo "Executing heuristic 0\n"
echo "Executing with 16 workers\n"
time mpirun -np 16 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_0.ini >> job_1_16_img_heur_0.txt
echo "Executing with 8 workers\n"
time mpirun -np 8 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_0.ini >> job_1_8_img_heur_0.txt

echo "Executing heuristic 1\n"
echo "Executing with 16 workers\n"
time mpirun -np 16 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_1.ini >> job_1_16_img_heur_1.txt
echo "Executing with 8 workers\n"
time mpirun -np 8 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_1.ini >> job_1_8_img_heur_1.txt

echo "Executing heuristic 2\n"
echo "Executing with 16 workers\n"
time mpirun -np 16 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_2.ini >> job_1_16_img_heur_2.txt
echo "Executing with 8 workers\n"
time mpirun -np 8 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_2.ini >> job_1_8_img_heur_2.txt

echo "Executing heuristic 3\n"
echo "Executing with 16 workers\n"
time mpirun -np 16 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_3.ini >> job_1_16_img_heur_3.txt
echo "Executing with 8 workers\n"
time mpirun -np 8 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_3.ini >> job_1_8_img_heur_3.txt

echo "Executing heuristic 4\n"
echo "Executing with 16 workers\n"
time mpirun -np 16 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_4.ini >> job_1_16_img_heur_4.txt
echo "Executing with 8 workers\n"
time mpirun -np 8 -f Malone/machinefiles ./Malone/malone -a 4 -e img_heur_4.ini >> job_1_8_img_heur_4.txt

echo "FINISHED!!\n"
