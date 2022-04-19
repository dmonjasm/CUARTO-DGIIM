#!/bin/bash
#Daniel Monjas Miguélez

for((k = 2; k <= 16; k = 2*k))
do

	for((j = 1; j <= 5; j++))
	do
		mpirun -np "$k" ./Ejercicio5_paralelo 1400 1400 1400
	done 

done
