#include <iostream>
#include <thread>

int main()
{
	auto hola = []
	{
		std::cout << "[" 
		          << std::this_thread::get_id() 
		          << "]: hola!" 
		          << std::endl;
	};
	
	hola();
	std::thread t(hola);
	t.join();
}
