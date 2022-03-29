//---------------------------------------------------------
// com.wait.cc
//---------------------------------------------------------

#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

//---------------------------------------------------------

int main()
{
	switch(fork())
	{
		case -1:
			std::cout << "fallo en fork()!";
			break;
		case 0:
			std::cout << "[" << getpid() << "] hijo  --> " 
			          << 123 << std::endl;
			exit(123);
			break;
		default:
			int msg = 0;
			wait(&msg);
			std::cout << "[" << getpid() << "] padre --> " 
			          << WEXITSTATUS(msg) << std::endl;
			break;
	}
}

//---------------------------------------------------------

