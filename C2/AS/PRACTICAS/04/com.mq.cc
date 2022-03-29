//---------------------------------------------------------
// com.mq.cc
//---------------------------------------------------------

#include <mqueue.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

//---------------------------------------------------------

#define check(code, error) docheck(code, \
                                   std::string(__FILE__) + ":" + std::to_string(__LINE__), \
                                   static_cast<decltype(code)>(error))

//---------------------------------------------------------

template<class T> T docheck(T code, const std::string &s, T error = -1)
{
	if (code == error)
	{
		std::cerr << "[" << getpid() << "] " << s << " code=" << code << " errno=" << errno 
		          << " --> " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	return code;
}

//---------------------------------------------------------

int main()
{
	const unsigned N = 64;
	const char EOS = '\0', *EOM = &EOS;
	
	mqd_t id = 0;
	char file[] = "/mq", msg[N];
	struct mq_attr attr = {0, 1, 1, 0}; // {flags, maxmsg, msgsize, curmsgs}
	
	switch(fork())
	{
		case -1: 
			std::cout << "fallo en fork()!";
			break;
		case 0:
			id = check(mq_open(file, O_CREAT | O_WRONLY, 0600, &attr), -1);
			std::cout << "  hijo envia: ";
			for (char c = 'a'; c <= 'z'; ++c)
			{
				check(mq_send(id, &c, 1, 0), -1);
				std::cout << c;
			}
			check(mq_send(id, EOM, 1, 0), -1); // fin de mensaje
			break;
		default:
			id = check(mq_open(file, O_CREAT | O_RDONLY, 0600, &attr), -1);
			std::cout << "padre recibe: ";
			do {
				check(mq_receive(id, msg, N, NULL), -1);
				std::cout << msg[0];
			} while (msg[0] != EOS); // ¿fin de mensaje?
			wait(nullptr);
			break;
	}
	
	std::cout << std::endl;
	mq_close(id);
}

//---------------------------------------------------------

