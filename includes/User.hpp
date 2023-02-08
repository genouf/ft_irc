#include <iostream>
#include "poll.h"

class User
{
	/*	MEMBER VAR	*/
	private:
		std::string		_nick;
		std::string		_user;
		bool			_status;

	public:
		/*	CONSTRUCTOR / DESTRUCTOR	*/
		User(std::string nick, std::string name);
		virtual ~User();

		/*	GETTER	*/
		std::string	getNick() const;
		std::string getName() const;
		/*	SETTER	*/
		void setNick(std::string input);
		void setName(std::string input);
};