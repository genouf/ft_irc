#include "../../../includes/Server.hpp"

void	Server::send_info_join(Channel &channel, std::string title, User &user)
{
	std::string AllUsers;

	std::cout << "User added to channel " << title << std::endl;
	channel.addUser(&user);
	std::map<int, User*> users = channel.getUsers();
	for (std::map<int, User*>::iterator it = users.begin(); it != users.end(); it++)
		AllUsers.append(it->second->getNick() + " ");
	if (!channel.getTopic().empty())
	{
		std::string msg = "332 " + user.getNick() + " " + title + " :" + channel.getTopic();
		this->send_client(msg, user);
	}
	std::string msg = "353 " + user.getNick() + " = " + title + " :";
	msg.append(AllUsers);
	this->send_client(msg, user);
	msg = "366 " + user.getNick() + " " + title + " :End of NAMES list";
	this->send_client(msg, user);
	for (std::map<int, User*>::iterator it = users.begin(); it != users.end(); it++)
		this->send_client("JOIN " + title, (*it->second));
}

int		Server::cmd_join(std::vector<std::string> params, User &user)
{
	if (params.size() == 0 || params[0].empty())
	{
		this->send_client("461 JOIN :Not enough parameters", user);
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
	{
		std::cout << "Channel " << *it << std::endl;
		this->send_info_join(this->_channels.find(*it)->second, *it, user);
	}
	return (0);
}
