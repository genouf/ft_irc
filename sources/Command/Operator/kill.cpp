#include "../../../includes/Server.hpp"
#include <numeric>

int		Server::cmd_kill(std::vector<std::string> params, User &user)
{
	if (!user.getOp())
	{
		std::string sret = "481 " + user.getNick() + " :Permission Denied- You're not an IRC operator";
		this->add_client(sret, user);
		return (0);
	}
	if (params.size() < 2 || (params.size() > 1 && params[1].size() < 2))
	{
		this->add_client("461 KILL :Not enough parameters", user);
		return (0);
	}
	if (find(this->_nicks.begin(), this->_nicks.end(), params[0]) == this->_nicks.end())
	{
		std::string sret = "401 " + params[0] + " :No such nick";
		this->add_client(sret, user);
		return (0);
	}
	User kill_user = this->_users.find(this->get_user_fd(params[0]))->second;
	params[1] = params[1].erase(0, 1);
	std::string reason;
	for (std::vector<std::string>::iterator it = params.begin() + 1; it != params.end(); it++)
		reason += *it + " ";
	reason = reason.erase(reason.size() - 1, 1);
	this->add_client("KILL " + kill_user.getNick() + " :" + reason, user, kill_user);
	this->add_client("Closing Link: " + kill_user.getNick() + "[" + kill_user.getIp() + "]" + " (Killed by " + user.getNick() + " " + reason + ")", kill_user);
	std::vector<std::string> quit;
	quit.push_back("Kick because : " + reason);
	this->cmd_quit(quit, kill_user);
	return(1);
}
