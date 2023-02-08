#include <iostream>
#include "poll.h"

class User
{
	/*	MEMBER VAR	*/
	private:
		std::string		_nick;
		std::string		_name;
		struct pollfd	_socket;

	public:
		/*	CONSTRUCTOR / DESTRUCTOR	*/
		User(struct pollfd socket);
		virtual ~User();

		/*	GETTER	*/
		std::string	getNick() const;
		std::string getName() const;
		/*	SETTER	*/
		void setNick(std::string input);
		void setName(std::string input);
};