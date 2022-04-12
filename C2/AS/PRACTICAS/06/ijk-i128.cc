#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <random>

const int B = 2, C = 3, N = 128;

__int128 a[N][N], b[N][N], c[N][N];

void ijk()
{
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			for (int k = 0; k < N; ++k)
				a[i][j] += b[i][k] * c[k][j];
}

void ikj()
{
	for (int i = 0; i < N; ++i)
		for (int k = 0; k < N; ++k)
			for (int j = 0; j < N; ++j)
				a[i][j] += b[i][k] * c[k][j];
}

void jik()
{
	for (int j = 0; j < N; ++j)
		for (int i = 0; i < N; ++i)
			for (int k = 0; k < N; ++k)
				a[i][j] += b[i][k] * c[k][j];
}

void jki()
{
	for (int j = 0; j < N; ++j)
		for (int k = 0; k < N; ++k)
			for (int i = 0; i < N; ++i)
				a[i][j] += b[i][k] * c[k][j];
}

void kij()
{
	for (int k = 0; k < N; ++k)
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				a[i][j] += b[i][k] * c[k][j];
}

void kji()
{
	for (int k = 0; k < N; ++k)
		for (int j = 0; j < N; ++j)
			for (int i = 0; i < N; ++i)
				a[i][j] += b[i][k] * c[k][j];
}

template<class F> void test(F& f, const char *msg)
{
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			a[i][j] = 0;
	
	const int R = 33;
	std::chrono::duration<double, std::milli> rep[R];
	
	for (auto& r: rep)
	{
		auto start = std::chrono::high_resolution_clock::now();
		f();
		auto stop = std::chrono::high_resolution_clock::now();
		r = stop - start;
	}
	
	std::nth_element(rep, rep + R / 2, rep + R);
	std::cout << msg << ":" << std::fixed << std::setprecision(2) << std::setw(6)
	          << rep[R/2].count() << "ms" << std::endl;
	
	assert(a[N - 1][N - 1] == B * C * N * R);
}

int main()
{
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
		{
			b[i][j] = B;
			c[i][j] = C;
		}
	
	test(ijk, "ijk");
	test(ikj, "ikj");
	test(jik, "jik");
	test(jki, "jki");
	test(kij, "kij");
	test(kji, "kji");
}

