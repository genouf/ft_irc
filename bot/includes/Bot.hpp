#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <stdlib.h>
#include <vector>
#include <map>

class Bot
{
	private:
		int					_sockfd;
		struct sockaddr_in	_serv_addr;
		std::string			_nick;
		std::string			_serv_password;

	public:
		Bot(std::string pass);
		~Bot();

		// Connection
		void	connect_to_server();
		void	send_pass();
		void	send_nick();
		void	send_user();
		void	auth();

		// Running
		void								run();
		void								rcv_msg();
		void								monitor_cmd(std::vector<std::string> arg);
		std::map<std::string, std::string>	weather(std::string city);

		// Utils
		std::vector<std::vector<std::string> >	parsing_msg(std::string msg);
		void									pop_back_str(std::string &str);

};