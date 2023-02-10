#include "../../../includes/Server.hpp"

int		Server::cmd_quit(std::vector<std::string> params, User &user)
{
	(void)params;
	(void)user;
	// Close the connection with the client.
	// Send a message quit to all the user who share the same channel with the reason provide by the client.
	return (1);
}