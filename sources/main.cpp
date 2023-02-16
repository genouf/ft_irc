#include "../includes/Server.hpp"
#include <csignal>

bool run = true;

void	handler(int sig)
{
	(void)sig;
	run = false;
	return ;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	int port = atoi(argv[1]);
	if (port < 0 || port > 65535)
	{
		std::cout << "Error: Port must be between 0 and 65535" << std::endl;
		return (1);
	}
	std::string password = argv[2];
	if (password.empty())
	{
		std::cout << "Error: Password can't be empty" << std::endl;
		return (1);
	}

	Server sock(port, password);
	std::cout << "SERVER ON" << std::endl;
	signal(SIGINT, handler);
	while (run)
		sock.run();

	return (0);
}
