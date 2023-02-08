#include "../includes/Server.hpp"
#include "../includes/User.hpp"
#include <signal.h>

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

	while (42)
	{
		int ret = poll(this->_sockets.data(), this->_sockets.size(), -1);
		if (ret < 0)
		{
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
					struct pollfd	tmp = {new_socket, POLLIN, 0};
					this->_sockets.push_back(tmp);
					this->_users.insert(std::make_pair(new_socket, User()));
				} while (new_socket != -1);
			}
			else
			{
				char msg[1024];
				int ret = recv(this->_sockets[i].fd, &msg, sizeof(msg), 0);
				if (ret < 0)
				{
					std::perror("Error receiving data");
					return (1);
				}
				if (ret == 0)
				{
					std::cout << "Socket closed " << this->_sockets[i].fd << std::endl;
					delete_socket(this->_sockets[i].fd, i);
					i--;
					continue;
				}
				std::cout << "Data received from " << i << ": " << msg << std::endl;
			}
		}
	}
	return (0);
}


/*	PRIVATE	*/
void	Server::delete_socket(int fd, int i)
{
	close(fd);
	this->_sockets.erase(this->_sockets.begin() + i);
	this->_users.erase(this->_sockets[i].fd);
}