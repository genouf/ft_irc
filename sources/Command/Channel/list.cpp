#include "../../../includes/Server.hpp"

int		Server::cmd_list(std::vector<std::string> params, User &user)
{
	if (params[0].empty())
	{
		for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
			this->send_client("322 " + user.getNick() + " " + it->first + " " + it->second.getNbUsers_string() + " :" + it->second.getTopic(), user);
	}
	else
	{
		std::vector<std::string> tmp = this->params_channel(params[0]);
		for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
		{
			if (this->_channels.find(*it) != this->_channels.end())
				this->send_client("322 " + user.getNick() + " " + *it + " " + this->_channels[*it].getNbUsers_string() + " :" + this->_channels[*it].getTopic(), user);
		}
	}
	this->send_client("323 " + user.getNick() + " :End of channel list", user);
	return (0);
}
