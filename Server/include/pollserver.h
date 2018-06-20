#ifndef __POLL_SERVER__
#define __POLL_SERVER__

#pragma -std=c++11

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fstream>

#define SERVER_PORT  12345

#define TRUE             1
#define FALSE            0

class PollServer
{
	private:
		int    len, rc, on = 1;
		int    listen_sd = -1, new_sd = -1;
		int    desc_ready, end_server = FALSE, compress_array = FALSE;
		int    close_conn;
		char   buffer[80];
		struct sockaddr_in   addr;
		int    timeout;
		struct pollfd fds[200];
		int    nfds = 1, current_size = 0, i, j;
		void pProcess(int &fd);
	public:
		void Start(void);
		void Stop(void);
};

#endif
