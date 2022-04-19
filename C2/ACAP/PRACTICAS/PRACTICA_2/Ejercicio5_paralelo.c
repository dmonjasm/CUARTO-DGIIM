//Daniel Monjas Miguelez
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
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

void sillyInit(double** matrixA, int filas, int columnas){
    for(int i=0; i < filas; i++){
        for(int j=0; j< columnas; j++){
            matrixA[i][j] = (double) rand()/RAND_MAX;
        }
    }
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
    MPI_Init(&argc, &argv); 
    MPI_Status status;
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD , &rank);

    if(argc != 4){
        printf("Usage: mpirun -np <num_threads> ./Ejercicio5_paralelo <filA> <colA> <colB>\n");
        exit(EXIT_FAILURE);
    }

    int filA = atoi(argv[1]);
    int colA = atoi(argv[2]);
    int colB = atoi(argv[3]);
    int celdas_result = filA*colB;
    int nProcs;

    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

    if(nProcs > filA){
        printf("No hay suficientes filas para tantos procesos\n");
        exit(EXIT_FAILURE);
    }

    //Calculo cuantas filas le corresponden a cada proceso
    int workSize = filA/nProcs;
    int offset = filA % nProcs;

    //Transformo el número de filas que le paso a cada proceso a número de celdas de A
    int quantity = (workSize + (rank<offset))*colA;

    double** matrixA = NULL;
    //La B la reserva fuera, ya que todos los procesos la van a tener entera.
    double** matrixB = reservarMatriz(colA, colB);
    double** matrixC = NULL;
    double** matrix_local = reservarMatriz(quantity/colA, colA);



    if(rank == 0){
        double inicio = get_wall_time();

        //Reservo las matrices que se van a multiplicar
        matrixA = reservarMatriz(filA, colA);

        //Inicializo aleatoriamente las matrices a multiplicar
        sillyInit(matrixA, filA, colA);
        sillyInit(matrixB, colA, colB);

        int start[nProcs], quantities[nProcs];

        //Calculo cuantas celdas le voy a pasar a cada proceso y a partir de que celda le voy a empezar a pasar a cada proceso
        //Este cálculo se hará para particionar la matriz A entre todos los procesos
        for(int i=0; i < nProcs; i++){
            start[i] = (i*workSize + (i<offset?i:offset))*colA;
            quantities[i] =  (workSize + (i<offset))*colA;
        }

        //Paso la matriz B entera a todos los procesos
        MPI_Bcast(matrixB[0], colA*colB, MPI_DOUBLE, rank, MPI_COMM_WORLD);

        //A cada proceso le paso las filas correspondientes de la matriz A
        MPI_Scatterv(matrixA[0], quantities, start, MPI_DOUBLE, matrix_local[0], quantity, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //Con la matriz B y el trozo que le corresponde de la A cada matriz calcula uno o varias filas de la matriz resultado
        matrixC = productoMatrices(matrix_local, quantity/colA, colA, matrixB, colB);

        double** resultado = reservarMatriz(filA, colB);

        //Calculo cuantas celdas de la matriz resultado va a recibir la hebra 0 de cada proceso y a partir de que celda empezará a colocarlas.
        for(int i=0; i < nProcs; i++){
            start[i] = (i*workSize + (i<offset?i:offset))*colB;
            quantities[i] =  (workSize + (i<offset))*colB;
        }

        //Uno todos los subresultados en el resultado total usando gatherv y las cantidades e inicios recién calculados.
        MPI_Gatherv( matrixC[0], quantity/colA*colB , MPI_DOUBLE , resultado[0], quantities ,start , MPI_DOUBLE, 0 , MPI_COMM_WORLD);

        double final = get_wall_time();


        //Si la matriz es suficientemente pequeña la muestro por terminal
        if(filA <= 7 && colB <=7){
            printf("Matrix A\n");
            printMatrix(matrixA, filA, colA);
            printf("\nMatrix B\n");
            printMatrix(matrixB, colA, colB);
            printf("\nResultado\n");
            printMatrix(resultado, filA, colB);
        }

        printf("Tiempo requerido: %lf\n", final-inicio);
    }

    else{
        //Cada worker recibe la matriz B
        MPI_Bcast(matrixB[0] , colA*colB, MPI_DOUBLE , 0, MPI_COMM_WORLD);

        //Cada worker recibe su trozo (filas) de la matriz A
        MPI_Scatterv(NULL, NULL, NULL, NULL, matrix_local[0], quantity, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //Hago el producto local con cada uno de los workers
        matrixC = productoMatrices(matrix_local, quantity/colA, colA, matrixB, colB);

        //Lo uno en el resultado total junto a los demás resultados locales
        MPI_Gatherv( matrixC[0], quantity/colA*colB , MPI_DOUBLE , NULL, NULL , NULL , NULL, 0 , MPI_COMM_WORLD);
    }

    //Libero las matrices dinámicas utilizadas.
    liberarMatriz(matrixB);
    liberarMatriz(matrixA);
    liberarMatriz(matrixC);
    liberarMatriz(matrix_local);

    MPI_Finalize();
    return 0;   
}


