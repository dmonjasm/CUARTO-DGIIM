#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <random>
#include "demangle.h"

const size_t B = 2, C = 3, N = 128;

template<class T> void ijk(T a[N][N], T b[N][N], T c[N][N])
{
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j)
			for (size_t k = 0; k < N; ++k)
				a[i][j] += b[i][k] * c[k][j];
}

template<class T> void ikj(T a[N][N], T b[N][N], T c[N][N])
{
	for (size_t i = 0; i < N; ++i)
		for (size_t k = 0; k < N; ++k)
			for (size_t j = 0; j < N; ++j)
				a[i][j] += b[i][k] * c[k][j];
}

template<class T> void jik(T a[N][N], T b[N][N], T c[N][N])
{
	for (size_t j = 0; j < N; ++j)
		for (size_t i = 0; i < N; ++i)
			for (size_t k = 0; k < N; ++k)
				a[i][j] += b[i][k] * c[k][j];
}

template<class T> void jki(T a[N][N], T b[N][N], T c[N][N])
{
	for (size_t j = 0; j < N; ++j)
		for (size_t k = 0; k < N; ++k)
			for (size_t i = 0; i < N; ++i)
				a[i][j] += b[i][k] * c[k][j];
}

template<class T> void kij(T a[N][N], T b[N][N], T c[N][N])
{
	for (size_t k = 0; k < N; ++k)
		for (size_t i = 0; i < N; ++i)
			for (size_t j = 0; j < N; ++j)
				a[i][j] += b[i][k] * c[k][j];
}

template<class T> void kji(T a[N][N], T b[N][N], T c[N][N])
{
	for (size_t k = 0; k < N; ++k)
		for (size_t j = 0; j < N; ++j)
			for (size_t i = 0; i < N; ++i)
				a[i][j] += b[i][k] * c[k][j];
}

template<class T, class F> void test_function(F& f)
{
	T a[N][N], b[N][N], c[N][N];
	
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j)
		{
			a[i][j] = 0;
			b[i][j] = B;
			c[i][j] = C;
		}
	
	const size_t R = 33;
	std::chrono::duration<double, std::milli> rep[R];
	
	for (auto& r: rep)
	{
		auto start = std::chrono::high_resolution_clock::now();
		f(a, b, c);
		auto stop = std::chrono::high_resolution_clock::now();
		r = stop - start;
	}
	
	std::nth_element(rep, rep + R / 2, rep + R);
	std::cout << " " << std::fixed << std::setprecision(2) << std::setw(6)
	          << rep[R/2].count() << "ms";
	
	assert(a[N - 1][N - 1] == B * C * N * R);
};

template<class T> void test_type()
{
	std::cout << std::setw(12) << ec::demangle<T>();
	test_function<T>(ijk<T>);
	test_function<T>(ikj<T>);
	test_function<T>(jik<T>);
	test_function<T>(jki<T>);
	test_function<T>(kij<T>);
	test_function<T>(kji<T>);
	std::cout << std::endl;
}

int main()
{
	std::cout << "                ijk      ikj      jik      jki      kij      kji\n"
	          << "              -------  -------  -------  -------  -------  -------\n";
	test_type<short>();
	test_type<int>();
	test_type<long>();
	test_type<__int128>();
	test_type<float>();
	test_type<double>();
	test_type<long double>();
	test_type<__float128>();
}

