#include "../includes/Channel.hpp"

Channel::Channel() {}
Channel::~Channel() {}

std::string Channel::getName() const { return(this->_name); }
std::string Channel::getTopic() const { return(this->_topic); }
std::vector<User> Channel::getUsers() const
{
	std::vector<User> users = std::vector<User>();
	for (std::map<int, User>::const_iterator it = this->_users.begin(); it != this->_users.end(); it++)
		users.push_back(it->second);
	return (users);
}

void Channel::setName(std::string name) { this->_name = name; }
void Channel::setTopic(std::string topic) { this->_topic = topic; }

void Channel::addUser(User user) { this->_users[user.getFd()] = user; }
void Channel::removeUser(User user) { this->_users.erase(this->_users.find(user.getFd())); }
void Channel::removeUser(std::string nick)
{
	for (std::map<int, User>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		if (it->second.getNick() == nick)
		{
			this->_users.erase(it);
			return ;
		}
	}
}

bool Channel::isUserInChannel(User user)
{
	for (std::map<int, User>::const_iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		if (it->second.getNick() == user.getNick())
			return (true);
	}
	return (false);
}

