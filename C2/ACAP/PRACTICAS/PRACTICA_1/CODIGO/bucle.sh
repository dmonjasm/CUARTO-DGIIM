#!/bin/bash

gcc PI.c -o PI

inicio=214748362

for ((i = 1 ; i <= 10 ; i++));
do
	echo "Ejecución $i ./PI $inicio"
	./PI "$inicio"
	echo -e "\n"
	((inicio+=214748365))
done 
