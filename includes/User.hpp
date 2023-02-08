#pragma once
#include <iostream>
#include "poll.h"

class User
{
	/*	MEMBER VAR	*/
	private:
		std::string		_nick;
		std::string		_user;
		bool			_auth;

	public:
		/*	CONSTRUCTOR / DESTRUCTOR	*/
		User();
		virtual ~User();

		/*	GETTER	*/
		std::string	getNick() const;
		std::string getUser() const;
		/*	SETTER	*/
		void setNick(std::string input);
		void setUser(std::string input);
};