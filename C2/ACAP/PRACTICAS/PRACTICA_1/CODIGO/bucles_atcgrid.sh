#!/bin/bash

inicio=214748362

for ((i = 1 ; i <= 10 ; i++));
do
	srun -Aacap -p acap ./PI "$inicio"
	((inicio+=214748365))
done 
