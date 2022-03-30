//---------------------------------------------------------
// medir.nulo.gb.cc
//---------------------------------------------------------

#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <benchmark/benchmark.h>

void test(benchmark::State& state)
{
	for (auto _ : state)
		switch(fork())
		{
			case -1: std::cout << "fallo en fork()!"; break;
			case  0: execl("./nulo", "./nulo", nullptr); break;
			default: wait(nullptr); break;
		};
}

BENCHMARK(test);

BENCHMARK_MAIN();

