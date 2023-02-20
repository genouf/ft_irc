#include "../includes/Server.hpp"
#include <csignal>

bool run = true;

void	handler(int sig)
{
	(void)sig;
	run = false;
	return ;
}

int	parsing(char **argv, int const &port, std::string const &password)
{
	std::string sport(argv[1]);

	if (sport.empty())
	{
		std::cout << "Error: Port can't be empty" << std::endl;
		return (1);
	}
	if (password.empty())
	{
		std::cout << "Error: Password can't be empty" << std::endl;
		return (1);
	}
	if (sport.find_first_not_of("1234567890") != std::string::npos)
	{
		std::cout << "Error: Port must be a number" << std::endl;
		return (1);
	}
	if (port < 0 || port > 65535)
	{
		std::cout << "Error: Port must be between 0 and 65535" << std::endl;
		return (1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	int port = atoi(argv[1]);
	std::string password = argv[2];

	if (parsing(argv, port, password) == 1)
		return (1);
	Server sock(port, password);
	std::cout << "SERVER ON" << std::endl;
	signal(SIGINT, handler);
	while (run)
		sock.run();
	return (0);
}
