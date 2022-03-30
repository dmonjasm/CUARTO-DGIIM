//-----------------------------------------------------------------------------
// fib-u.cc
//-----------------------------------------------------------------------------

#include <iostream> // cout endl
#include <sstream>  // istringstream
#include <boost/fiber/all.hpp> // async

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

template<class T> T fib(T n)
{
	if (n < 2)
		return n;
	else
		return boost::fibers::async(fib<T>, n - 1).get() + 
		       boost::fibers::async(fib<T>, n - 2).get();
}

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

