//-----------------------------------------------------------------------------

#include <cassert>
#include <mutex>
#include <random>
#include <vector>

//-----------------------------------------------------------------------------

using type = long long unsigned;
const type MAX = 93; // fib(MAX + 1) doesn't fit in type
const type VALUE = 12200160415121876738ull; // fib(MAX) == VALUE

//-----------------------------------------------------------------------------

template <typename T> T fib1(T n)
{
	if (n < 2)
		return n;
	else
		return fib1(n - 2) + fib1(n - 1);
}

//-----------------------------------------------------------------------------

template <typename T> T fib2(T n)
{
	T i = 1, j = 0;
	for (T k = 0; k < n; ++k)
	{
		T t = i + j;
		i = j;
		j = t;
	}
	return j;
}

//-----------------------------------------------------------------------------

template<class T> T fib3(T n)
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

//-----------------------------------------------------------------------------

template<class T> T fib4(T n)
{
	static std::vector<T> solutions = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
	
	try
	{
		return solutions.at(n);
	}
	catch (...)
	{
		std::size_t size = solutions.size();
		solutions.resize(n + 1);
		for (auto i = solutions.begin() + size; i != solutions.end(); ++i)
			*i = *(i - 2) + *(i - 1);
	}
	
	return solutions[n];
}

//-----------------------------------------------------------------------------

template<class T> T fib5(T n)
{
	static std::vector<T> solutions = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
	std::size_t size = solutions.size();
	
	if (n >= size)
	{
		solutions.resize(n + 1);
		for (auto i = solutions.begin() + size; i != solutions.end(); ++i)
			*i = *(i - 2) + *(i - 1);
	}
	
	return solutions[n];
}

//-----------------------------------------------------------------------------

template<class T> T fib6(T n)
{
	static std::once_flag flag;
	std::call_once(flag, [&]{ fib5(MAX); });
	
	return fib5(n);
}

//-----------------------------------------------------------------------------

template<typename F> void test(F& f)
{
	assert(f(MAX) == VALUE);
	
	std::default_random_engine eng(0);
	
	for (int i = 0; i < 10000; ++i)
		f(eng() % (MAX + 1));
}

//-----------------------------------------------------------------------------

int main()
{
//	test(fib1<type>);
	test(fib2<type>);
	test(fib3<type>);
	test(fib4<type>);
	test(fib5<type>);
	test(fib6<type>);
}

//-----------------------------------------------------------------------------
