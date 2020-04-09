#!/bin/bash
echo "Executing with 64 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 4 -e img64.ini  >> img64_e.txt
echo "Executing with 64 mutants \n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 3 -e img64.ini >> img64_p.txt
echo "Executing with 32 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 4 -e img32.ini  >> img32_e.txt
echo "Executing with 32 mutants \n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 3 -e img32.ini >> img32_p.txt 
echo "Executing with 16 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 4 -e img16.ini  >> img16_e.txt
echo "Executing with 16 mutants \n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 3 -e img16.ini >> img16_p.txt 
echo "Executing with 8 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone  -a 4 -e img8.ini  >> img8_e.txt 
echo "Executing with 8 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 3 -e img8.ini >> img8_p.txt 
echo "Executing with 4 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 4 -e img4.ini >> img4_e.txt 
echo "Executing with 4 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 3 -e img4.ini >> img4_p.txt 
echo "FINISHED!!\n"


echo "Executing with 64 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 4 -e comp64.ini  >> comp64_e.txt
echo "Executing with 64 mutants \n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 3 -e comp64.ini >> comp64_p.txt 
echo "Executing with 32 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 4 -e comp32.ini  >> comp32_e.txt
echo "Executing with 32 mutants \n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 3 -e comp32.ini >> comp32_p.txt 
echo "Executing with 16 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 4 -e comp16.ini  >> comp16_e.txt
echo "Executing with 16 mutants \n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 3 -e comp16.ini >> comp16_p.txt 
echo "Executing with 8 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone  -a 4 -e comp8.ini  >> comp8_e.txt 
echo "Executing with 8 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 3 -e comp8.ini >> comp8_p.txt 
echo "Executing with 4 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 4 -e comp4.ini >> comp4_e.txt 
echo "Executing with 4 mutants\n"
time mpirun -np 32 -f Malone/machinefiles ./Malone/malone -a 3 -e comp4.ini >> comp4_p.txt 
echo "FINISHED!!\n"
