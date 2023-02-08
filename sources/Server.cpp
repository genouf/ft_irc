#include "../includes/Server.hpp"
#include <signal.h>

int	running = 1;

void	stop_server(int sig)
{
	(void)sig;
	running = 0;
	return ;
}

/*	CONSTRUCTOR / DESTRUCTOR	*/
Server::Server(int port, std::string password)
{
	this->_password = password;

	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd < 0)
	{
		std::perror("Error creating socket");
		exit(1);
	}

	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(this->_sockfd, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) < 0)
	{
		perror("Error binding socket");
		close(this->_sockfd);
		exit(1);
	}

	if (listen(this->_sockfd, MAX_QUEUED) < 0)
	{
		perror("Error listening socket");
		close(this->_sockfd);
		exit(1);
	}
	struct pollfd tmp = {this->_sockfd, POLLIN, 0};
	this->_sockets.push_back(tmp);
	return ;
}

Server::~Server()
{
	for (std::vector<pollfd>::iterator it = this->_sockets.begin() ; it != this->_sockets.end(); it++)
		close(it->fd);
	return ;
}

/*	GETTER	*/
int Server::getSocket() const { return(this->_sockfd); }

/*	FUNCTIONS	*/
int	Server::run()
{
	size_t	current_size = 0;
	int		new_socket = -1;

	signal(SIGINT, stop_server);
	while (running)
	{
		int ret = poll(this->_sockets.data(), this->_sockets.size(), -1);
		if (ret < 0)
		{
			if (running == 1)
				std::perror("Error polling");
			return (1);
		}
		current_size = this->_sockets.size();
		for (size_t i = 0; i < current_size; i++)
		{
			if (this->_sockets[i].revents == 0)
				continue ;
			if (this->_sockets[i].revents != POLLIN)
			{
				std::perror("Error about revents");
				return (1);
			}
			if (this->_sockets[i].fd == this->_sockfd)
			{
				std::cout << "Listening socket is readable" << std::endl;
				do 
				{
					// Penser a rajouter la structure sockaddr_in si necessaire.
					new_socket = accept(this->_sockfd, NULL, NULL);
					if (new_socket < 0)
					{
						// A preciser
						if (errno != EWOULDBLOCK)
						{
							std::perror("Accept failed");
							return (1);
						}
						break;
					}
					authenticate(new_socket);
				} while (new_socket != -1);
			}
		}
	}
	return (0);
}

void	Server::authenticate(int new_socket)
{
	char	buffer[1024];
	for (int i = 0; i < 2; i++)
	{
		int		ret = recv(new_socket, buffer, sizeof(buffer), 0);
		if (ret < 0)
		{
			std::perror("Error receiving");
			return ;
		}
		if (ret == 0)
		{
			std::cout << "Connection closed" << std::endl;
			return ;
		}
		buffer[ret] = '\0';
		std::cout << "Received: " << buffer << std::endl;
	}

	send(new_socket, "001 :Connected\n", 16, 0);
	send(new_socket, "Hello World!", 12, 0);
	struct pollfd	tmp = {new_socket, POLLIN, 0};
	this->_sockets.push_back(tmp);
}