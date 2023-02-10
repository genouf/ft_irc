#include "../../../includes/Server.hpp"

int		Server::cmd_list(std::vector<std::string> params, User &user)
{
	(void)params;

	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
		this->send_client(":127.0.0.1 322 " + user.getNick() + " " + it->first + " 0 :" + it->second.getTopic(), user.getFd());
	this->send_client(":127.0.0.1 323 " + user.getNick() + " :End of channel list", user.getFd());
	return (0);
}
