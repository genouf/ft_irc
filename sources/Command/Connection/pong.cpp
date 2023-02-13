#include "../../../includes/Server.hpp"
#include <ctime>

void	Server::monitor_ping()
{
	for (std::map<int, User>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		clock_t	tmp = clock() - it->second.ping_info.last_ping;
		float	last_ping = ((float)tmp) / CLOCKS_PER_SEC;

		if (last_ping >= (float)0.08)
			this->send_ping(it->second);
	}
	return ;
}

void	Server::send_ping(User &user)
{
	if (user.ping_info.responded == false)
	{
		std::vector<std::string> tmp;
		tmp.push_back(":Ping timeout: 120 seconds");
		this->cmd_quit(tmp, user);
		return ;
	}
	user.ping_info.last_ping = clock();
	user.ping_info.responded = false;
	this->send_client("PING " + user.ping_info.token, user);
	return ;
}

int		Server::cmd_pong(std::vector<std::string> params, User &user)
{
	std::string tmp_s(":");
	tmp_s.append(user.ping_info.token);

	if (params[0] == tmp_s)
	{
		user.ping_info.responded = true;
		return (1);
	}
	std::vector<std::string> tmp;
	tmp.push_back(":Bad token PONG");
	this->cmd_quit(tmp, user);
	return (0);
}