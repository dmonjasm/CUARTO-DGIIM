//---------------------------------------------------------
// sw-f.cc
//---------------------------------------------------------

#include <chrono>
#include <csignal>
#include <iostream>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>

using namespace std::chrono_literals;

//---------------------------------------------------------

bool run = true;

int main(int argc, char* argv[])
{
	boost::fibers::fiber t([]
	{
		std::signal(SIGUSR1, [](int){ run = false; });
		while(run) boost::this_fiber::yield();
	});
	
	boost::this_fiber::sleep_for(1us);
	
	auto start = std::chrono::high_resolution_clock::now();
	std::raise(SIGUSR1);
	t.join();
	auto stop = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duracion = stop - start;
	std::cout << duracion / 1ns << "ns ≈ " 
	          << duracion / 1us << "µs" << std::endl;
}

//---------------------------------------------------------

