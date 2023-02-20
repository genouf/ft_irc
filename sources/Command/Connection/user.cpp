#include "../../../includes/Server.hpp"

int		Server::cmd_user(std::vector<std::string> params, User &user)
{
	if (params.size() < 4)
	{
		this->add_client("461 USER :Not enough parameters", user);
		this->disconnect(user);
		return (0);
	}
	if (user.isAut())
	{
		this->add_client("462 :You may not reregister", user);
		return (0);
	}
	std::string name;
	name = params.back();
	params.pop_back();
	if (params.back()[0] == ':')
	{
		name = params.back() + " " + name;
		name.erase(0, 1);
		params.pop_back();
	}
	user.setIp(params.back());
	params.pop_back();
	if (name.size() < 1 || params.back().size() < 1)
	{
		this->add_client("461 USER :Not enough parameters", user);
		this->disconnect(user);
		return (0);
	}
	user.setRealname(name);
	user.setUsername(params.back());
	user.auth_ok.user = true;
	return (1);
}
