#pragma once
#include <iostream>
#include "Server.hpp"
#include "poll.h"

class User
{
	/*	MEMBER VAR	*/
	private:
		std::string			_nick;
		std::string			_user;
		bool				_auth;
		int					_fd;
		std::string			_ip;
		std::string			_hostname;
		

	public:
		/*	CONSTRUCTOR / DESTRUCTOR	*/
		User(int fd, struct sockaddr_in addr);
		virtual ~User();

		/*	GETTER	*/
		std::string	getNick() const;
		std::string getUser() const;
		/*	SETTER	*/
		void setNick(std::string input);
		void setUser(std::string input);
};