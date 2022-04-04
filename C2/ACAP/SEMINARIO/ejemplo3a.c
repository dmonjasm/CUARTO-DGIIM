#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

void* body(void* param){
	printf("Hola de nuevo\n");
	return 0;
}

int main(int argc, char* argv){
	pthread_t threads[NUM_THREADS];
	for(long int i = 0; i<NUM_THREADS; i++){
		pthread_create(&threads[i], 0, body, (void*) i);
	}
	for(long int i = 0; i<NUM_THREADS; i++){
		pthread_join(threads[i], 0);
		printf("El hilo %ld ha terminado\n", i);
	}
	return 0;
}
