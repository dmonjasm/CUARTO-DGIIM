#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
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
    MPI_Init(&argc, &argv);
    MPI_Status status;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(argc < 2)
        printf("./Ejercicio1: FALTAN ARGUMENTO.\nModo de uso: ./Ejercicio1 <tamaño>");

    else{
        int * ingresos = NULL;
        int * gastos = NULL;

        if(rank==0){
            int nProcs = 0;
            MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

            int potencial = atoi(argv[1]);
            ingresos = (int *) malloc(sizeof(int)*potencial);
            gastos = (int *) malloc(sizeof(int)*potencial);
            bufferInit(ingresos, potencial);
            bufferInit(gastos, potencial);

            int workSize = potencial/nProcs;
            int offset = potencial % nProcs;
            int start, quantity;

            double inicio = get_wall_time();
            for(int i=1; i < nProcs; i++){
                start = i*workSize + (i<offset?i:offset);
                quantity =  workSize + (i<offset);

                MPI_Send(&(ingresos[start]), quantity, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&(gastos[start]), quantity, MPI_INT, i, 0, MPI_COMM_WORLD);
            }

            quantity = workSize + (0 < offset);

            int parciales[2] = {sumaVec(ingresos, quantity), sumaVec(gastos, quantity)};
            int totales[2];

            MPI_Reduce(parciales, totales, 2, MPI_INT, MPI_SUM, rank, MPI_COMM_WORLD);
            
            double final = get_wall_time();

            printf("Tiempo: %.3lf segundos\n", final-inicio);
            printf("La suma de los ingresos es %i\n", totales[0]);
            printf("La suma de los gastos es %i\n", totales[1]);
            printf("La diferencia entre gastos e ingresos es %i (gastos-ingresos)\n", totales[0]-totales[1]);
        }

        else{
            MPI_Probe( 0 , MPI_ANY_TAG, MPI_COMM_WORLD , &status);

            int quantity = 0;
            MPI_Get_count(&status, MPI_INT, &quantity);
            ingresos = (int *) malloc(sizeof(int)*quantity);
            gastos = (int *) malloc(sizeof(int)*quantity);
            MPI_Recv(ingresos, quantity, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Recv(gastos, quantity, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            int parciales[2] = {sumaVec(ingresos, quantity), sumaVec(ingresos, quantity)};

            MPI_Reduce(parciales, 0, 2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        }

        free(ingresos);
        free(gastos);
    }

    MPI_Finalize();
    return 0;
}