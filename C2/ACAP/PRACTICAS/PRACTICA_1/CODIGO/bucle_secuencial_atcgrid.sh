#!/bin/bash
#Daniel Monjas Miguélez
#17/03/2022

gcc PI.c -o PI

inicio=214748362

for ((i = 1 ; i <= 10 ; i++));
do
	srun -Aacap -p acap ./PI "$inicio"
	((inicio+=214748365))
done 
