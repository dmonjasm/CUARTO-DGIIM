//---------------------------------------------------------
// com.pipe.cc
//---------------------------------------------------------

#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

int main()
{
	const char EOS = '\0';
	const int RD = 0, WR = 1;
	
	int c2p[2];
	
	pipe(c2p); // child to parent pipe
	
	switch(fork())
	{
		case -1: // fallo
			std::cout << "fallo en fork()!" << std::endl; 
			exit(EXIT_FAILURE); 
			break;
		case  0: // hijo
			std::cout << "hijo escribe: ";
			for (char c = 'a'; c <= 'z'; ++c)
			{
				write(c2p[WR], &c, 1);
				std::cout << c;
			}
			write(c2p[WR], &EOS, 1);
			break;
		default: // padre
			char c;
			std::cout << "   padre lee: ";
			do {
				read(c2p[RD], &c, 1);
				if (c != '\0') std::cout << c;
			} while (c != '\0');
			wait(nullptr);
			break;
	}
	std::cout << std::endl;
}

