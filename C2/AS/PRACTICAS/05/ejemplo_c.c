#include <pthread.h>
#include <stdio.h>

void* quien_soy(void* a)
{
	printf("[%lu]: hola\n", pthread_self());
	return NULL; // pthread_exit(NULL);
}

int main()
{
	pthread_t id;
	pthread_create(&id, NULL, quien_soy, NULL);
	pthread_join(id, NULL);
}
