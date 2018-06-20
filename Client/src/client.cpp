#include "../include/client.h"

Client::Client()
{
  int rc;
  struct sockaddr_in addr = {0};
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = inet_addr("10.0.2.15");
  rc = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
  if (rc == 0)
	std::cout << " client connected\n";
  else
	std::cout << " client connection fails\n";
}

int Client::Signin(std::string username, std::string pass)
{
  return 0;
}

int Client::Login(std::string username, std::string pass)
{
  char msg[MAXBUF];
  sprintf (msg, "login:shawry.lk:2203");
  	n = write(sockfd, msg, strlen(msg));	/* Send message */
  return n;
}


 

