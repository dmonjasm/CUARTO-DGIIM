#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <unistd.h>
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

int main(int argc, char **argv)
{
	const char PING = '1', PONG = '2';
	struct msgbuf msg, msgi = {PING}, msgo = {PONG};
	int msgid = msgget(IPC_PRIVATE, 0600);
	
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
				msgrcv(msgid, &msg , 0, PING, 0);
				msgsnd(msgid, &msgo, 0, 0);
				++counter;
			}
			break;
		default: // padre
			counter_name = "ping";
			signal(SIGALRM, result);
			alarm(1);
			while (true)
			{
				msgsnd(msgid, &msgi, 0 ,0);
				msgrcv(msgid, &msg , 0, PONG, 0);
				++counter;
			}
			wait(nullptr);
			break;
	}
}
