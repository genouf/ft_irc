#include "../../../includes/Server.hpp"

int	Server::cmd_part(std::vector<std::string> params, User &user)
{
	if (params.size() == 0 || params[0].empty())
	{
		send_client("461 PART :Not enough parameters", user);
		return (0);
	}
	std::vector<std::string> chan = this->params_channel(params[0]);
	for (std::vector<std::string>::iterator it = chan.begin(); it != chan.end(); it++)
	{
		if (this->_channels.find(*it) != this->_channels.end())
		{
			if (this->_channels[*it].isUserInChannel(user))
			{
				for (std::map<int, User*>::iterator it2 = this->_channels[*it].getUsers().begin(); it2 != this->_channels[*it].getUsers().end(); it2++)
				{
					if (params.size() > 1)
						send_client("PART " + *it + " " + params[1], user, (*it2->second));
					else
						send_client("PART " + *it, user, (*it2->second));
				}
				this->_channels[*it].removeUser(user);
				if (this->_channels[*it].getUsers().size() == 0)
					this->_channels.erase(*it);
			}
			else
				send_client("442 " + *it + " :You're not on that channel", user);
		}
		else
			send_client("403 " + *it + " :No such channel", user);
	}
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->second.getUsers().size() == 0)
			this->_channels.erase(it);
	}
	return (0);
}
