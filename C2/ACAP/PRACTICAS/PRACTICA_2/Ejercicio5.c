#include <stdio.h>
#include <unistd.h>
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

void sillyInit(double** matrixA, int filas, int columnas){
    for(int i=0; i < filas; i++){
        for(int j=0; j< columnas; j++){
            matrixA[i][j] = (double) rand()/RAND_MAX;
        }
    }
}

double** reservarMatriz(int fil, int col){
    double** matriz = 0;
    double* matriz_container = malloc(sizeof(double)*fil*col);//Gran vector contiguo

    if(matriz_container){
        matriz = malloc(sizeof(double*)*fil);//Aqui guardaremos el comienzo de cada fila

        if(matriz){
            for(int i = 0; i<fil; i++){//Vamos a ir apuntando a cada fila
                matriz[i] = &(matriz_container[i*col]);//El inicio de la fila i es i*columnas
            }
        }
        
        else{//Ahora pediir memoria ha fallado... pero:
            printf("Error. No se ha podido reservar la carcasa 2D de la matriz.\n");
            free(matriz_container);//Esto si funciono, y debemos limpiarlo
        }
    }
    
    else{
        printf("Error. No se ha podido reservar memoria para la matriz.\n");
    }

    return matriz;//Tenemos esta memoria heap... y luego otro debera liberarla eh
}

void liberarMatriz(double** matriz){
	if(matriz){//Liberando el principio de la matriz ya estamos apuntando al principio del container
		free(matriz[0]);//Pero claro, solo si apunta a algo
	}

	free(matriz);//Ahora podemos eliminar el conjunto de punteros a fila
}

void printMatrix(double** matrixA, int filas, int columnas){
    for(int i=0; i < filas; i++){
        for(int j= 0; j < columnas; j++){
            printf("%.3lf ", matrixA[i][j]);
        }

        printf("\n");
    }

    printf("\n\n");
}

double** productoMatrices(double** matrixA, int filA, int colA, double** matrixB, int colB){
    double** matrixC = reservarMatriz(filA, colB);

    for(int i=0;i<colB;i++){
        for(int j=0; j<filA;j++){
            matrixC[j][i]=0.0;
            for(int k=0;k<colA;k++){
                matrixC[j][i]+=matrixA[j][k]*matrixB[k][i];
            }
        }
    }

    return matrixC;
}



int main(int argc, char* argv[]){

    if(argc != 4){
        printf("Usage: ./Ejercicio5 <filasA> <columnasA> <columnasB>\n");
        exit(EXIT_FAILURE);
    }

    int filA = atoi(argv[1]);
    int colA = atoi(argv[2]);
    int colB = atoi(argv[3]);

    double inicio = get_wall_time();
    double **matA = reservarMatriz(filA, colA);
    double **matB = reservarMatriz(colA, colB);
    double **matC = NULL;

    sillyInit(matA, filA, colA);
    sillyInit(matB, colA, colB);

    matC = productoMatrices(matA, filA, colA, matB, colB);

    double final = get_wall_time();

    if(filA <= 7 && colB <= 7){
        printf("Matrix A\n");
        printMatrix(matC, filA, colB);

        printf("\nMatrix B\n");
        printMatrix(matA, filA, colA);

        printf("\nMatrix C\n");
        printMatrix(matB, colA, colB);
    }

    printf("Tiempo requerido: %lf\n", final-inicio);

    liberarMatriz(matA);
    liberarMatriz(matB);

    return 0;
}