#include <iostream>
#include <boost/fiber/fiber.hpp>

void hola() { std::cout << "hola!" << std::endl; }

auto mundo = []{ std::cout << "mundo!" << std::endl; };

int main(int argc, char* argv[])
{
	boost::fibers::fiber fh(hola), fm(mundo);
	fh.join();
	fm.join();
}
