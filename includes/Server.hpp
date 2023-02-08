#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <cerrno>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include "User.hpp"

#define MAX_QUEUED 1000

class Server
{
	/*	MEMBER VAR	*/
	private:
		int					_sockfd;
		struct sockaddr_in	_addr;
		std::string			_password;
		std::vector<pollfd>	_sockets;
		std::map<int, User> _users;

		void	delete_socket(int fd, int i);
	public:
		/*	CONSTRUCTOR / DESTRUCTOR	*/
		Server(int port, std::string password);
		virtual ~Server();

		/*	GETTER	*/
		int getSocket() const;

		/*	FUNCTIONS	*/
		int		run();

		/*	EXCEPTIONS	*/
		class InvalidServerException
		{
			public:
				InvalidServerException(std::string error) : e(error + ": " + std::strerror(errno))
				{}

				const char *what() const throw()
				{
					return(e.c_str());
				}

			private:
				std::string e;
		};
};

