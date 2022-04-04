#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

int sharedVal = 0;
pthread_mutex_t lock;

void* body(void* param){
	pthread_mutex_lock(&lock);
	sharedVal += 5;
	pthread_mutex_unlock(&lock);
	return 0;
}

int main( int argc, char* argv[]){
	printf("[INI] Valor compartido: %d\n", sharedVal);
	pthread_t hilos[NUM_THREADS];
	pthread_mutex_init(&lock, 0);
	for(int i = 0; i<NUM_THREADS; i++){
		pthread_create(&(hilos[i]), 0, body, 0);
	}
	for(int i = 0; i<NUM_THREADS; i++){
		pthread_join(hilos[i], 0);
	}
	pthread_mutex_destroy(&lock);
	printf("[FIN] Valor compartido: %d\n", sharedVal);
	return 0;
}
