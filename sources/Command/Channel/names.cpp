#include "../../../includes/Server.hpp"

int		Server::cmd_names(std::vector<std::string> params, User &user)
{
	if (params.size() == 0 || params[0].empty())
	{
		for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
		{
			if (params[0] == it->first)
			{
				std::string	names;
				for (std::map<int, User*>::iterator it2 = it->second.getUsers().begin(); it2 != it->second.getUsers().end(); it2++)
				{
					if (it2->second->getOp())
						names += "@";
					names += it2->second->getNick() + " ";
				}
				this->send_client("353 " + user.getNick() + " = " + it->first + " :" + names, user);
				this->send_client("366 " + user.getNick() + " " + it->first + " :End of NAMES list.", user);
			}
		}
	}
	else
	{
		std::vector<std::string> tmp = this->params_channel(params[0]);
		for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
		{
			std::map<std::string, Channel>::iterator chan = this->_channels.find(*it);
			if (chan != this->_channels.end())
			{
				std::string	names;
				for (std::map<int, User*>::iterator it2 = chan->second.getUsers().begin(); it2 != chan->second.getUsers().end(); it2++)
				{
					if (it2->second->getOp())
						names += "@";
					names += it2->second->getNick() + " ";
				}
				this->send_client("353 " + user.getNick() + " = " + chan->first + " :" + names, user);
				this->send_client("366 " + user.getNick() + " " + chan->first + " :End of NAMES list.", user);
			}
		}
	}
	return (0);
}

