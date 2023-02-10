#include "../../../includes/Server.hpp"

int		Server::cmd_names(std::vector<std::string> params, User &user)
{
	(void)params;
	(void)user;
	
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		std::cout << "Channel: " << it->first << std::endl;
		std::cout << "Params: " << params[0] << std::endl;
		if (params[0] == it->first)
		{
			std::string	names;
			for (std::map<int, User*>::iterator it2 = it->second.getUsers().begin(); it2 != it->second.getUsers().end(); it2++)
			{
				std::cout << "User: " << it2->second->getNick() << std::endl;
				names += it2->second->getNick();
				names += " ";
			}
			this->send_client(":127.0.0.1 353 " + user.getNick() + " = " + it->first + " :" + names, user.getFd());
			this->send_client(":127.0.0.1 366 " + user.getNick() + " " + it->first + " :End of /NAMES list.", user.getFd());
		}
	}
	return (0);
}

