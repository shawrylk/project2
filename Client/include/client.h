#ifndef __CLIENT__
#define __CLIENT__

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

#define MAXBUF 256
 
class Client
{
	private:
		char buffer[MAXBUF];
		 int fds[5];
		  char *ipaddr[5];
		  struct sockaddr_in addr;
		  struct sockaddr_in client;
		  struct pollfd pollfds[5];
		  int addrlen, n,i,max=0;;
		  int sockfd, commfd;
		  fd_set rset;
	public:
		Client();
		int Signin(std::string username, std::string pass);
		int Login(std::string username, std::string pass);
};
#endif
