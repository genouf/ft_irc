#include "../../../includes/Server.hpp"

int		Server::cmd_oper(std::vector<std::string> params, User &user)
{
	if (params.size() < 2)
	{
		this->send_client("461 OPER :Not enough parameters", user);
		return (0);
	}
	if (find(this->_nicks.begin(), this->_nicks.end(), params[0]) == this->_nicks.end())
	{
		this->send_client("491 OPER :No O-lines for you host", user);
		return (0);
	}
	if (this->_oper_password != params[1])
	{
		this->send_client("464 OPER :Password incorrect", user);
		return (0);
	}
	user.setOp(true);
	this->send_client("381 OPER :You are now an IRC operator", user);
	std::vector<std::string> tmp;
	tmp.push_back(params[0]);
	tmp.push_back("+o");
	this->cmd_mode(tmp, user);
	return(1);
}
