#include <iostream>
#include <thread>

void codigo()
{
	std::cout << "[" << std::this_thread::get_id() 
	          << "]: hola" << std::endl;
}

int main()
{
	//codigo();
	std::thread t(codigo);
	t.join();
}
