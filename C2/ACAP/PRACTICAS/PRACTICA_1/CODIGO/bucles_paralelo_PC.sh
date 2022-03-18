#!/bin/bash
#Daniel Monjas Miguélez
#17/03/2022

mpicc pi_paralelo.c -o pi_paralelo

inicio=214748362

for ((j = 1 ; j <= 5 ; j++))
do
	inicio=214748362 
	for ((i = 1 ; i <= 10 ; i++));
	do
		mpiexec --oversubscribe -np 8 ./pi_paralelo "$inicio"
		((inicio+=214748364))
	done 
done
