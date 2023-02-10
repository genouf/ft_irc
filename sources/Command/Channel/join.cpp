#include "../../../includes/Server.hpp"

int		Server::cmd_join(std::vector<std::string> params, User &user)
{
	size_t i = 0;
	std::string AllUsers;
	if (params.size() == 0)
	{
		this->send_client(":127.0.0.1 461 JOIN :Not enough parameters", user.getFd());
		return (0);
	}
	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		if (it->first == params[i])
		{
			std::vector<User> users = it->second.getUsers();
			for (std::vector<User>::iterator it = users.begin(); it != users.end(); it++)
			{
				AllUsers.append(it->getNick() + " ");
				this->send_client(":" + user.getNick() + " JOIN " + params[i], it->getFd());
			}
			it->second.addUser(user);
			std::string msg = ":127.0.0.1 332 " + user.getNick() + " " + params[i] + " :" + it->second.getTopic();
			this->send_client(msg, user.getFd());

			msg = ":127.0.0.1 353 " + user.getNick() + " = " + params[i] + " :";
			msg.append(user.getNick() + " " + AllUsers);
			this->send_client(msg, user.getFd());

			msg = ":127.0.0.1 366 " + user.getNick() + " " + params[i] + " :End of NAMES list";
			this->send_client(msg, user.getFd());
			if (i == params.size() - 1)
				return (0);
			i++;
		}
	}
	return (0);
}
