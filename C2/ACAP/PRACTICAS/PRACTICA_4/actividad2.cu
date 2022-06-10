#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <sys/time.h>

#define NBLOCKS 8192
#define THREADS_PER_BLOCK 256

const int numRuns = 10;

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        printf("Error en la medicion de tiempo CPU!!\n");
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void vector_init(double* vector, int size){
    for(int i=0; i < size; i++){
        vector[i] = (double)rand()/RAND_MAX + rand();
    }
}

__device__ double MyatomicMax(double *address, double val)
{
    unsigned long long ret = __double_as_longlong(*address);
    while(val > __longlong_as_double(ret))
    {
        unsigned long long old = ret;
        if((ret = atomicCAS((unsigned long long *)address, old, __double_as_longlong(val))) == old)
            break;
    }
    return __longlong_as_double(ret);
}

__global__ __forceinline__ void kernelMax(double* vector, int size, double* max){
    __shared__ int workSize;
    __shared__ int offset;
    __shared__ int start;
    __shared__ int quantity;
    __shared__ double max_local;

    if(threadIdx.x == 0){
        workSize = size/gridDim.x;
        offset = size%gridDim.x;
        start = blockIdx.x*workSize + (blockIdx.x < offset?blockIdx.x:offset);
        quantity = workSize + (blockIdx.x < offset);
        max_local = vector[0];
    }

    __syncthreads();

    for(int i=start+threadIdx.x; i < start+quantity; i+=blockDim.x){
        if(max_local < vector[i]){
            MyatomicMax(&max_local, vector[i]);
        }
    }

    __syncthreads();

    if(*max < max_local && threadIdx.x == 0)
       MyatomicMax(max,max_local);
}

double maximo_secuencial(double* vector, int size){
    double inicio, fin;

    inicio = get_wall_time();

    double max = vector[0];

    for(int i=1; i < size; i++){
        if(vector[i] > max)
            max = vector[i];
    }

    fin = get_wall_time();

    printf("El tiempo consumido por la CPU es %f\n", fin-inicio);

    return max;
}

void comparar_maximos(double max_cpu, double max_gpu){
    if(max_cpu != max_gpu)
        printf("Error!!!\n");
    
    else{
        printf("\n---Comprobación correcta!!!---\n");
    }

    printf("Maximo-CPU: %f\n", max_cpu);
    printf("Maximo-GPU: %f\n", max_gpu);
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
    double max_gpu = 0.0;

    double *dev_vec = nullptr;
    double *dev_max = nullptr;

    float milliSeconds = 0.0;
    float aveGPUMS = 0.0;

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    for(int i=-1; i < numRuns; i++){
        cudaEventRecord(start);

        cudaMalloc( (void**) &dev_vec, size*sizeof( double ) );
        cudaMemcpy( dev_vec, vec, size*sizeof(double), cudaMemcpyHostToDevice );
        cudaMalloc( (void**) &dev_max, sizeof( double ));
    
        cudaMemset(dev_max, 0.0 , sizeof(double));
        
        if(i < 0)
            kernelMax<<<NBLOCKS, THREADS_PER_BLOCK>>>(dev_vec, size, dev_max);
        
        else{
            cudaDeviceSynchronize();
            kernelMax<<<NBLOCKS, THREADS_PER_BLOCK>>>(dev_vec, size, dev_max);
            cudaDeviceSynchronize();
            cudaMemcpy( &max_gpu, dev_max, sizeof(double), cudaMemcpyDeviceToHost);
        }

        cudaFree(dev_vec);
        cudaFree(dev_max);

        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&milliSeconds, start,stop);

        if(i>=0)
            aveGPUMS += milliSeconds;
    }

    printf("El tiempo consumido por la GPU es %f\n", aveGPUMS/(1000*numRuns));

    comparar_maximos(max_cpu, max_gpu);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return 0;
}