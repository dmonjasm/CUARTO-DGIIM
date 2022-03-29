//---------------------------------------------------------
// com.thread.cc
//---------------------------------------------------------

#include <iostream>
#include <thread>

int main()
{
	const std::size_t N = 64;
	char msg[N] = "";
	
	std::thread escritor([&]
	{
		char *p = msg;
		for (char c = 'a'; c <= 'z'; ++c)
			*p++ = c;
		*p = '\0';
		std::cout << "escritor: " << msg << std::endl;
	});
	
	escritor.join();
	
	std::thread lector([&]
	{
		std::cout << "  lector: " << msg << std::endl;
	});
	
	lector.join();
}
