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
		pollfd				_fd;
		std::string			_ip;
		std::string			_hostname;
		

	public:
		/*	CONSTRUCTOR / DESTRUCTOR	*/
		User(pollfd fd, struct sockaddr_in addr);
		virtual ~User();

		/*	GETTER	*/
		std::string	getNick() const;
		std::string getUser() const;
		int 		getAut() const ;
		pollfd 		getFd() const;

		/*	SETTER	*/
		void setNick(std::string input);
		void setUser(std::string input);
		void setAut(bool input);
};