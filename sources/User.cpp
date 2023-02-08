#include "../includes/User.hpp"

/*	CONSTRUCTOR / DESTRUCTOR	*/
User::User(int fd, struct sockaddr_in addr) : _nick(""), _user(""), _auth(false), _fd(fd)
{
	_ip = inet_ntoa(addr.sin_addr);
	char hostname[1024];
	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), hostname, sizeof(hostname), NULL, 0, 0) == 0)
		_hostname = hostname;
	else
		_hostname = "unknown";
	std::cout << "New user connected: " << _hostname << " (" << _ip << ")" << std::endl;
	return ;
}

User::~User() { return ; }

/*	GETTER	*/
std::string	User::getNick() const { return (this->_nick); }

std::string	User::getUser() const { return (this->_user); }

/*	SETTER	*/
void User::setNick(std::string input) { this->_nick = input; }

void User::setUser(std::string input) { this->_user = input; }