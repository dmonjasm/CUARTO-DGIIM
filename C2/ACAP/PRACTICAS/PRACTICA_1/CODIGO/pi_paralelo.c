// Calculo aproximado de PI mediante la serie de Leibniz e integral del cuarto de circulo
// https://es.wikipedia.org/wiki/Serie_de_Leibniz
// N.C. Cruz, Universidad de Granada, 2022
//DANIEL MONJAS MIGUÉLEZ
//17/03/2022

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

#define MASTER 0

double get_wall_time(){
	struct  timeval time;

	if( gettimeofday(&time, NULL)){
		printf("Error de medición de tiempo\n");

		return 0;
	}

	return (double) time.tv_sec + (double) time.tv_usec * 0.000001;
}

double piLeibniz(int steps, int id, int num_total){
	double partpi = 0.0;
	double num = 1.0;
	double alterna_signo = 1.0;

	if(id%2 != 0){	//Si el proceso empieza en un término impar de la suma, es una resta
		num = -1.0;
		alterna_signo = -1.0;
	}
		
	double denom = 1.0+id*2.0;	//Se ajusta el denominador en función de que proceso la ejecuta

	for(int i = id; i<steps; i+= num_total){
		partpi += num/denom;

		num = -1.0*alterna_signo;

		denom += 2.0*num_total;
	}
	return 4.0*partpi;
}

double piRectangles(int intervals, int id, int num_total){
	double width = 1.0/intervals;
	double sum = 0.0, x;
	for(int i = id; i<intervals; i+= num_total){
		x = (i + 0.5)*width;
		sum += 4.0/(1.0 + x*x);
	}
	return sum*width;
}

int main(int argc, char* argv[]){
	if(argc!=2){	//El primer argumento siempre es el nombre del programa
		printf("Uso: ./prog esfuerzo\n");
	}else{
		int steps = atoi(argv[1]);
		if(steps<=0){
			printf("El nÃºmero de iteraciones debe ser un entero positivo!\n");
		}else{
			double inicio = 0.0, final = 0.0;
			double time_used_leiniz = 0.0, time_used_rect = 0.0;
			int rank, numProcs;
			double suma_total = 0.0, tiempo_total=0.0;

			MPI_Init(&argc, &argv);
			MPI_Comm_rank(MPI_COMM_WORLD, &rank);
			MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

			inicio = get_wall_time();
			double pi = piLeibniz(steps, rank, numProcs);
			MPI_Reduce(&pi, &suma_total, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
			final = get_wall_time();
			time_used_leiniz = final - inicio;

			/*if(rank == MASTER)
				printf("PI por la serie de G. Leibniz [%d iteraciones] =\t%lf\n", steps, suma_total);
			*/
			inicio = get_wall_time();
			pi = piRectangles(steps, rank, numProcs);
			MPI_Reduce(&pi, &suma_total, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
			final = get_wall_time();
			time_used_rect = final - inicio;

			if(rank == MASTER){
				//printf("PI por integración de cálculo [%d intervalos] = \t%lf\n", steps, suma_total);
				printf("Tiempo: %.3lf segundos\n", time_used_leiniz+time_used_rect);
			}

			MPI_Finalize();
		}
	}
	return 0;
}
