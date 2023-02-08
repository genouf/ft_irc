#include "../includes/User.hpp"

/*	CONSTRUCTOR / DESTRUCTOR	*/
User::User(std::string nick, std::string name) : _nick(nick), _name(name) { return ; }

User::~User() { return ; }

/*	GETTER	*/
std::string	User::getNick() const { return (this->_nick); }

std::string	User::getName() const { return (this->_name); }

/*	SETTER	*/
void User::setNick(std::string input) { this->_nick = input; }

void User::setName(std::string input) { this->_name = input; }