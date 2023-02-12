#include "../../../includes/Server.hpp"

void	Server::send_info_join(Channel &channel, std::string title, User &user)
{
	std::string AllUsers;

	std::cout << "User added to channel " << title << std::endl;
	std::map<int, User*> users = channel.getUsers();
	for (std::map<int, User*>::iterator it = users.begin(); it != users.end(); it++)
	{
		AllUsers.append(it->second->getNick() + " ");
		this->send_client(":" + user.getNick() + " JOIN " + title, it->second->getFd());
	}
	if (!channel.getTopic().empty())
	{
		std::string msg = ":127.0.0.1 332 " + user.getNick() + " " + title + " :" + channel.getTopic();
		this->send_client(msg, user.getFd());
	}
	std::string msg = ":127.0.0.1 353 " + user.getNick() + " = " + title + " :";
	msg.append(user.getNick() + " " + AllUsers);
	channel.addUser(&user);
	this->send_client(msg, user.getFd());
	msg = ":127.0.0.1 366 " + user.getNick() + " " + title + " :End of NAMES list";
	this->send_client(msg, user.getFd());
}

int		Server::cmd_join(std::vector<std::string> params, User &user)
{
	if (params[0].empty())
	{
		this->send_client(":127.0.0.1 461 JOIN :Not enough parameters", user.getFd());
		return (0);
	}
	params = this->params_channel(params[0]);
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
	{
		if (!this->isChannel(*it))
		{
			Channel channel(*it);
			this->_channels.insert(std::make_pair<std::string, Channel>(*it, channel));
		}
	}
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		this->send_info_join(this->_channels.find(*it)->second, params[0], user);
	return (0);
}
