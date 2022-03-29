// https://www.prodevelopertutorial.com/linux-system-programming-creating-udp-sockets/
//DANIEL MONJAS MIGUÉLEZ, 70274432W, 4ºDGIIM
//ARQUITECTURA DE SISTEMAS
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
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace boost::asio;

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
	//Cambiamos el char por un string para poder utilizar la función buffer
	const std::string PING = "1", PONG = "2";
	const int PUERTO_PADRE = 8888, PUERTO_HIJO = 8889;
	//Se cambia el tipo de buffer para la recpeción de mensajes
    boost::array<char, 1024> recv_buffer;
	
	//Se crea un objeto io_service que contiene la funcionalidad básica de las operaciones de entrada y salida
    io_service io_service;

	//Creamos dos sockets. Cada uno se enlazará con una hebra para la recepción y envío de mensajes.
    ip::udp::socket socket_padre(io_service);
    ip::udp::socket socket_hijo(io_service);

	//Se inician los sockets con el protocolo IPv4 UDP
    socket_padre.open(ip::udp::v4());
    socket_hijo.open(ip::udp::v4());

	//Se inicializan dos endpoint, uno para cada hebra, si se quisieran más hebras se deberían crear más endpoint
    ip::udp::endpoint padre = ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), PUERTO_PADRE);
    ip::udp::endpoint hijo = ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), PUERTO_HIJO);
	
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

			//Se enlaza el socket_hijo al endpoint inicializado al puerto del hijo para la recepción de mensajes desde el padre
            socket_hijo.bind(hijo);
			signal(SIGALRM, result);
			alarm(1);
			
			while (true)
			{
				socket_padre.send_to(boost::asio::buffer(PING),padre);
				socket_hijo.receive_from(boost::asio::buffer(recv_buffer), padre);
				++counter;
			}
			break;
		}
		default: // padre
		{
			counter_name = "ping";

			//Se enlaza el socket_padre al endpoint inicializado al puerto del padre para la recepción de mensajes desde el hijo
            socket_padre.bind(padre);
			signal(SIGALRM, result);
			alarm(1);
			
			while (true)
			{   
				socket_padre.receive_from(boost::asio::buffer(recv_buffer), hijo);
				++counter;
				socket_hijo.send_to(boost::asio::buffer(PONG),hijo);
			}
			wait(NULL);
			break;
		}
	}
}