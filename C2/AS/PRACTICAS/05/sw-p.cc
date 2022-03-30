//---------------------------------------------------------
// sw-p.cc
//---------------------------------------------------------

#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

//---------------------------------------------------------

void manejador(int)
{
	exit(0);
}

int main(int argc, char* argv[])
{
	int pid = fork();
	switch(pid)
	{
	case -1:
		std::cerr << argv[0] << ": error en fork()" << std::endl;
		break;
	case 0:
		signal(SIGUSR1, manejador);
		while(true);
		break;
	default:
		auto start = std::chrono::high_resolution_clock::now();
		kill(pid, SIGUSR1);
		wait(nullptr);
		auto stop = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duracion = stop - start;
		std::cout << duracion / 1ns << "ns ≈ " 
		          << duracion / 1us << "µs" << std::endl;
		break;
	}
}

//---------------------------------------------------------

