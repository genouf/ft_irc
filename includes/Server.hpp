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
#include <string>
#include <algorithm>
#include <sstream>
#include <fcntl.h>
#include <poll.h>
#include "User.hpp"
#include "Channel.hpp"

#define MAX_QUEUED 1000

class User;
class Channel;

class Server
{
	typedef int(Server::*cmd_f)(std::vector<std::string>, User &);
	/*	MEMBER VAR	*/
	private:
		int								_sockfd;
		struct sockaddr_in				_addr;
		std::string						_password;
		std::vector<pollfd>				_sockets;
		std::map<int, User>				_users;
		std::vector<std::string>		_nicks;
		std::map<std::string, cmd_f>	_cmd_functions;
		std::map<std::string, Channel>	_channels;

		void									init_cmd_functions();
		/*	UTILS	*/
		void									send_client(std::string msg, int fd);
		void									delete_socket(pollfd fd);
		void									disconnect(User user);
		int										new_socket();
		int										new_msg(int &i);
		std::vector<std::vector<std::string> >	parsing_msg(std::string msg);
		/*	CMD	*/
		bool	check_pass(std::vector<std::vector<std::string> > input);
		void	monitor_cmd(std::vector<std::vector<std::string> > input, int user_fd);
		int		cmd_password(std::vector<std::string> params, User &user);
		int		cmd_nick(std::vector<std::string> params, User &user);
		int		cmd_user(std::vector<std::string> params, User &user);

		// Channel
		int		cmd_list(std::vector<std::string> params, User &user);
		int		cmd_join(std::vector<std::string> params, User &user);
		int		cmd_part(std::vector<std::string> params, User &user);
		int		cmd_topic(std::vector<std::string> params, User &user);

	public:
		/*	CONSTRUCTOR / DESTRUCTOR	*/
		Server(int port, std::string password);
		virtual ~Server();

		/*	GETTER	*/
		int		getSocket() const;

		/*	FUNCTIONS	*/
		int		run();
};

