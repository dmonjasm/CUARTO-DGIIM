#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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

double sumaElementos(double** matrix, int fil, int col){
    double result = 0.0;

    for(int i=0; i < fil; i++){
        for(int j = 0; j < col; j++){
            result += matrix[i][j];
        }
    }

    return result;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage:./Ejercicio4 <filas> <columnas>\n");
        
        exit(EXIT_FAILURE);
    }

    int fil = atoi(argv[1]);
    int col = atoi(argv[2]);
    double** matrix = reservarMatriz(fil, col);

    sillyInit(matrix, fil, col);

    double resultado = sumaElementos(matrix, fil, col);

    if(fil < 7 && col < 7){
        for(int i = 0; i < fil; i++){
            for(int j = 0; j < col; j++){
                printf("%.3lf ", matrix[i][j]);
            }

            printf("\n");
        }
    }

    liberarMatriz(matrix);

    printf("El resultado de la suma es %.3lf\n", resultado);
}