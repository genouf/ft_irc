#include "../../../includes/Server.hpp"

int		Server::cmd_privmsg(std::vector<std::string> params, User &user)
{
	if (params.size() == 0 || params[0].empty())
	{
		this->send_client("411 " + user.getNick() + " :No recipient given (PRIVMSG)", user);
		return (0);
	}
	std::string msg;
	for (std::vector<std::string>::iterator it = params.begin() + 1; it != params.end(); it++)
	{
		msg += *it;
		if (it != params.end() - 1)
			msg += " ";
	}
	msg.erase(0, 1);
	if (this->isChannel(params[0]) == true)
	{
		std::cout << "Channel" << std::endl;
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
			if (it->first == params[0])
				for (std::map<int, User*>::iterator it2 = it->second.getUsers().begin(); it2 != it->second.getUsers().end(); it2++)
					if (it2->second->getFd() != user.getFd())
						this->send_client(" PRIVMSG " + params[0] + " :" + msg, user, (*it2->second));
	}
	else if (this->isUser(params[0]) == true)
		for (std::map<int, User>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
			if (it->second.getNick() == params[0])
				this->send_client(" PRIVMSG " + params[0] + " :" + msg, user, it->second);
	return (0);
}
