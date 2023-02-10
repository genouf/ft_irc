#include "../../../includes/Server.hpp"

int		Server::cmd_list(std::vector<std::string> params, User &user)
{
	(void)params;
	//Channel channel;
	//channel.setName("#channel");
	//channel.setTopic("Topic");
	//Channel channel2;
	//channel2.setName("#channel2");
	//channel2.setTopic("Topic2");
	//this->_channels.insert(std::make_pair(channel.getName(), channel));
	//this->_channels.insert(std::make_pair(channel2.getName(), channel2));
	//std::cout << "[CMD LIST] send to " << user.getNick() << " on fd " << user.getFd() << std::endl;

	for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
		this->send_client(":127.0.0.1 322 " + user.getNick() + " " + it->first + " 0 :" + it->second.getTopic() + "\r\n", user.getFd());
	this->send_client(":127.0.0.1 323 " + user.getNick() + " :End of channel list", user.getFd());
	return (0);
}
