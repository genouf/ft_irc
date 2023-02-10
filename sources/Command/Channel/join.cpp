#include "../../../includes/Server.hpp"

int		Server::cmd_join(std::vector<std::string> params, User &user)
{
	std::cout << "[CMD JOIN] send to " << user.getNick() << " on fd " << user.getFd() << std::endl;
	Channel channel;
	channel.setName("#channel");
	channel.setTopic("Topic");
	Channel channel2;
	channel2.setName("#channel2");
	channel2.setTopic("Topic2");
	this->_channels.insert(std::make_pair(channel.getName(), channel));
	this->_channels.insert(std::make_pair(channel2.getName(), channel2));
	size_t i = 0;
	std::string AllUsers;
	if (params.size() == 0)
	{
		this->send_client(":127.0.0.1 461 JOIN :Not enough parameters\r\n", user.getFd());
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
				this->send_client(":" + user.getNick() + " JOIN " + params[i] + "\r\n", it->getFd());
			}
			it->second.addUser(user);
			std::string msg = ":127.0.0.1 332 " + user.getNick() + " " + params[i] + " :" + it->second.getTopic() + "\r\n";
			std::cout << msg << std::endl;
			this->send_client(msg, user.getFd());

			msg = ":127.0.0.1 353 " + user.getNick() + " = " + params[i] + " :";
			msg.append(user.getNick() + " " + AllUsers + "\r\n");
			std::cout << msg << std::endl;
			this->send_client(msg, user.getFd());

			msg = ":127.0.0.1 366 " + user.getNick() + " " + params[i] + " :End of NAMES list\r\n";
			std::cout << msg << std::endl;
			this->send_client(msg, user.getFd());
			if (i == params.size() - 1)
				return (0);
			i++;
		}
	}
	return (0);
}
