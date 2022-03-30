//-----------------------------------------------------------------------------
// medir.cc
//-----------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <boost/fiber/fiber.hpp>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;
using namespace std::chrono;

//-----------------------------------------------------------------------------

#define check(code) docheck(code, __FILE__, __LINE__)

int docheck(int error, const char *file, int line)
{
	if (error == -1)
	{
		cerr << file << ":" << line << ": " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
	return error;
}

//-----------------------------------------------------------------------------

void usuario()
{
	boost::fibers::fiber t([]{});
	t.join();
}

void nucleo()
{
	thread t([]{});
	t.join();
}

void proceso()
{
	switch(check(fork()))
	{
		case 0:  check(execl("nulo", "nulo", nullptr)); break;
		default: wait(nullptr);                         break;
	}
}

//-----------------------------------------------------------------------------

template<class F> void test(F& f, const char *msg)
{
	const int n = 999, w = 7;
	duration<double, micro> rep[n];

	for (auto& r: rep)
	{
		auto start = high_resolution_clock::now();
		f();
		auto stop = high_resolution_clock::now();
		r = (stop - start);
	}
	
	nth_element(rep, rep + n / 2, rep + n);
	
	cout << msg 
	     << "  minimo: " << fixed << setprecision(2) << setw(w) 
	     << min_element(rep, rep + n)->count() << "µs"
	     << "  mediana: " << fixed << setprecision(2) << setw(w) 
	     << rep[n/2].count() << "µs"
	     << "  máximo: " << fixed << setprecision(2) << setw(w) 
	     << max_element(rep, rep + n)->count() << "µs" << endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	test(usuario, "hebra usuario nula:");
	test( nucleo, " hebra nucleo nula:");
	test(proceso, "      proceso nulo:");
}

//-----------------------------------------------------------------------------
