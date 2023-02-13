#include "../includes/Server.hpp"
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

	std::cout << "SERVER ON" << std::endl;
	signal(SIGINT, handler);
	while (run)
		sock.run();

	return (0);
}
