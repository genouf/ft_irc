#include "../../../includes/Server.hpp"

int		Server::cmd_kill(std::vector<std::string> params, User &user)
{
	if (user.getOp() != true)
	{
		std::string sret = "481 " + user.getNick() + " :Permission Denied- You're not an IRC operator\r\n";
		this->send_client(sret, user);
		return (0);
	}
	if (params.size() < 2)
	{
		this->send_client("461 KILL :Not enough parameters\r\n", user);
		return (0);
	}
	if (find(this->_nicks.begin(), this->_nicks.end(), params[0]) == this->_nicks.end())
	{
		std::string sret = "401 " + user.getNick() + " :No such nick\r\n";
		this->send_client(sret, user);
		return (0);
	}
	return(1);
}