#include "../includes/Server.hpp"
#include "poll.h"
#include <csignal>

bool run = true;

void	handler(int sig)
{
	(void)sig;
	run = false;
	return ;
}

int main()
{
	Server sock(6667, "PASSWORD");

	Channel channel;
	channel.setName("#channel");
	// channel.setTopic("Topic");
	Channel channel2;
	channel2.setName("#channel2");
	channel2.setTopic("Topic2");
	sock.add_channel(channel);
	sock.add_channel(channel2);

	std::cout << "SERVER ON" << std::endl;
	signal(SIGINT, handler);
	while (run)
		sock.run();

	return (0);
}
