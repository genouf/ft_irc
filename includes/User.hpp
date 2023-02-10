#pragma once
#include <iostream>
#include "Server.hpp"
#include "poll.h"

class User
{
	private:
		/*	PRIVATE STRUCT */
		struct auth_ok 
		{
			auth_ok()
			{
				this->pass = false;
				this->nick = false;
				this->user = false;
			}
			virtual ~auth_ok() {}

			bool	authentificated()
			{
				if (this->pass == true && this->nick == true && this->user == true)
					return (true);
				return (false);
			}

			bool	pass;
			bool	nick;
			bool	user;
		};

		/*	PRIVATE MEMBER VAR	*/
		std::string			_nick;
		std::string			_user;
		bool				_auth;
		pollfd				_fd;
		std::string			_ip;
		std::string			_hostname;

	public:
		/*	CONSTRUCTOR / DESTRUCTOR	*/
		User();
		User(pollfd fd, struct sockaddr_in addr);
		virtual ~User();

		/*	GETTER	*/
		std::string	getNick() const;
		std::string getUser() const;
		int 		getAut() const ;
		pollfd		getPollFd() const;
		int 		getFd() const;

		/*	SETTER	*/
		void setNick(std::string input);
		void setUser(std::string input);
		void setAut(bool input);

		auth_ok				_auth_ok;
};
