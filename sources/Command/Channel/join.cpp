#include "../../../includes/Server.hpp"

int		Server::cmd_join(std::vector<std::string> params, User &user)
{
	size_t 		i = 0;
	std::string AllUsers;

	if (params.size() == 0)
	{
		this->send_client(":127.0.0.1 461 JOIN :Not enough parameters", user.getFd());
		return (0);
	}
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->first == params[0])
		{
			std::cout << "User added to channel " << params[0] << std::endl;
			it->second.addUser(&user);
			std::map<int, User*> users = it->second.getUsers();
			for (std::map<int, User*>::iterator it = users.begin(); it != users.end(); it++)
			{
				AllUsers.append(it->second->getNick() + " ");
				this->send_client(":" + user.getNick() + " JOIN " + params[0], it->second->getFd());
			}
			std::string msg = ":127.0.0.1 332 " + user.getNick() + " " + params[0] + " :" + it->second.getTopic();
			this->send_client(msg, user.getFd());

			msg = ":127.0.0.1 353 " + user.getNick() + " = " + params[0] + " :";
			msg.append(user.getNick() + " " + AllUsers);
			this->send_client(msg, user.getFd());

			msg = ":127.0.0.1 366 " + user.getNick() + " " + params[0] + " :End of NAMES list";
			this->send_client(msg, user.getFd());
			if (i == params.size() - 1)
				return (0);
			i++;
		}
	}
	// for (std::map<std::string, Channel>::iterator it_map = this->_channels.begin(); it_map != this->_channels.end(); it_map++)
	// {
	// 	// std::cout << "size map :" << it_map->second.getUsers().size() << std::endl;
	// 	for (std::map<int, User>::const_iterator it_user = it_map->second.getUsers().begin(); it_user != it_map->second.getUsers().end(); it_user++)
	// 	{
	// 		std::cout << "NICK IS " << it_user->second.getNick() <<std::endl;
	// 	}
	// }
	return (0);
}
