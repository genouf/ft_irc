#include "../../../includes/Server.hpp"
#include <algorithm>

int	Server::cmd_part(std::vector<std::string> params, User &user)
{
	if (params[0].empty())
	{
		send_client("461 PART :Not enough parameters", user.getFd());
		return (0);
	}
	size_t i = 0;
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->first == params[i])
		{
			if (it->second.isUserInChannel(user))
			{
				for (std::map<int, User*>::iterator it2 = it->second.getUsers().begin(); it2 != it->second.getUsers().end(); it2++)
					send_client(":" + it2->second->getNick() + "!" + it2->second->getUsername() + "@127.0.0.1" + " PART " + params[i], it2->second->getFd());
				it->second.removeUser(user);
				if (it->second.getUsers().size() == 0)
					this->_channels.erase(it);
				return (0);
			}
			else
			{
				send_client(":127.0.0.1 442 " + params[i] + " :You're not on that channel", user.getFd());
				return (0);
			}
			if (i == params.size() - 1)
				return (0);
			i++;
		}
	}
	return (0);
}
