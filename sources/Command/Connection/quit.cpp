#include "../../../includes/Server.hpp"
#include <numeric>
#include <set>

int		Server::cmd_quit(std::vector<std::string> params, User &user)
{
	std::set<int>	user_called;

	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->second.isUserInChannel(user))
		{
			for (std::map<int, User*>::iterator it_user = it->second.getUsers().begin(); it_user != it->second.getUsers().end(); it_user++)
			{
				if (it_user->second->getFd() != user.getFd() && user_called.find(it_user->second->getFd()) == user_called.end())
				{
					this->send_client("QUIT " + std::accumulate(params.begin(), params.end(), std::string("")), it_user->second->getFd());
					user_called.insert(it_user->second->getFd());
				}
			}
			it->second.getUsers().erase(user.getFd());
		}
	}
	this->disconnect(user);
	return (0);
}