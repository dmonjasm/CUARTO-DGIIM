#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

template<class T> T fib(T n)
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

int main(int argc, char *argv[])
{
	if (argc < 2)
		throw invalid_argument("necesito un número como parámetro");

	istringstream iss(argv[1]);
	unsigned n;
	iss >> n;
	if (!iss)
		throw invalid_argument("el parámetro no es un número válido");

	cout << argv[0] << "(" << argv[1] << ") = " << fib(n) << endl;
}
