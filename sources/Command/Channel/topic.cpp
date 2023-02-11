#include "../../../includes/Server.hpp"

int Server::cmd_topic(std::vector<std::string> params, User &user)
{
	if (params[0].empty())
	{
		this->send_client("127.0.0.1 461 " + user.getNick() + " TOPIC :Not enough parameters\r\n", user.getFd());
		return (0);
	}
	else
	{

		if (params.size() == 1)
		{
			for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
			{
				if (it->first == params[0])
				{
					this->send_client(":127.0.0.1 332 " + user.getNick() + " " + it->first + " :" + it->second.getTopic(), user.getFd());
					return (0);
				}
			}
			this->send_client(":127.0.0.1 403 " + params[0] + " :No such channel", user.getFd());
			return (0);
		}
		else
		{
			if (params[1] == "::")
			{
				for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
				{
					if (it->first == params[0])
					{
						it->second.setTopic("");
						this->send_client(":127.0.0.1 332 " + user.getNick() + " " + it->first + " :" + it->second.getTopic(), user.getFd());
						return (0);
					}
				}
				this->send_client(":127.0.0.1 403 " + params[0] + " :No such channel", user.getFd());
				return (0);
			}
			else
			{
				for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
				{
					if (it->first == params[0])
					{
						std::string newTopic;
						for (std::vector<std::string>::iterator it2 = params.begin(); it2 != params.end(); it2++)
						{
							if (it2 != params.begin())
								newTopic += *it2 + " ";
						}
						it->second.setTopic(newTopic);
						this->send_client(":127.0.0.1 332 " + user.getNick() + " " + it->first + " :" + it->second.getTopic(), user.getFd());
						return (0);
					}
				}
				this->send_client(":127.0.0.1 403 " + params[0] + " :No such channel", user.getFd());
				return (0);
			}
		}
	}
	return (0);
}
