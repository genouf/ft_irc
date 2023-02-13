#include "../includes/User.hpp"

/*	CONSTRUCTOR / DESTRUCTOR	*/
User::User() : _nick(""), _username(""), _auth(false), _op(false) { return ; }

User::User(pollfd fd, struct sockaddr_in addr) : _nick("test"), _username(""), _auth(false), _fd(fd), _op(false)
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

std::string	User::getUsername() const { return (this->_username); }

std::string	User::getRealname() const { return (this->_realname); }

int 		User::getAut() const { return (this->_auth); }

pollfd 		User::getPollFd() const { return (this->_fd); }

int 		User::getFd() const { return (this->_fd.fd); }

std::string User::getIp() const { return (this->_ip); }

bool		User::getOp() const { return (this->_op); }

std::string &User::get_input() { return (this->_input); }

/*	SETTER	*/
void User::setNick(std::string input) { this->_nick = input; }

void User::setUsername(std::string input) { this->_username = input; }

void User::setRealname(std::string input) { this->_realname = input; }

void User::setAut(bool input) { this->_auth = input; }

void User::setIp(std::string input) { this->_ip = input; }

void User::setOp(bool input) { this->_op = input; }