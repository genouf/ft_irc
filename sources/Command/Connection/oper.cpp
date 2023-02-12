#include "../../../includes/Server.hpp"

int		Server::cmd_oper(std::vector<std::string> params, User &user)
{
	if (params.size() < 2)
	{
		this->send_client(":127.0.0.1 461 OPER :Not enough parameters", user.getFd());
		return (0);
	}
	if (find(this->_nicks.begin(), this->_nicks.end(), params[0]) != this->_nicks.end() || this->_password != params[1])
	{
		this->send_client(":127.0.0.1 464 OPER :Password incorrect", user.getFd());
		return (0);
	}
	user.setOp(true);
	this->send_client(":127.0.0.1 381 :You are now an IRC operator", user.getFd());
	return(1);
}
