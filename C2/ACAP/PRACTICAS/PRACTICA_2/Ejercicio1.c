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

void bufferInit(int * buf, int length){
    for(int i=0; i < length; i++){
        buf[i] = rand()%10;
    }
}

int sumaVec(int * vec, int len){
    int suma = 0;
    for(int i=0; i < len; i++){
        suma += vec[i];
    }

    return suma;
}

int main(int argc, char * argv[]){
    if(argc < 2)
        printf("./Ejercicio1: FALTAN ARGUMENTO.\nModo de uso: ./Ejercicio1 <tamaño>");

    else{
        int potencial = atoi(argv[1]);
        int * ingresos = (int *) malloc(sizeof(int)*potencial);
        int * gastos = (int *) malloc(sizeof(int)*potencial);
        bufferInit(ingresos, potencial);
        bufferInit(gastos, potencial);

        double inicio = get_wall_time();
        int sum_ingresos = sumaVec(ingresos, potencial);
        int sum_gastos = sumaVec(gastos, potencial);

        double final = get_wall_time();


        printf("Tiempo %.3lf\n", final-inicio);
        printf("La suma de los ingresos es %i\n", sum_ingresos);
        printf("La suma de los gastos es %i\n", sum_gastos);
        printf("La diferencia entre gastos e ingresos es %i (gastos-ingresos)\n", sum_ingresos-sum_gastos);
    }
}