#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Para sleep (UNIX)

const int NUM_THREADS = 8;

int finished_threads = 0;//Variable sobre la que vamos a hacer la condicion

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//Protector (exclusion mutua) de la condicion
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


void* funcionHebrada(void* arg){
	sleep(rand()%10);//Simulacion de trabajo a realizar... avisamos al terminar:
	pthread_mutex_lock(&mutex);
	finished_threads++;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	printf("Hilo [%ld] ha terminado\n", (long int) arg);
	return 0;
}


int main(){
	pthread_t* threads = malloc(sizeof(pthread_t)*NUM_THREADS);
	for(long int i = 0; i<NUM_THREADS; i++){
		pthread_create(&threads[i], 0, funcionHebrada, (void*) i);
	}

	//No hacemos join, pero aun asi el main sabra cuando han terminado:
	pthread_mutex_lock(&mutex);
	while(finished_threads != NUM_THREADS){//¿Por que no un simple "if"? -> (Spurious wakeup) https://en.wikipedia.org/wiki/Spurious_wakeup
		printf("[Principal] Epero :(\n");
		pthread_cond_wait(&cond, &mutex);//Yo, el principal, esperare
	}
	pthread_mutex_unlock(&mutex);
	printf("[Principal] Han terminado todos! :D\n");
	free(threads);
	return 0;
}
