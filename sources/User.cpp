#include "../includes/User.hpp"

/*	CONSTRUCTOR / DESTRUCTOR	*/
User::User(struct pollfd socket) : _socket(socket) { return ; }

User::~User() { return ; }

/*	GETTER	*/
std::string	User::getNick() const { return (this->_nick); }

std::string	User::getName() const { return (this->_name); }

/*	SETTER	*/
void User::setNick(std::string input) { this->_nick = input; }

void User::setName(std::string input) { this->_name = input; }