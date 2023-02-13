#include "../../includes/Server.hpp"

int	Server::cmd_motd(std::vector<std::string> params, User &user)
{
	(void)params;
	this->send_client("375 " + user.getNick() + " :- " + "ft_irc Message of the day - ", user);
	this->send_client("372 " + user.getNick() + " :- " + "Welcome to the ft_irc server", user);
	this->send_client("376 " + user.getNick() + " :End of MOTD command.", user);
	return (0);
}
