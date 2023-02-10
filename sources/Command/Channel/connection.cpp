#include "../../../includes/Server.hpp"

int		Server::cmd_password(std::vector<std::string> params, User &user)
{
	std::cout << "JE SUIS DANS LE CMD PASSWORD" << std::endl;
	std::string pass;
	if (params[0].empty())
	{
		this->send_client(":127.0.0.1 461 PASS : Not enough parameters", user.getFd());
		return (0);
	}
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		pass = pass + *it;
	if (this->_password != pass)
	{
		std::cout << "PASSWORD INCORRECT" << std::endl;
		this->send_client(":127.0.0.1 464 : Password incorrect", user.getFd());
		this->disconnect(user);
		return (0);
	}
	else
	{
		std::cout << "PASSWORD CORRECT" << std::endl;
		user._auth_ok.pass = true;
	}
	return (1);//question pour le code ERR_ALREADYREGISTERED (462)
}

int		Server::cmd_nick(std::vector<std::string> params, User &user)
{
	std::cout << "JE SUIS DANS LA CMD NICK" << std::endl;
	if (params.size() == 0)
	{
		std::cout << "BAD NICK" << std::endl;
		this->send_client(":127.0.0.1 431 :No nickname given", user.getFd());
		this->disconnect(user);
		return (0);
	}
	if (params.size() > 1 || params[0].find_first_not_of("0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm-", 0) != std::string::npos)
	{
		std::cout << "BAD NICK" << std::endl;
		std::string sret = ":127.0.0.1 432 " + params[0] + " :Erroneus nickname\r\n";
		this->send_client(sret, user.getFd());
		this->disconnect(user);
		return (0);
	}
	if (find(this->_nicks.begin(), this->_nicks.end(), params[0]) != this->_nicks.end())
	{
		std::cout << "BAD NICK" << std::endl;
		std::string sret = ":127.0.0.1 433 " + params[0] + " :Nickname is already in use\r\n";
		this->send_client(sret, user.getFd());
		if (!user.getAut())
		{
			this->disconnect(user);
			return (0);
		}
		else
			return (1);
	}
	std::cout << "GOOD NICK" << std::endl;
	user._auth_ok.nick = true;
	user.setNick(params[0]);
	this->_nicks.push_back(params[0]);
	return (1);
}

int		Server::cmd_user(std::vector<std::string> params, User &user)
{
	std::cout << "JE SUIS DANS LA CMD USER" << std::endl;
	if (user.getAut())
	{
		this->send_client(":127.0.0.1 462 :You may not reregister", user.getFd());
	}
	if (params.size() < 5)
	{
		std::cout << "TOO SMALL NAME" << std::endl;
		this->send_client(":127.0.0.1 433 USER :Not enough parameters", user.getFd());
		this->disconnect(user);
		return (0);
	}
	std::string name;
	name = params.back();
	params.pop_back();
	if (params.back()[0] == ':')
	{
		name = params.back() + " " + name;
		name.erase(0, 1);
		params.pop_back();
	}
	user.setIp(params.back());
	params.pop_back();
	if (name.size() < 1 || params.back().size() < 1)
	{
		std::cout << "TOO SMALL NAME" << std::endl;
		this->send_client(":127.0.0.1 433 USER :Not enough parameters", user.getFd());
		this->disconnect(user);
		return (0);
	}
	user.setRealname(name);
	user.setUsername(params.back());
	user.setAut(true);
	user._auth_ok.user = true;
	std::cout << user.getUsername() << " " << user.getRealname() << " " << user.getIp() << std::endl;
	return (0);
}