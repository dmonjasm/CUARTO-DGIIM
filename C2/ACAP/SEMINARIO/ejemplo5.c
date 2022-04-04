#include <stdio.h>
#include <pthread.h>

pthread_t master;

void* body(void* param){
	pthread_t me = pthread_self();
	if(pthread_equal(me, master)){
		printf("[WORKER]: Yo soy el master :-D\n");
	}else{
		printf("[WORKER]: Yo no soy el master :-(\n");
	}
	return 0;
}

int main(int argc, char* argv){
	master = pthread_self();
	pthread_t worker;

	pthread_create(&worker, 0, body, 0);
	pthread_join(worker, 0);

	if(pthread_equal(pthread_self(), master)){
		printf("[MASTER]: Yo soy el master :-D\n");
	}else{
		printf("[MASTER]: Yo no soy el master :-(\n");
	}
	return 0;
}
