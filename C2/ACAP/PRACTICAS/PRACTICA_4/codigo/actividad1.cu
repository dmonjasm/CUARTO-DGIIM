#include <stdio.h>
#include <stdlib.h>

void vector_init(double* vector, int size){
    for(int i=0; i < size; i++){
        vector[i] = (double)rand()/RAND_MAX;
    }
}

__global__ void calcularMax(double* vector, int size){
    __shared__ double aux;
}

double maximo_secuencial(double* vector, int size){
    double max = vector[0];

    for(int i=1; i < size; i++){
        if(vector[i] > max)
            max = vector[i];
    }

    return max;
}

double comparar_maximos(double max_cpu, double max_gpu){
    if(max_cpu != max_gpu)
        printf("Error!!!\n");
    
    else
        printf("Comprobación correcta\n");
}
int main(int argc, char * argv[]){
    if(argc < 2){
        printf("Usage: ./actividad1.cu <tam_vec>\n");
        exit(EXIT_FAILURE);
    }

    int size = atoi(argv[1]);
    double* vec = (double*)malloc(size*sizeof(double));

    vector_init(vec, size);

    double max_cpu = maximo_secuencial(vec, size);

    double* dev_vec = nullptr;
    cudaMalloc3D()

    for(int i=0; i < size; i++)
        printf("%f\n", vec[i]);

    printf("El maximo es: %f\n", max_cpu);
    
    return 0;
}