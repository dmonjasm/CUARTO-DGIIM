//---------------------------------------------------------
// com.pthread.cc
//---------------------------------------------------------

#include <pthread.h>
#include <iostream>

const std::size_t N = 64;
char msg[N] = "";

void *escritor(void*)
{
	char *p = msg;
	for (char c = 'a'; c <= 'z'; ++c)
		*p++ = c;
	*p = '\0';
	std::cout << "escritor: " << msg << std::endl;
	return nullptr;
}

void *lector(void*)
{
	std::cout << "  lector: " << msg << std::endl;
	return nullptr;
}

int main()
{
	pthread_t e, l;
	
	pthread_create(&e, nullptr, escritor, nullptr);
	pthread_join(e, nullptr);
	
	pthread_create(&l, nullptr,   lector, nullptr);
	pthread_join(l, nullptr);
}
