#include "../../../includes/Server.hpp"

int		Server::is_auth_nick(User &user)
{
	if (!user.isAut())
	{
		this->disconnect(user);
		return (0);
	}
	return (1);
}

int		Server::cmd_nick(std::vector<std::string> params, User &user)
{
	if (params.size() == 0 || params[0].empty())
	{
		this->add_client("431 :No nickname given", user);
		return (this->is_auth_nick(user));
	}
	if (user.auth_ok.pass == false)
	{
		this->add_client("ERROR : No password supplied.", user);
		this->disconnect(user);
		return (0);
	}
	if (params.size() > 1 || params[0].find_first_not_of("0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm-_", 0) != std::string::npos)
	{
		std::string sret = "432 " + params[0] + " :Erroneus nickname\r\n";
		this->add_client(sret, user);
		return (this->is_auth_nick(user));
	}
	std::vector<std::string>::iterator nick_found = find(this->_nicks.begin(), this->_nicks.end(), params[0]);
	if (nick_found != this->_nicks.end())
	{
		std::string sret = "433 " + params[0] + " :Nickname is already in use\r\n";
		this->add_client(sret, user);
		return (this->is_auth_nick(user));
	}
	if (user.isAut() == true)
	{
		this->add_client("NICK " + params[0], user);
		this->_nicks.erase(find(this->_nicks.begin(), this->_nicks.end(), user.getNick()));
	}
	user.auth_ok.nick = true;
	user.setNick(params[0]);
	this->_nicks.push_back(params[0]);
	return (1);
}
