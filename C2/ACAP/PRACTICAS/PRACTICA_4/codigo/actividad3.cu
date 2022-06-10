#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <sys/time.h>
#include <cmath>

#define NBLOCKS 8192
#define THREADS_PER_BLOCK 256
#define EPSILON 0.000000000001

const int numRuns = 10;

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        printf("Error en la medicion de tiempo CPU!!\n");
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
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
    double* matriz_container =(double *) malloc(sizeof(double)*fil*col);//Gran vector contiguo

    if(matriz_container){
        matriz = (double**) malloc(sizeof(double*)*fil);//Aqui guardaremos el comienzo de cada fila

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
    double inicio = 0.0, final = 0.0;


    inicio = get_wall_time();
    for(int i=0; i < filA;i++){
        for(int j=0; j < colB;j++){
            matrixC[i][j]=0.0;
            for(int k=0; k < colA ;k++){
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    final = get_wall_time();

    printf("Tiempo usado por la CPU:%f\n", final-inicio);

    return matrixC;
}

void comprobarMatrices(double** matCPU, int fil, int col, double** matGPU){
    bool iguales = true;

    for(int i=0; i < fil && iguales; i++){
        for(int j=0; j < col && iguales; j++){
            if(abs(matCPU[i][j] - matGPU[i][j]) > EPSILON){   
                iguales = false;
                printf("i:%i\tj:%i\n", i, j);
                printf("MatrizCPU:%.9f\n", matCPU[i][j]);
                printf("MatrizGPU:%.9f\n", matGPU[i][j]);
            }
        }
    }

    if(iguales)
        printf("--Cálculo correcto!!!--\n");

    else
        printf("--ERROR--\n");
}

__global__ void kernelProductoMatrices(double* matA, int filA, int colA, double* matB, int colB, double* matC){
    __shared__ int workSize;
    __shared__ int offset;
    __shared__ int start;
    __shared__ int quantity;

    
    if(threadIdx.x == 0){
        workSize = filA / gridDim.x;
        offset = filA % gridDim.x;
        quantity = (workSize + (blockIdx.x<offset));
        start = (blockIdx.x*workSize) + (blockIdx.x<offset?blockIdx.x:offset);
    }

    __syncthreads();

    for(int i= start; i < start+quantity; i++){
        for(int j=threadIdx.x; j < colB; j+=blockDim.x){
            for(int k=0; k < colA; k++){
                matC[i*colB+j] += matA[i*colA+k]*matB[k*colB+j];
            }
        }
    }
} 

int main(int argc, char* argv[]){
    if(argc != 4){
        printf("Usage: ./Ejercicio5 <filasA> <columnasA> <columnasB>\n");
        exit(EXIT_FAILURE);
    }

    int filA = atoi(argv[1]);
    int colA = atoi(argv[2]);
    int colB = atoi(argv[3]);

    double **matA = reservarMatriz(filA, colA);
    double **matB = reservarMatriz(colA, colB);
    double **matC = NULL;

    sillyInit(matA, filA, colA);
    sillyInit(matB, colA, colB);

    matC = productoMatrices(matA, filA, colA, matB, colB);

    if(filA < 7 && colB < 7){
        printf("Matriz A:\n");
        printMatrix(matA, filA, colA);
        printf("Matriz B:\n");
        printMatrix(matB, colA, colB);
        printf("MatrizCPU:\n");
        printMatrix(matC, filA, colB);
    }

    double* dev_matA = nullptr;
    double* dev_matB = nullptr;
    double* dev_matC = nullptr;
    double **matGPU = reservarMatriz(filA, colB);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float milliSeconds = 0.0;
    float aveGPUMS = 0.0;    

    for(int i=-1; i < numRuns; i++){ 
        cudaEventRecord(start);

        cudaMalloc( (void**) &dev_matA, filA*colA*sizeof(double));
        cudaMemcpy( dev_matA, matA[0], filA*colA*sizeof(double), cudaMemcpyHostToDevice);
        cudaMalloc( (void**) &dev_matB, colB*colA*sizeof(double));
        cudaMemcpy( dev_matB, matB[0], colB*colA*sizeof(double), cudaMemcpyHostToDevice);
        cudaMalloc( (void**) &dev_matC, filA*colB*sizeof(double)); 

        cudaMemset(dev_matC, 0.0, filA*colB*sizeof(double));

        if(i < 0){
            kernelProductoMatrices<<<NBLOCKS, THREADS_PER_BLOCK>>>(dev_matA, filA, colA, dev_matB, colB, dev_matC);
        }

        else{
            cudaDeviceSynchronize();
            kernelProductoMatrices<<<NBLOCKS, THREADS_PER_BLOCK>>>(dev_matA, filA, colA, dev_matB, colB, dev_matC);
            cudaDeviceSynchronize();
            cudaMemcpy( matGPU[0], dev_matC, filA*colB*sizeof(double), cudaMemcpyDeviceToHost);
        }

        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&milliSeconds, start,stop);
        
        if(i >= 0)
            aveGPUMS+=milliSeconds;

        cudaFree(dev_matA);
        cudaFree(dev_matB);
        cudaFree(dev_matC);

    }

    comprobarMatrices(matC, filA, colB, matGPU);
    printf("Tiempo usado por la GPU:%f\n",aveGPUMS/(1000*numRuns));

    if(filA < 7 && colB < 7){
        printf("\nMatrizGPU:\n");
        printMatrix(matGPU, filA, colB);
    }

    liberarMatriz(matA);
    liberarMatriz(matB);
    liberarMatriz(matC);
    liberarMatriz(matGPU);

    return 0;
}