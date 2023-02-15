#include "../../../includes/Server.hpp"

int	Server::kick_user(Channel &channel, User &user_from, User &user_to, std::vector<std::string> params)
{
	(void)params;
	if (channel.isUserInChannel(user_to) == false)
	{
		this->send_client("441 KICK " + user_to.getNick() + " " + channel.getName() + " :They aren't on that channel", user_from);
		return (0);
	}
	std::vector<std::string> tmp;
	tmp.push_back(channel.getName());
	tmp.push_back("kicked");
	this->cmd_part(tmp, user_to);
	if (params.size() > 2 && params[2].size() != 1)
		this->send_client("KICK " + params[0] + " " + user_to.getNick() + params[2], user_from, user_to);
	else
		this->send_client("KICK " + params[0] + " " + user_to.getNick() + " :has been kicked", user_from, user_to);
	return (1);
}


int		Server::cmd_kick(std::vector<std::string> params, User &user)
{
	bool	ok = true;

	if (user.getOp() == false)
	{
		this->send_client("481 :Permission Denied- You're not an IRC operator", user);
		return (0);
	}
	if (params.size() < 2)
	{
		this->send_client("461 KICK :Not enough parameters", user);
		return (0);
	}
	std::map<std::string, Channel>::iterator chan = this->_channels.find(params[0]);
	if (chan == this->_channels.end())
	{
		this->send_client("403 KICK :No such channel", user);
		return (0);
	}
	if (chan->second.isUserInChannel(user) == false)
	{
		this->send_client("442 KICK :You're not on that channel", user);
		return (0);
	}
	std::vector<std::string> to_kick = this->params_channel(params[1]);
	for (std::vector<std::string>::iterator it = to_kick.begin(); it != to_kick.end(); it++)
	{
		std::map<int, User>::iterator it_user = this->find_user_by_name(*it);

		if (it_user == this->_users.end())
		{
			this->send_client("441 KICK " + *it + " " + chan->second.getName() + " :They aren't on that channel", user);
			ok = false;
			continue;
		}
		if (this->kick_user(chan->second, user, it_user->second, params) == 0)
			ok = false;
	}
	if (ok == false)
		return (0);
	return (1);
}
