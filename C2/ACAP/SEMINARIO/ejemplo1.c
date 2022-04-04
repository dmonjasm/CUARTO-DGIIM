#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

void* body(void* param){
	printf("Hola PThreads!\n");
	return 0;
}

int main(int argc, char* argv){
	pthread_t threads[NUM_THREADS];
	for(int i = 0; i<NUM_THREADS; i++){
		pthread_create(&threads[i], 0, body, 0);
	}
	pthread_exit(0);
}
