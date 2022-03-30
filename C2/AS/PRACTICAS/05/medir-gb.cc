//-----------------------------------------------------------------------------
// medir-gb.cc
//-----------------------------------------------------------------------------

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <boost/fiber/fiber.hpp>
#include <benchmark/benchmark.h>

//-----------------------------------------------------------------------------

void usuario()
{
	boost::fibers::fiber t([]{});
	t.join();
}

void nucleo()
{
	std::thread t([]{});
	t.join();
}

void proceso()
{
	if (fork())
		wait(nullptr);
	else
		execl("nulo", "nulo", nullptr);
}

//-----------------------------------------------------------------------------

template<auto f> void test(benchmark::State& state)
{
	for (auto _ : state)
		f();
}

//-----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(test, usuario);
BENCHMARK_TEMPLATE(test,  nucleo);
BENCHMARK_TEMPLATE(test, proceso);

BENCHMARK_MAIN();

//-----------------------------------------------------------------------------
