#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

void* body(void* param){
	long int val = *((long int*) param);
	printf("Hola, soy el hilo %ld\n", val);
	return 0;
}

int main(int argc, char* argv){
	pthread_t threads[NUM_THREADS];
	for(long int i = 0; i<NUM_THREADS; i++){
		pthread_create(&threads[i], 0, body, &i);
	}
	pthread_exit(0);
}
