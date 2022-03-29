//---------------------------------------------------------
// ping.pong.thread.cc
//---------------------------------------------------------

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

//---------------------------------------------------------

using namespace std::chrono_literals;

//---------------------------------------------------------

const char PING = '1', PONG = '0';
std::atomic<bool> end(false);
std::atomic<char> table(PONG);

//---------------------------------------------------------

void work(const char *msg, const char ball)
{
	std::size_t counter = 0;
	while(!end)
	{
		while(table == ball);
		table = ball;
		++counter;
	}
	table = ball;
	std::cout << std::string(msg) + " = " + std::to_string(counter) + "\n";
}

//---------------------------------------------------------

int main()
{
	std::thread pingger(work, "ping", PING), pongger(work, "pong", PONG);
	
	std::this_thread::sleep_for(1s);
	end = true;
	
	pingger.join();
	pongger.join();
}

//---------------------------------------------------------

