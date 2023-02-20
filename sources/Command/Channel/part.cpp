#include "../../../includes/Server.hpp"

int	Server::cmd_part(std::vector<std::string> params, User &user)
{
	if (params.size() == 0 || params[0].empty())
	{
		add_client("461 PART :Not enough parameters", user);
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
						add_client("PART " + *it + " " + params[1], user, (*it2->second));
					else
						add_client("PART " + *it + " " + ":Leaving", user, (*it2->second));
				}
				this->_channels[*it].removeUser(user);
			}
			else
				add_client("442 " + *it + " :You're not on that channel", user);
		}
		else
			add_client("403 " + *it + " :No such channel", user);
	}
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); )
	{
		if (it->second.getUsers().size() == 0)
			this->_channels.erase(it++);
		else
			++it;
	}
	return (0);
}
