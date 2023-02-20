#include "../../includes/Server.hpp"

int	Server::cmd_info(std::vector<std::string> params, User &user)
{
	(void)params;
	this->add_client("371 " + user.getNick() + " : - CGG Server, created by cmarion, genouf and gkehren", user);
	this->add_client("374 " + user.getNick() + " :End of INFO list", user);
	return (0);
}
