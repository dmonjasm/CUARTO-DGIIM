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

template<class T, class F> void test_type(F& f)
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
	std::cout << std::fixed << std::setprecision(2) << std::setw(8)
	          << rep[R/2].count();
	
	assert(a[N - 1][N - 1] == B * C * N * R);
};

#define test_function(f) std::cout << std::setw(8) << #f; test_type<short>(f<short>); test_type<int>(f<int>); test_type<long>(f<long>); test_type<__int128>(f<__int128>); test_type<float>(f<float>); test_type<double>(f<double>); test_type<long double>(f<long double>); test_type<__float128>(f<__float128>); std::cout << std::endl;

int main()
{
	const char *types[] = {"(ms)", "short", "int", "long", "int128", "float", "double", "ldouble", "floa128"};
	
	for (auto& i: types)
		std::cout << std::setw(8) << i;
	std::cout << std::endl;
	
	for (int i = 0; i < 9; ++i)
		std::cout << std::setw(8) << "-------";
	std::cout << std::endl;
	
	test_function(ijk);
	test_function(ikj);
	test_function(jik);
	test_function(jki);
	test_function(kij);
	test_function(kji);
}

