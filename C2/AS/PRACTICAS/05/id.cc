#include <pthread.h>
#include <iomanip>
#include <iostream>
#include <thread>
#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>
#include <boost/thread/thread.hpp>

int main()
{
	int GAP = 32;
	std::cout << std::setw(GAP) 
	          << "pthread_self() = " 
	          << pthread_self() << std::endl
	          << std::setw(GAP) 
	          << "std::this_thread::get_id() = " 
	          << std::this_thread::get_id() << std::endl
	          << std::setw(GAP) 
	          << "boost::this_thread::get_id() = " 
	          << boost::this_thread::get_id() << std::endl
	          << std::setw(GAP) 
	          << "boost::this_fiber::get_id() = " 
	          << boost::this_fiber::get_id() << std::endl;
}
