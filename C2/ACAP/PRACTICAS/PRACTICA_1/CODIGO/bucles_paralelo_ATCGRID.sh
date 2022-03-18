#!/bin/bash
#Daniel Monjas Miguélez
#17/03/2022

mpicc pi_paralelo.c -o pi_paralelo

for ((i = 1 ; i <= 5 ; i++));
do
	mpirun -np 2 ./pi_paralelo 214748362
	echo -e "\n"
done 
