// Calculo aproximado de PI mediante la serie de Leibniz e integral del cuarto de circulo
// https://es.wikipedia.org/wiki/Serie_de_Leibniz
// N.C. Cruz, Universidad de Granada, 2022
//DANIEL MONJAS MIGUÉLEZ
//17/03/2022

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

double get_wall_time(){
	struct  timeval time;

	if( gettimeofday(&time, NULL)){
		printf("Error de medición de tiempo\n");

		return 0;
	}

	return (double) time.tv_sec + (double) time.tv_usec * 0.000001;
}

double piLeibniz(int steps){
	double partpi = 0.0;
	double num = 1.0;
	double denom = 1.0;
	for(int i = 0; i<steps; i++){
		partpi += num/denom;
		num = -1.0*num; // Alternamos el signo
		denom += 2.0;
	}
	return 4.0*partpi;
}

double piRectangles(int intervals){
	double width = 1.0/intervals;
	double sum = 0.0, x;
	for(int i = 0; i<intervals; i++){
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

			inicio = get_wall_time();
			double pi = piLeibniz(steps);
			final = get_wall_time();
			time_used_leiniz = final - inicio;


			printf("PI por la serie de G. Leibniz [%d iteraciones] =\t%lf\n", steps, pi);

			inicio = get_wall_time();
			pi = piRectangles(steps);
			final = get_wall_time();
			time_used_rect = final - inicio;

			printf("PI por integraciÃ³n de cÃ­rculo [%d intervalos] = \t%lf\n", steps, pi);
			printf("\nTiempo: %.3lf segundos\n", time_used_leiniz+time_used_rect);
		}
	}
	return 0;
}
