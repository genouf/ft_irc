#pragma once
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string>
#include <poll.h>
#include <ctime>

enum status {
	UNKNOWN,
	AUTHENTIFICATED,
	CONNECTED,
	DISCONNECTED
};

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
		std::string			_nick;
		std::string			_username;
		std::string			_realname;
		enum status			_status;
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
		pollfd		getPollFd() const;
		int 		getFd() const;
		std::string	getIp() const;
		bool		getOp() const;

		/*	SETTER	*/
		void setNick(std::string input);
		void setUsername(std::string input);
		void setRealname(std::string input);
		void setStatus(enum status input);
		void setIp(std::string ip);
		void setOp(bool input);

		bool	isAut() const;

		auth_ok				auth_ok;
		pinged				ping_info;
};
