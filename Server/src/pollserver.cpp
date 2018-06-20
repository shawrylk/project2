#include "../include/pollserver.h"

void PollServer::Start(void)
{
  listen_sd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_sd < 0)
  {
    perror("socket() failed");
    exit(-1);
  }
  rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
                  (char *)&on, sizeof(on));
  if (rc < 0)
  {
    perror("setsockopt() failed");
    close(listen_sd);
    exit(-1);
  }
  rc = ioctl(listen_sd, FIONBIO, (char *)&on);
  if (rc < 0)
  {
    perror("ioctl() failed");
    close(listen_sd);
    exit(-1);
  }
  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port        = htons(SERVER_PORT);
  rc = bind(listen_sd,(struct sockaddr *)&addr, sizeof(addr));
  if (rc < 0)
  {
    perror("bind() failed");
    close(listen_sd);
    exit(-1);
  }
  rc = listen(listen_sd, 32);
  if (rc < 0)
  {
    perror("listen() failed");
    close(listen_sd);
    exit(-1);
  }
  memset(fds, 0 , sizeof(fds));
  fds[0].fd = listen_sd;
  fds[0].events = POLLIN;
  timeout = (1 * 60 * 1000);
  do
  {
    std::cout << "Waiting on poll()...\n";
    rc = poll(fds, nfds, timeout);
    if (rc < 0)
    {
      perror("  poll() failed");
      break;
    }
    if (rc == 0)
    {
      std::cout <<"  poll() timed out.  End program.\n";
      break;
    }
    current_size = nfds;
    for (i = 0; i < current_size; i++)
    {
      if(fds[i].revents == 0)
        continue;
      if(fds[i].revents != POLLIN)
      {
        std::cout <<"  Error! revents = "<< fds[i].revents << std::endl;
        end_server = TRUE;
        break;

      }
      if (fds[i].fd == listen_sd)
      {
        std::cout <<"  Listening socket is readable\n";
        do
        {
          new_sd = accept(listen_sd, NULL, NULL);
          if (new_sd < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  accept() failed");
              end_server = TRUE;
            }
            break;
          }
          std::cout <<"  New incoming connection" << new_sd << std::endl;

          if (rc < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  recv() failed");
              close_conn = TRUE;
            }
            break;
          }
          if (rc == 0)
          {
            std::cout <<"  Connection closed\n";
            close_conn = TRUE;
            break;
          }
	         pProcess(new_sd);
        } while (new_sd != -1);
      }
      else
      {
        std::cout <<"  Descriptor %d is readable "<< fds[i].fd << std::endl;
        close_conn = FALSE;
        do
        {
          rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
          if (rc < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  recv() failed");
              close_conn = TRUE;
            }
            break;
          }
          if (rc == 0)
          {
            std::cout <<"  Connection closed\n";
            close_conn = TRUE;
            break;
          }
          len = rc;
          std::cout <<"  %d bytes received "<< len << std::endl;
          rc = send(fds[i].fd, buffer, len, 0);
          if (rc < 0)
          {
            perror("  send() failed");
            close_conn = TRUE;
            break;
          }

        } while(TRUE);
        if (close_conn)
        {
          close(fds[i].fd);
          fds[i].fd = -1;
          compress_array = TRUE;
        }


      }
    }
    if (compress_array)
    {
      compress_array = FALSE;
      for (i = 0; i < nfds; i++)
      {
        if (fds[i].fd == -1)
        {
          for(j = i; j < nfds; j++)
          {
            fds[j].fd = fds[j+1].fd;
          }
          i--;
          nfds--;
        }
      }
    }

  } while (end_server == FALSE);

  for (i = 0; i < nfds; i++)
  {
    if(fds[i].fd >= 0)
      close(fds[i].fd);
  }
}

void PollServer::Stop(void)
{

}

void PollServer::pProcess(int &fd)
{
	char *pch;
	int wr = TRUE;
	std::ifstream DTBI("DTB.txt");
	std::string line;
	rc = recv(new_sd, buffer, sizeof(buffer), 0);
	pch = strtok(buffer,":");
	if (strcmp("login",pch) == 0)
	{

		if (DTBI.is_open())
		  {
			pch = strtok(NULL,"");
			while( getline( DTBI,line))
			{
				if (strcmp(line.c_str(),pch) == 0)
				{
					fds[nfds].fd = new_sd;
					fds[nfds].events = POLLIN;
					nfds++;
					std::cout <<"account connected\n";
					break;
				}
			}
		  }
		else
			std::cout << "can't open DTB";
	}
	else
	{
	if (strcmp("signin",pch) == 0)
	{
		std::ofstream DTBO;
		DTBO.open("DTB.txt", std::ofstream::app);
		pch = strtok(NULL,"");
		while( getline( DTBI,line))
			{
				if (strcmp(line.c_str(),pch) == 0)
				{
					std::cout <<"account existed\n";
					wr = FALSE;
				}
			}
		if (wr == TRUE)
		{
			DTBO << std::endl << pch;
			std::cout <<"account created\n";
			DTBO.close();
		}

	}
	DTBI.close();
	}
}
