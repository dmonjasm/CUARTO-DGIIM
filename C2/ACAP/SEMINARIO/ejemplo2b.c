#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4

typedef struct t_struct{
	int id;
	int numThreads;
} tareas;

void* body(void* param){
	tareas* laMia = (tareas*) param;
	printf("Hola, soy el hilo %d de %d!\n", laMia->id, laMia->numThreads);
	return 0;
}

int main(int argc, char* argv){
	pthread_t threads[NUM_THREADS];
	struct t_struct miTarea[NUM_THREADS];
	for(long int i = 0; i<NUM_THREADS; i++){
		miTarea[i].id = i;
		miTarea[i].numThreads = NUM_THREADS;
		pthread_create(&threads[i], 0, body, &(miTarea[i]));
	}
	pthread_exit(0);
}
