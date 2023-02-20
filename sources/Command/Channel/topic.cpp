#include "../../../includes/Server.hpp"

int Server::cmd_topic(std::vector<std::string> params, User &user)
{
	if (params.size() == 0 || params[0].empty())
	{
		this->add_client("461 " + user.getNick() + " TOPIC :Not enough parameters\r\n", user);
		return (0);
	}
	else
	{
		std::cout << "TOPIC" << std::endl;
		if (params.size() == 1)
		{
			std::map<std::string, Channel>::iterator it = this->_channels.find(params[0]);
			if (it != this->_channels.end())
			{
				if (it->second.getTopic().empty())
				{
					this->add_client("331 " + user.getNick() + " " + params[0] + " :No topic is set", user);
					return (0);
				}
				else
				{
					this->add_client("332 " + user.getNick() + " " + params[0] + " " + it->second.getTopic(), user);
					return (0);
				}
			}
			else
			{
				this->add_client("403 " + params[0] + " :No such channel", user);
				return (0);
			}
		}
		else if (user.getOp() == true)
		{
			std::cout << "Topic is " << params[1] << std::endl;
			if (params[1] == "::")
			{
				for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
				{
					if (it->first == params[0])
					{
						it->second.setTopic("");
						this->add_client("332 " + user.getNick() + " " + it->first + " :" + "Topic Cleared", user);
						return (0);
					}
				}
				this->add_client("403 " + params[0] + " :No such channel", user);
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
						std::cout << "New topic is " << newTopic << std::endl;
						newTopic.erase(0, 1);
						newTopic.erase(newTopic.size() - 1, 1);
						it->second.setTopic(newTopic);
						this->add_client("332 " + user.getNick() + " " + it->first + " :" + it->second.getTopic(), user);
						return (0);
					}
				}
				this->add_client("403 " + params[0] + " :No such channel", user);
				return (0);
			}
		}
		else
		{
			this->add_client("482 " + user.getNick() + params[0] + " :You're not channel operator", user);
			return (0);
		}
	}
	return (0);
}
