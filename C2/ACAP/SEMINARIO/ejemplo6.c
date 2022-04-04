#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

pthread_once_t once_control = PTHREAD_ONCE_INIT;

void Inicializo(void){
	printf("Estoy inicializando!\n");
}

void* body(void* param){
	pthread_once(&once_control, Inicializo);
	printf("Hola desde el hilo %ld\n", (long int) param);
	return 0;
}

int main(int argc, char* argv){
	pthread_t threads[NUM_THREADS];
	for(long int i = 0; i<NUM_THREADS; i++){
		pthread_create(&threads[i], 0, body, (void*) i);
	}
	pthread_exit(0);
}
