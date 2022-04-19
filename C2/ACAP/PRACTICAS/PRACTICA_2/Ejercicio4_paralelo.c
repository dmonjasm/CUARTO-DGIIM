//Daniel Monjas Miguélez
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

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

double sumaLocal(double* matrix_local, int quantity){
    double suma = 0.0;

    for(int i=0; i < quantity; i++){
        suma += matrix_local[i];
    }

    return suma;
}

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    MPI_Status status;
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD , &rank);

    if(argc != 3){
        printf("Usage:./Ejercicio4 <filas> <columnas>\n");
        
        exit(EXIT_FAILURE);
    }

    int fil = atoi(argv[1]);
    int col = atoi(argv[2]);
    int celdas = fil*col;

    int nProcs;
    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

    if(celdas < nProcs){
        printf("El tamaño de la matriz no es compatible con el número de hebras\n");
        exit(EXIT_FAILURE);
    }

    int workSize = celdas/nProcs;
    int offset = celdas % nProcs;
    int quantity = workSize + (rank<offset);

    //Como conozco que cantidad de datos le corresponde a cada hebra le reservo un vector de double con tantas posiciones como celdas de la matriz vaya a recibir
    double* matrix_local=(double *)malloc(sizeof(double)*quantity);
    double resultado_local;

    if(rank == 0){
        int start[nProcs], quantities[nProcs];
        double** matrix = reservarMatriz(fil, col);

        sillyInit(matrix, fil, col);

        for(int i=0; i < nProcs; i++){
            start[i] = i*workSize + (i<offset?i:offset);
            quantities[i] =  workSize + (i<offset);
        }

        //Aprovecho que C almacena las matrices estáticas mapeando las filas contiguamente en memoria, de forma que en memoria 
        //la matriz se ve como si fuera un array de tamaño filas por columnas. Gracias a esto puedo usar scatterv sin problemas.
        //El reparto es equilibrado, pues todas las hebras reciben el mismo número de número para sumar (+-1 número por hebra).
        MPI_Scatterv(matrix[0], quantities, start, MPI_DOUBLE, matrix_local, quantity, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        resultado_local = sumaLocal(matrix_local, quantity);

        double resultado_total;

        MPI_Reduce(&resultado_local, &resultado_total, 1, MPI_DOUBLE, MPI_SUM, rank, MPI_COMM_WORLD);

        if(fil <= 7 && col <= 7){
            for(int i = 0; i < fil; i++){
                for(int j = 0; j < col; j++){
                    printf("%.3lf ", matrix[i][j]);
                }

                printf("\n");
            }
        }

        printf("\nEl resultado de la suma es %lf\n", resultado_total);

        liberarMatriz(matrix);
    }

    else{
        MPI_Scatterv(NULL, NULL, NULL, NULL, &(matrix_local[0]), quantity, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        resultado_local = sumaLocal(matrix_local, quantity);

        MPI_Reduce(&resultado_local, NULL, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    free(matrix_local);

    MPI_Finalize();
    return 0;
}
