#pragma once
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string>
#include <poll.h>
#include <ctime>
#include <vector>

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

		struct pinged
		{
			pinged()
			{
				this->last_ping = clock();
				this->responded = true;
			}
			~pinged() {}

			clock_t 	last_ping;
			bool		responded;
			std::string	token;
		};

		/*	PRIVATE MEMBER VAR	*/
		std::string			_input;
		std::vector<std::string> _inputs;
		std::string			_nick;
		std::string			_username;
		std::string			_realname;
		bool				_auth;
		pollfd				_fd;
		std::string			_ip;
		std::string			_hostname;
		bool				_op;

	public:
		/*	CONSTRUCTOR / DESTRUCTOR	*/
		User();
		User(pollfd fd, struct sockaddr_in addr);
		virtual ~User();

		/*	GETTER	*/
		std::string	getNick() const;
		std::string getUsername() const;
		std::string getRealname() const;
		int 		getAut() const ;
		pollfd		getPollFd() const;
		int 		getFd() const;
		std::string	getIp() const;
		bool		getOp() const;
		std::string &get_input();

		/*	SETTER	*/
		void setNick(std::string input);
		void setUsername(std::string input);
		void setRealname(std::string input);
		void setAut(bool input);
		void setIp(std::string ip);
		void setOp(bool input);

		auth_ok				auth_ok;
		pinged				ping_info;
};
