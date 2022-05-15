#include <cstdio>
#include <cstdlib>     // srand, rand
#include <ctime>       // time
#include <sys/time.h>  // get_wall_time

#define IMDEP 256
#define SIZE (100*1024*1024) // 100 MB

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

void* inicializarImagen(unsigned long nBytes){
        unsigned char * img = (unsigned char*) malloc( nBytes );
        for(unsigned long i = 0; i<nBytes; i++){
                img[i] = rand() % IMDEP;
        }
        return img;     
}

void histogramaCPU(unsigned char* img, unsigned long nBytes, unsigned int* histo){
	double inicio, final;
	
	inicio = get_wall_time();
	
        for(int i = 0; i<IMDEP; i++)
                histo[i] = 0;//Inicializacion
        for(unsigned long i = 0; i<nBytes; i++){
                histo[ img[i] ]++;
        }
        
        final = get_wall_time();
        printf("Tiempo de CPU (s): %.9lf\n", final-inicio);
}

long calcularCheckSum(unsigned int* histo){
        long checkSum = 0;
        for(int i = 0; i<IMDEP; i++){
                checkSum += histo[i];
        }
        return checkSum;
}

int compararHistogramas(unsigned int* histA, unsigned int* histB){
        int valido = 1; 
        for(int i = 0; i<IMDEP; i++){
                if(histA[i] != histB[i]){
                        printf("Error en [%d]: %u != %u\n", i, histA[i], histB[i]);
                        valido = 0;
                }
        }
        return valido;
}

__global__ void kernelHistograma(unsigned char *imagen, unsigned long size, unsigned int* histo){
        __shared__ unsigned int temp[IMDEP];
        //temp[threadIdx.x] = 0;

        for(int j = threadIdx.x; j < IMDEP; j+=blockDim.x){
                temp[j] = 0;
        }
        __syncthreads();

        unsigned long i = threadIdx.x + blockIdx.x * blockDim.x;
        int offset = blockDim.x * gridDim.x;

        while (i < size) {
                atomicAdd( &temp[imagen[i]], 1);
                i += offset;
        }

        __syncthreads();

        for(int j = threadIdx.x; j < IMDEP; j+=blockDim.x){
                atomicAdd( &(histo[j]), temp[j] );
        }
}

int main(void){
        unsigned char* imagen = (unsigned char*) inicializarImagen(SIZE);
        unsigned int histoCPU[IMDEP];
        histogramaCPU(imagen, SIZE, histoCPU);
        long chk = calcularCheckSum(histoCPU);
        printf("Check-sum CPU: %ld\n", chk);


        unsigned char *dev_imagen = 0;
        unsigned int *dev_histo = 0;
       
        cudaMalloc( (void**) &dev_imagen, SIZE );
        cudaMemcpy( dev_imagen, imagen, SIZE, cudaMemcpyHostToDevice );
        cudaMalloc( (void**) &dev_histo, IMDEP * sizeof( unsigned int) );

        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);
        float milliSeconds = 0.0;
        float aveGPUMS = 0.0;

        for(int iter = -1; iter<numRuns; iter++){//La iteración -1 es para que la tarjeta se ponga en marcha, normalmente siempre da peores tiempos.
                cudaMemset( dev_histo, 0, IMDEP * sizeof( unsigned int ) );
                if(iter<0){
                        kernelHistograma<<<NBLOCKS, THREADS_PER_BLOCK>>>(dev_imagen, SIZE, dev_histo);
                }else{
                        cudaDeviceSynchronize();
                        cudaEventRecord(start);
                        kernelHistograma<<<NBLOCKS, THREADS_PER_BLOCK>>>(dev_imagen, SIZE, dev_histo);
                        cudaEventRecord(stop);
                        cudaEventSynchronize(stop);
                        cudaEventElapsedTime(&milliSeconds, start, stop);
                        aveGPUMS += milliSeconds;
                }
        }
        cudaEventDestroy(start);
        cudaEventDestroy(stop);

        unsigned int gpuHisto[IMDEP];
        cudaMemcpy(gpuHisto, dev_histo, IMDEP * sizeof(unsigned int), cudaMemcpyDeviceToHost );
        chk = calcularCheckSum(gpuHisto);
        printf("Check-sum GPU: %ld\n", chk);
        
        if(compararHistogramas(histoCPU, gpuHisto))
                printf("Cálculo correcto!!\n");

        printf("Tiempo medio de ejecucion del kernel<<<%d, %d>>> sobre %u bytes [s]: %.9f\n", NBLOCKS, THREADS_PER_BLOCK, SIZE, aveGPUMS / (numRuns*1000.0));

        free(imagen);
        cudaFree(dev_imagen);
        cudaFree(dev_histo);
                
        return 0;
}
