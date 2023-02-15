#include "../../../includes/Server.hpp"

int		Server::cmd_mode(std::vector<std::string> params, User &user)
{
	if (params.size() < 2 || params[0].empty())
	{
		this->send_client("461 MODE :Not enough parameters", user);
		return (0);
	}
	if (params[1][0] == '+')
	{
		if (params[1][1] == 'O' || params[1][1] == 'o')
		{
			user.setOp(true);
			this->send_client("221 " + user.getNick() + "+O :operator", user);
		}
	}
	else if (params[1][0] == '-')
	{
		if (params[1][1] == 'O' ||params[1][1] == 'o')
		{
			user.setOp(false);
			this->send_client("221 " + user.getNick() + "-O :basic", user);
		}
	}
	return (0);
}
