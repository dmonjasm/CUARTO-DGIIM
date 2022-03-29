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
int partner = 0;

//---------------------------------------------------------

void result(int)
{
	std::cout << counter_name << " = " << counter << std::endl;
	close(partner);
	exit(EXIT_SUCCESS);
}

//---------------------------------------------------------

int main()
{
	const char PING = '1', PONG = '2';
	const int PORT = 8888;
	
	int listenfd = 0;
	char c;
	
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);
	
	switch(fork())
	{
		case -1: // Fallo en el fork
		{
			std::cout << "fallo en fork()!" << std::endl; 
			exit(EXIT_FAILURE); 
			break;
		}
		case 0: // Hijo
		{
			counter_name = "pong";
			signal(SIGALRM, result);
			alarm(1);
			
			// Creamos el socket vacio
			partner = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			
			while (connect(partner, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1);
			
			while (true)
			{
				write(partner, &PING, 1);
				read(partner, &c, 1);
				++counter;
			}
			break;
		}
		default: // Padre
		{
			counter_name = "ping";
			signal(SIGALRM, result);
			alarm(1);
			
			// Crear el socket vario
			listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			
			// Asignamos los datos al socket
			bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
			
			//Si hay error, salir
			if (listenfd == -1)
				exit(EXIT_FAILURE);
			
			// Inicializamos la conexion al socket
			listen(listenfd, 3);
			partner = accept(listenfd, (struct sockaddr*)NULL, NULL);
			//Si hay error, salir
			if (partner == -1)
			{
				close(partner);
				exit(EXIT_FAILURE);
			}
			
			while (true)
			{
				read(partner, &c, 1);
				write(partner, &PONG, 1);
				++counter;
			}
			wait(NULL);
			break;
		}
	}
}

