#include "../includes/User.hpp"

/*	CONSTRUCTOR / DESTRUCTOR	*/
User::User() : _nick(""), _user(""), _auth(false) { return ; }

User::User(pollfd fd, struct sockaddr_in addr) : _nick("test"), _user(""), _auth(false), _fd(fd)
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

int 		User::getAut() const { return (this->_auth); }

pollfd 		User::getPollFd() const { return (this->_fd); }

int 		User::getFd() const { return (this->_fd.fd); }

std::string User::getIp() const { return (this->_ip); }


/*	SETTER	*/
void User::setNick(std::string input) { this->_nick = input; }

void User::setUser(std::string input) { this->_user = input; }

void User::setAut(bool input) { this->_auth = input; }

void User::setIp(std::string input) { this->_ip = input; }
