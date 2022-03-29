//---------------------------------------------------------
// com.msg.cc
//---------------------------------------------------------

#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>

//---------------------------------------------------------

int main()
{
	const long MSIZE = 1, MTYPE = 1;
	const char EOS = '\0';
	const int MFLAGS = 0;
	
	struct msgbuf { long mtype; char mtext[MSIZE]; } msg = {MTYPE, EOS};
	
	int msgid = msgget(IPC_PRIVATE, 0600);
	assert(msgid != -1);
	
	switch(fork())
	{
		case -1: 
			std::cout << "fallo en fork()!"; break;
		case 0: 
			std::cout << "  hijo envía: ";
			for (char c = 'a'; c <= 'z'; ++c)
			{
				msg.mtext[0] = c;
				msgsnd(msgid, &msg, MTYPE, MFLAGS);
				std::cout << msg.mtext[0];
			}
			msg.mtext[0] = EOS; // fin de mensaje
			msgsnd(msgid, &msg, MTYPE, MFLAGS);
			break;
		default:
			std::cout << "padre recibe: ";
			do {
				msgrcv(msgid, &msg, MSIZE, MTYPE, MFLAGS);
				std::cout << msg.mtext[0];
			} while (msg.mtext[0] != EOS); // ¿fin de mensaje?
			wait(nullptr);
			break;
	}
	std::cout << std::endl;
}

//---------------------------------------------------------

