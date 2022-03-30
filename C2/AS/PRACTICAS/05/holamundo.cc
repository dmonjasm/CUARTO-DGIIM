#include <iostream>
#include <thread>

void hola()
{
	std::cout << "hola!" << std::endl;
}

auto mundo = []
{
	std::cout << "mundo!" << std::endl;
};

int main(int argc, char* argv[])
{
	std::thread fh(hola), fm(mundo);
	fh.join();
	fm.join();
}
