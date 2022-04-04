#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

void* body(void* param){
	printf("Hola desde el hilo %ld\n", (long int) param);
	return 0;
}

int main(int argc, char* argv){
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	for(long int i = 0; i<NUM_THREADS; i++){
		pthread_create(&threads[i], &attr, body, (void*) i);
	}
	pthread_attr_destroy(&attr);
	pthread_exit(0);
}
