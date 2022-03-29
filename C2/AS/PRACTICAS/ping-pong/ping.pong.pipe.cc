//---------------------------------------------------------
// ping.pong.pipe.cc
//---------------------------------------------------------

#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <string>

//---------------------------------------------------------

std::size_t counter = 0;
std::string counter_name;

//---------------------------------------------------------

void result(int)
{
	std::cout << counter_name << " = " << counter << std::endl;
	exit(EXIT_SUCCESS);
}

//---------------------------------------------------------

int main()
{
	const char PING = '1', PONG = '0';
	const int RD = 0, WR = 1;
	
	int p2c[2], c2p[2];
	pipe(c2p); // child to parent pipe
	pipe(p2c); // parent to child pipe
	char c;
	
	switch(fork())
	{
		case -1: // fallo
			std::cout << "fallo en fork()!" << std::endl; 
			exit(EXIT_FAILURE); 
			break;
		case  0: // hijo
			counter_name = "pong";
			signal(SIGALRM, result);
			alarm(1);
			while (true)
			{
				read(p2c[RD], &c, 1);
				write(c2p[WR], &PONG, 1);
				++counter;
			}
			break;
		default: // padre
			counter_name = "ping";
			signal(SIGALRM, result);
			alarm(1);
			while (true)
			{
				write(p2c[WR], &PING, 1);
				read(c2p[RD], &c, 1);
				++counter;
			}
			wait(nullptr);
			break;
	}
}

//---------------------------------------------------------

