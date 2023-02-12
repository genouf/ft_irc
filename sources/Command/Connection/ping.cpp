#include "../../../includes/Server.hpp"

int		Server::cmd_ping(std::vector<std::string> params, User &user)
{
	this->send_client("PONG [127.0.0.1] " + params[0], user);
	return (1);
}
