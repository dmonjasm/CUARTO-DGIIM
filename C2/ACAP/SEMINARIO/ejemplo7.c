#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 4

pthread_barrier_t barrera;

void* body(void* param){
	long int me = (long int) param;
	if(me==0){
		sleep(10);
	}
	pthread_barrier_wait(&barrera);
	printf("Hola desde el hilo [%ld]\n", me);
	return 0;
}

int main(int argc, char* argv){
	pthread_t threads[NUM_THREADS];
	pthread_barrier_init(&barrera, 0, NUM_THREADS);
	for(long int i = 0; i<NUM_THREADS; i++){
		pthread_create(&threads[i], 0, body, (void*) i);
	}
	for(long int i = 0; i<NUM_THREADS; i++){
		pthread_join(threads[i], 0);
	}
	printf("Hemos terminado!\n");
	pthread_barrier_destroy(&barrera);
	return 0;
}
