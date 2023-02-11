#include "../../../includes/Server.hpp"
#include <numeric>

int		Server::cmd_quit(std::vector<std::string> params, User &user)
{
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->second.isUserInChannel(user))
		{
			for (std::map<int, User*>::iterator it_user = it->second.getUsers().begin(); it_user != it->second.getUsers().end(); it_user++)
			{
				if (it_user->second->getFd() != user.getFd())
					this->send_client("QUIT " + std::accumulate(params.begin(), params.end(), std::string("")), it_user->second->getFd());
			}
			break;
		}
	}
	this->disconnect(user);
	return (0);
}