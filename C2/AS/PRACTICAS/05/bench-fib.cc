#include <future>
#include <map>
#include <ostream>
#include <random>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <benchmark/benchmark.h>
#include <boost/fiber/all.hpp>

template<class T> T fib(T n)
{
	if (n < 2)
		return n;
	else
		return fib(n - 1) + fib(n - 2);
}

template<class T> T fib_iter(T n)
{
	T prev = 1, curr = 1, next = 1;
	for (T i = 3; i <= n; ++i)
	{
		next = curr + prev;
		prev = curr;
		curr = next;
	}
	return next;
}

template<class T> T fib_map(T n)
{
	static std::map<T, T> solutions = {{0, 0}, {1, 1}};

	if (solutions.find(n) == solutions.end())
		solutions[n] = fib_map(n - 2) + fib_map(n - 1);

	return solutions[n];
}

template<class T> T fib_hash(T n)
{
	static std::unordered_map<T, T> solutions = {{0, 0}, {1, 1}};
	try
	{
		return solutions.at(n);
	}
	catch(std::out_of_range& e)
	{
		return solutions[n] = fib_hash(n - 2) + fib_hash(n - 1);
	}
}

template<class T> T fib_hash2(T n)
{
	static std::unordered_map<T, T> solutions = {{0, 0}, {1, 1}};

	if (solutions.find(n) == solutions.end())
		solutions[n] = fib_hash2(n - 2) + fib_hash2(n - 1);

	return solutions[n];
}

template<class T> T fib_vector(T n)
{
	static std::vector<T> solutions = {0, 1};
	static size_t last = 1;
	
	while (n > last++)
		solutions.push_back(solutions[last - 2] + solutions[last - 1]);
	
	return solutions[n];
}

template<class T> T fib_vector2(T n)
{
	static std::vector<T> solutions = {0, 1};
	std::size_t last = solutions.size();
	
	if (last <= n)
	{
		solutions.resize(n + 1);
		for (auto i = solutions.begin() + last; i != solutions.end(); ++i)
			*i = *(i - 2) + *(i - 1);
	}
	
	return solutions[n];
}

template<class T> T fib_array(T n)
{
	static const size_t N = 1000000;
	static size_t last = 1;
	static T solutions[N] = {0, 1};
	
	if (n > last)
	{
		solutions[n] = fib_array(n - 2) + fib_array(n - 1);
		last = n;
	}
	
	return solutions[n];
}

template<class T> T fib_array2(T n)
{
	static const size_t N = 1000000;
	static size_t last = 1;
	static T solutions[N] = {0, 1};
	
	while (n > last++)
		solutions[last] = solutions[last - 2] + solutions[last - 1];
	
	return solutions[n];
}

template<class T> T fib_u(T n)
{
	if (n < 2)
		return n;
	else
		return boost::fibers::async(fib_u<T>, n - 1).get() + 
		       boost::fibers::async(fib_u<T>, n - 2).get();
}

template<class T> T fib_n(T n)
{
	if (n < 2)
		return n;
	else
		return std::async(fib_n<T>, n - 1).get() + std::async(fib_n<T>, n - 2).get();
}

std::random_device device;
std::default_random_engine engine(device());
std::uniform_int_distribution<unsigned> distribution(0, 30);
auto r = std::bind(distribution, engine);

template<auto f> void test(benchmark::State& state)
{
	unsigned t = 0;
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(t += f(r()));
	}
	std::ostream cnull(0);
	cnull << t;
}

//// Register the function as a benchmark
//BENCHMARK_TEMPLATE(test, fib<unsigned>);
//BENCHMARK_TEMPLATE(test, fib_iter<unsigned>);
//BENCHMARK_TEMPLATE(test, fib_map<unsigned>);
//BENCHMARK_TEMPLATE(test, fib_hash<unsigned>);
//BENCHMARK_TEMPLATE(test, fib_hash2<unsigned>);
//BENCHMARK_TEMPLATE(test, fib_vector<unsigned>);
//BENCHMARK_TEMPLATE(test, fib_vector2<unsigned>);
//BENCHMARK_TEMPLATE(test, fib_array<unsigned>);
//BENCHMARK_TEMPLATE(test, fib_array2<unsigned>);
//BENCHMARK_TEMPLATE(test, fib_u<unsigned>); // ->UseRealTime();
//BENCHMARK_TEMPLATE(test, fib_n<unsigned>); // ->UseRealTime();

template<class T> void register_benchmark()
{
	BENCHMARK_TEMPLATE(test, fib<T>);
	BENCHMARK_TEMPLATE(test, fib_iter<T>);
	BENCHMARK_TEMPLATE(test, fib_map<T>);
	BENCHMARK_TEMPLATE(test, fib_hash<T>);
	BENCHMARK_TEMPLATE(test, fib_hash2<T>);
	BENCHMARK_TEMPLATE(test, fib_vector<T>);
	BENCHMARK_TEMPLATE(test, fib_vector2<T>);
	BENCHMARK_TEMPLATE(test, fib_array<T>);
	BENCHMARK_TEMPLATE(test, fib_array2<T>);
//	BENCHMARK_TEMPLATE(test, fib_u<T>); // ->UseRealTime();
//	BENCHMARK_TEMPLATE(test, fib_n<T>); // ->UseRealTime();
}

struct Init
{
	Init()
	{
		register_benchmark<unsigned int>(); 
	}
} init;

// Run the benchmark
BENCHMARK_MAIN();

