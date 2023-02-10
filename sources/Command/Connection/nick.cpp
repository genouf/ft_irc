#include "../../../includes/Server.hpp"

int		Server::is_auth_nick(User &user)
{
	if (!user.getAut())
	{
		this->disconnect(user);
		return (0);
	}
	return (1);
}

int		Server::cmd_nick(std::vector<std::string> params, User &user)
{
	if (params[0].empty())
	{
		this->send_client(":127.0.0.1 431 :No nickname given", user.getFd());
		return (this->is_auth_nick(user));
	}
	if (params.size() > 1 || params[0].find_first_not_of("0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm-", 0) != std::string::npos)
	{
		std::string sret = ":127.0.0.1 432 " + params[0] + " :Erroneus nickname\r\n";
		this->send_client(sret, user.getFd());
		return (this->is_auth_nick(user));
	}
	std::vector<std::string>::iterator nick_found = find(this->_nicks.begin(), this->_nicks.end(), params[0]);
	if (nick_found != this->_nicks.end())
	{
		std::string sret = ":127.0.0.1 433 " + params[0] + " :Nickname is already in use\r\n";
		this->send_client(sret, user.getFd());
		return (this->is_auth_nick(user));
	}
	user._auth_ok.nick = true;
	user.setNick(params[0]);
	if (nick_found != this->_nicks.end())
		this->_nicks.erase(nick_found);
	this->_nicks.push_back(params[0]);
	return (1);
}
