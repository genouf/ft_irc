#include "../includes/User.hpp"

/*	CONSTRUCTOR / DESTRUCTOR	*/
User::User() : _auth(false) { return ; }

User::~User() { return ; }

/*	GETTER	*/
std::string	User::getNick() const { return (this->_nick); }

std::string	User::getUser() const { return (this->_user); }

/*	SETTER	*/
void User::setNick(std::string input) { this->_nick = input; }

void User::setUser(std::string input) { this->_user = input; }