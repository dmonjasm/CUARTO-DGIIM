//Daniel Monjas Miguelez
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
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

//Para inicializar los vectores asumimos que las unidades son microgramos
void sillyInit(double* vector, int tam){
    for(int i=0; i < tam; i++){
        vector[i] = (double) rand() / RAND_MAX * 40.0;
    }
}

double dotProduct(int len, double* vecA, double* vecB){
	double result = 0.0;

	for(int i = 0; i<len; i++){
		result += vecA[i]*vecB[i];
	}
	return result;
}

//Lo que haré es que cada hebra calcule la suma de cuadrados de su trozo de vector
//tras esto sumo todas las sumas parciales de cuadrados y les hago la raíz
double moduloCuadrado(double* vector, int tam){
    double suma = 0.0;

    for(int i=0; i < tam; i++){
        suma += pow(vector[i], 2);
    }

    return suma;
}

void printVector(double* vector, int tam){
    for(int i=0; i < tam; i++){
        printf("%.3lf\n", vector[i]);
    }

    printf("\n");
}

int main(int argc, char* argv[]){
    MPI_Init(&argc,&argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int nProcs;
    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

    if(argc != 2){
        printf("Usage: ./Ejercicio6 <tam_vectorA> <tam_vectorB>\n");
        exit(EXIT_FAILURE);
    }   

    //Los tamaños de los vectores tienen que se iguales, pues sino no se podría hacer la similitud del coseno
    int tam = atoi(argv[1]);

    if(tam < 0){
        printf("Alguno de los tamaños introducidos no es válido.\n");
        exit(EXIT_FAILURE);
    }

    double* vecA = NULL;
    double* vecB = NULL;

    int workSize = tam/nProcs;
    int offset = tam % nProcs;
    int quantity = workSize + (rank<offset);
    double resultado_local[3];

    if(rank == 0){
        vecA = (double *)malloc(sizeof(double)*tam);
        vecB = (double *)malloc(sizeof(double)*tam);

        sillyInit(vecA,tam);
        sillyInit(vecB,tam);

        double inicio = get_wall_time();

        int start[nProcs], quantities[nProcs];

        for(int i = 0; i < nProcs; i++){
            start[i] = i*workSize + (i<offset?i:offset);
            quantities[i] =  workSize + (i<offset);
        }

        double* vec_local1 = (double *)malloc(sizeof(double)*quantity);
        double* vec_local2 = (double *)malloc(sizeof(double)*quantity);
        
        MPI_Scatterv(vecA, quantities, start, MPI_DOUBLE, vec_local1, quantity, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatterv(vecB, quantities, start, MPI_DOUBLE, vec_local2, quantity, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        double resultado_total[3];

        resultado_local[0] = dotProduct(quantity, vec_local1,vec_local2);
        resultado_local[1] = moduloCuadrado(vec_local1, quantity);
        resultado_local[2] = moduloCuadrado(vec_local2, quantity);

        MPI_Reduce( &resultado_local , &resultado_total , 3, MPI_DOUBLE , MPI_SUM , 0 , MPI_COMM_WORLD);

        double resultado_final = resultado_total[0]/(sqrt(resultado_total[1])*sqrt(resultado_total[2]));

        double final = get_wall_time();

        if(tam <= 5){
            printf("Vector A\n");
            printVector(vecA, tam);

            printf("Vector B\n");
            printVector(vecB, tam);
        }

        printf("\nSimilitud del coseno: %lf\n", resultado_final);
        printf("Tiempo consumido: %lf\n", final-inicio);

        free(vec_local1);
        free(vec_local2);
    }

    else{
        vecA = (double *)malloc(sizeof(double)*quantity);
        vecB = (double *)malloc(sizeof(double)*quantity);

        MPI_Scatterv(NULL, NULL, NULL, NULL, vecA, quantity, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatterv(NULL, NULL, NULL, NULL, vecB, quantity, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        resultado_local[0] = dotProduct(quantity, vecA,vecB);
        resultado_local[1] = moduloCuadrado(vecA, quantity);
        resultado_local[2] = moduloCuadrado(vecB, quantity);

        MPI_Reduce( &resultado_local , NULL , 3, MPI_DOUBLE , MPI_SUM , 0 , MPI_COMM_WORLD);
    }

    free(vecA);
    free(vecB);

    MPI_Finalize();
    return 0;   
}
