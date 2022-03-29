#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <string>

//---------------------------------------------------------

std::size_t counter = 0;
std::string counter_name;

//---------------------------------------------------------

void result(int)
{
	std::cout << counter_name << " = " << counter << std::endl;
	exit(EXIT_SUCCESS);
}

//---------------------------------------------------------

void work(const char *msg, volatile char& table,  const char ball)
{
	counter_name = msg;
	signal(SIGALRM, result);
	alarm(1);
	while (true)
	{
		while (table == ball);
		table = ball;
		++counter;
	}
}

//---------------------------------------------------------

int main()
{
	const char PING = '1', PONG = '0';

	auto address = mmap(nullptr, 1, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
	volatile char& table = *static_cast<volatile char*>(address);
	table = PONG;

	switch(fork())
	{
		case -1: // fallo
			std::cout << "fallo en fork()!" << std::endl; 
			exit(EXIT_FAILURE); 
			break;
		case  0: // hijo
			work("pong", table, PONG);
			break;
		default: // padre
			work("ping", table, PING);
			wait(nullptr);
			break;
	}
}

//---------------------------------------------------------

