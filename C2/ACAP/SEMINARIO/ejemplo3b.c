#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

void* body(void* param){
	double input = ((long int) param) + 0.0;
	double* localRes = malloc(sizeof(double));
	localRes[0] = input*input;
	return localRes;
}

int main(int argc, char* argv){
	pthread_t threads[NUM_THREADS];
	for(long int i = 0; i<NUM_THREADS; i++){
		pthread_create(&threads[i], 0, body, (void*) i);
	}
	void* buffer = 0;
	for(long int i = 0; i<NUM_THREADS; i++){
		pthread_join(threads[i], &buffer);
		printf("El hilo %ld eleva su ID al cuadrado: %.2lf\n", i, *((double*) buffer));
		free(buffer);		
	}
	return 0;
}
