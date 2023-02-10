#include "../../../includes/Server.hpp"
#include <algorithm>

int	Server::cmd_part(std::vector<std::string> params, User &user)
{
	if (params.size() == 0)
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
				for (std::vector<User>::iterator it2 = it->second.getUsers().begin(); it2 != it->second.getUsers().end(); it2++)
					send_client(":" + user.getNick() + " PART " + params[i] + "", it2->getFd());
				it->second.removeUser(user);
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
		else
			send_client(":127.0.0.1 403 " + params[i] + " :No such channel", user.getFd());
	}
	return (0);
}
