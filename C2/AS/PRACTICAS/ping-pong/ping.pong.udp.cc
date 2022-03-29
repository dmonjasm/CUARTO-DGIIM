// https://www.prodevelopertutorial.com/linux-system-programming-creating-udp-sockets/

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>

//---------------------------------------------------------

std::size_t counter = 0;
std::string counter_name;
int cliente = 0, servidor = 0; // descriptores de ficheros de los sockets

//---------------------------------------------------------

void result(int)
{
	std::cout << counter_name << " = " << counter << std::endl;
	close(cliente);
	close(servidor);
	exit(EXIT_SUCCESS);
}

//---------------------------------------------------------

int main()
{
	const char PING = '1', PONG = '2';
	const int PUERTO_PADRE = 8888, PUERTO_HIJO = 8889;
	char buffer;
	
	struct sockaddr_in si_padre = {AF_INET, htons(PUERTO_PADRE), INADDR_ANY},
	                   si_hijo  = {AF_INET, htons(PUERTO_HIJO ), INADDR_ANY},
	                   si_otro;
	socklen_t len = sizeof si_otro;
	
	switch(fork())
	{
		case -1: // fallo en el fork
		{
			std::cout << "fallo en fork()!" << std::endl; 
			exit(EXIT_FAILURE); 
			break;
		}
		case 0: // hijo
		{
			counter_name = "pong";
			signal(SIGALRM, result);
			alarm(1);
			
			servidor = socket(PF_INET, SOCK_DGRAM, 0);
			bind(servidor, (struct sockaddr*)&si_hijo, (socklen_t)sizeof si_hijo);
			
			cliente = socket(PF_INET, SOCK_DGRAM, 0);
			
			while (true)
			{
				sendto(cliente, &PING, 1, 0, (struct sockaddr*)&si_padre, (socklen_t)sizeof si_padre);
				recvfrom(servidor, &buffer, 1, 0, (struct sockaddr*)&si_otro, &len);
				++counter;
			}
			break;
		}
		default: // padre
		{
			counter_name = "ping";
			signal(SIGALRM, result);
			alarm(1);
			
			servidor = socket(PF_INET, SOCK_DGRAM, 0);
			bind(servidor, (struct sockaddr*)&si_padre, (socklen_t)sizeof si_padre);
			
			cliente = socket(PF_INET, SOCK_DGRAM, 0);
			
			while (true)
			{
				recvfrom(servidor, &buffer, 1, 0, (struct sockaddr*)&si_otro, &len);
				++counter;
				sendto(cliente, &PONG, 1, 0, (struct sockaddr*)&si_hijo, (socklen_t)sizeof si_hijo);
			}
			wait(NULL);
			break;
		}
	}
}

