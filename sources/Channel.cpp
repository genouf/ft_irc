#include "../includes/Channel.hpp"

/*	CONSTRUCTOR AND DESTRUCTOR	*/
Channel::Channel() { return ; }

Channel::Channel(std::string name) :_name(name) { return ; }

Channel::~Channel() {return ; }

/*	GETTER	*/
std::string const &Channel::getName() const { return(this->_name); }
std::string const &Channel::getTopic() const { return(this->_topic); }
std::map<int, User*> &Channel::getUsers() { return (this->_users); }

/*	SETTER	*/
void Channel::setName(std::string const &name) { this->_name = name; }
void Channel::setTopic(std::string topic) { this->_topic = topic; }

/*	ADD OR REMOVE	*/
void Channel::addUser(User *user) { this->_users[user->getFd()] = user; }
void Channel::removeUser(User user) { this->_users.erase(this->_users.find(user.getFd())); }
void Channel::removeUser(std::string nick)
{
	for (std::map<int, User*>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		if (it->second->getNick() == nick)
		{
			this->_users.erase(it);
			return ;
		}
	}
}

/*	INFO	*/
bool	Channel::isUserInChannel(User user)
{
	for (std::map<int, User*>::const_iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		if (it->second->getNick() == user.getNick())
			return (true);
	}
	return (false);
}

int	Channel::getNbUsers()
{
	return (this->_users.size());
}

std::string Channel::getNbUsers_string()
{
	std::stringstream ss;
	ss << this->_users.size();
	return (ss.str());
}
