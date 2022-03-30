//---------------------------------------------------------
// sw-t.cc
//---------------------------------------------------------

#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

//---------------------------------------------------------

bool run = true;

int main(int argc, char* argv[])
{
	std::thread t([]
	{
		std::signal(SIGUSR1, [](int){ run = false; });
		while(run) std::this_thread::yield();
	});
	
	std::this_thread::sleep_for(1us);
	
	auto start = std::chrono::high_resolution_clock::now();
	std::raise(SIGUSR1);
	t.join();
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duracion = stop - start;
	std::cout << duracion / 1ns << "ns ≈ " 
	          << duracion / 1us << "µs" << std::endl;
}

//---------------------------------------------------------

