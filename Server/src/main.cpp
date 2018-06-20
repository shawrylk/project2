#include "../include/pollserver.h"
#include <thread>

int main(int argc, char *argv[])
{
	PollServer *serverPtr = new PollServer();
	std::thread server_th(&PollServer::Start,serverPtr);
	std::cout << "main says hello\n";
	server_th.join();
	return 0;
}
