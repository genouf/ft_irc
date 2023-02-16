#include "../includes/Bot.hpp"
#include "signal.h"

bool run = true;

void handler(int sig)
{
	(void)sig;
	run = false;
	return ;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: ./cgg_bot <password>" << std::endl;
		return (1);
	}
	std::string password = argv[1];
	if (password.empty())
	{
		std::cout << "Error: Password can't be empty" << std::endl;
		return (1);
	}
	Bot cgg_bot(argv[1]);
	signal(SIGINT, handler);
	while (run)
		cgg_bot.run();
	return (0);
}