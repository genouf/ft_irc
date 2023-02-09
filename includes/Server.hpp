#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <cerrno>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <fcntl.h>
#include <poll.h>
#include "User.hpp"

#define MAX_QUEUED 1000

class User;

class Server
{
	typedef int(Server::*cmd_f)(std::vector<std::string>, User);
	/*	MEMBER VAR	*/
	private:
		int								_sockfd;
		struct sockaddr_in				_addr;
		std::string						_password;
		std::vector<pollfd>				_sockets;
		std::map<int, User>				_users;
		std::vector<std::string>		_nicks;
		std::map<std::string, cmd_f>	_cmd_functions;

		void									init_cmd_functions();
		/*	UTILS	*/
		void									delete_socket(int fd, int i);
		int										new_socket();
		int										new_msg(int &i);
		std::vector<std::vector<std::string> >	parsing_msg(std::string msg);
		/*	CMD	*/
		void	monitor_cmd(std::vector<std::vector<std::string> > input, int user_fd);
		int		cmd_password(std::vector<std::string> params, User user);

	public:
		/*	CONSTRUCTOR / DESTRUCTOR	*/
		Server(int port, std::string password);
		virtual ~Server();

		/*	GETTER	*/
		int		getSocket() const;

		/*	FUNCTIONS	*/
		int		run();
};

