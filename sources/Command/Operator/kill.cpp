#include "../../../includes/Server.hpp"

int		Server::cmd_kill(std::vector<std::string> params, User &user)
{
	if (!user.getOp())
	{
		std::string sret = "481 " + user.getNick() + " :Permission Denied- You're not an IRC operator\r\n";
		this->send_client(sret, user);
		return (0);
	}
	if (params.size() < 2 || (params.size() > 1 && params[1].size() < 2))
	{
		this->send_client("461 KILL :Not enough parameters\r\n", user);
		return (0);
	}
	if (find(this->_nicks.begin(), this->_nicks.end(), params[0]) == this->_nicks.end())
	{
		std::string sret = "401 " + params[0] + " :No such nick\r\n";
		this->send_client(sret, user);
		return (0);
	}
	User kill_user = this->_users.find(this->get_user_fd(params[0]))->second;
	this->send_client("ERROR :You have been killed, " + params[2], kill_user);
	// Send a KILL message to the killed_user
	this->send_client("KILL " + kill_user.getNick() + " :" + params[2], kill_user);
	this->send_client("Closing Link: " + kill_user.getNick() + " (" + kill_user.getNick() + ")", kill_user);
	// Kill the user with cmd_quit
	std::vector<std::string> quit;
	quit.append("Killed " + kill_user.getNick() + " (" + params[2] + ");
	this->cmd_quit(params, kill_user);
	return(1);
}