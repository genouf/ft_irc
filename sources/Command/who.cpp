#include "../../includes/Server.hpp"

int	Server::cmd_who(std::vector<std::string> params, User &user)
{
	if (this->isChannel(params[0]))
	{
		Channel &channel = this->_channels.find(params[0])->second;
		std::map<int,User*> users = channel.getUsers();
		for (std::map<int,User*>::iterator it = users.begin(); it != users.end(); it++)
			this->send_client("352 " + user.getNick() + " " + channel.getName() + " " + it->second->getUsername() + " " + it->second->getIp() + " localhost "  + it->second->getRealname() + ": " + it->second->getNick(), user);
		this->send_client("315 " + user.getNick() + " " + channel.getName() + " :End of /WHO list.", user);
	}
	else if (this->isUser(params[0]))
	{
		User &user_to = this->_users.find(this->get_user_fd(params[0]))->second;
		this->send_client("352 " + user.getNick() + " " + user_to.getNick() + " " + user_to.getUsername() + " " + user_to.getIp() + " localhost "  + user_to.getRealname() + ": " + user_to.getNick(), user);
		this->send_client("315 " + user.getNick() + " " + user_to.getNick() + " :End of /WHO list.", user);
	}
	else
		this->send_client("402 " + user.getNick() + " " + params[0] + " :No such server", user);
	return (0);
}