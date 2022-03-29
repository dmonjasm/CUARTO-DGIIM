//---------------------------------------------------------
// ping.pong.mq.cc
//---------------------------------------------------------

#include <mqueue.h>

#include <atomic>
#include <cassert>
#include <cerrno>
#include <chrono>
#include <cstring>
#include <future>
#include <iostream>
#include <string>
#include <thread>

//---------------------------------------------------------

using namespace std::chrono_literals;

//---------------------------------------------------------

#define check(code) docheck(code, std::string(__FILE__) + ":" + std::to_string(__LINE__))

//---------------------------------------------------------

int docheck(int code, const std::string &s)
{
	if (code == -1)
	{
		std::cerr << s << " code=" << code << " errno=" << errno 
		          << " --> " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	return code;
}

//---------------------------------------------------------

int main(int argc, char *argv[])
{
	const unsigned N = 64;
	const char PING[N] = "1", PONG[N] = "0";
	std::atomic<bool> end(false);
	struct mq_attr attr = {0, 1, N, 0}; // {flags, maxmsg, msgsize, curmsgs}

	mqd_t mq_ping = check(mq_open("/ping", O_CREAT | O_RDWR, 0600, &attr));
	mqd_t mq_pong = check(mq_open("/pong", O_CREAT | O_RDWR, 0600, &attr));

	auto pong = std::async(std::launch::async, [&]
	{
		int pong = 0;
		char msg[N] = "";
		
		while(!end)
		{
			check(mq_send(mq_ping, PING, 0, 0));
			check(mq_receive(mq_pong, msg, N, NULL));
			++pong;
		}
		check(mq_send(mq_ping, PING, 0, 0));
		
		std::cout << "pong = " + std::to_string(pong) + "\n";
	});

	auto ping = std::async(std::launch::async, [&]
	{
		int ping = 0;
		char msg[N] = "";
		
		while(!end)
		{
			check(mq_receive(mq_ping, msg, N, NULL));
			++ping;
			check(mq_send(mq_pong, PONG, 0, 0));
		}
		
		std::cout << "ping = " + std::to_string(ping) + "\n";
	});

	std::this_thread::sleep_for(1s);
	end = true;

	ping.wait(); 
	pong.wait();

	mq_unlink("/ping");
	mq_unlink("/pong");
}

//---------------------------------------------------------

