#include "../includes/Bot.hpp"
#include "signal.h"

bool run = true;

void handler(int sig)
{
	(void)sig;
	run = false;
	return ;
}

int	main(void)
{
	Bot cgg_bot("1234");

	signal(SIGINT, handler);
	while (run)
		cgg_bot.run();
	return (0);
}