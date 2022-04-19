#!/bin/bash
#Daniel Monjas Miguélez

for((k = 2; k <= 16; k = 2*k))
do
	echo Cores $k >> resultados_paralelo.txt
	for((i = 16; i <= 1500; i+=149))
	do
		echo Tamaño $i >> resultados_pc.txt
		echo Tamaño $i >> resultados_paralelo.txt
		for((j = 1; j <= 5; j++))
		do	
			./Ejercicio5 "$i" "$i" "$i" >> resultados_pc.txt
		done
			
		
			for((j = 1; j <= 5; j++))
			do
				mpirun --oversubscribe -np "$k" ./Ejercicio5_paralelo "$i" "$i" "$i" >> resultados_paralelo.txt
			done 
	done
done
