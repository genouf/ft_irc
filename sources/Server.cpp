#include "../includes/Server.hpp"
#include "../includes/User.hpp"
#include <string>
#include <sstream>

/*	CONSTRUCTOR / DESTRUCTOR	*/
Server::Server(int port, std::string password)
{
	int on = 1;

	this->_password = password;
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd < 0)
	{
		std::perror("Error creating socket");
		exit(1);
	}
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)))
	{
		std::perror("Setsockopt failed");
		close(this->_sockfd);
		exit(1);
	}
	if (fcntl(this->_sockfd, F_SETFL, O_NONBLOCK))
	{
		std::perror("Fcntl failed");
		close(this->_sockfd);
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
	this->init_cmd_functions();
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
	while (42)
	{
		int ret = poll(this->_sockets.data(), this->_sockets.size(), -1);
		if (ret < 0)
		{
			std::perror("Error polling");
			return (1);
		}
		for (int i = 0; i < (int)this->_sockets.size(); i++)
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
				ret = this->new_socket();
				if (ret == 1)
					return (1);
				else if (ret == 2)
					break;
			}
			else
			{
				ret = this->new_msg(i);
				if (ret == 1)
					return (1);
				else if (ret == 2)
					continue;
			}
		}
	}
	return (0);
}


/*	PRIVATE	*/

void	Server::init_cmd_functions()
{
	this->_cmd_functions["PASS"] = &Server::cmd_password;
	return ;
}

/*	UTILS	*/
void	Server::delete_socket(int fd, int i)
{
	close(fd);
	this->_sockets.erase(this->_sockets.begin() + i);
	this->_users.erase(this->_sockets[i].fd);
}

int		Server::new_socket()
{
	int		new_socket = -1;

	std::cout << "Listening socket is readable" << std::endl;
	do
	{
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		new_socket = accept(this->_sockfd, (struct sockaddr*)&client_addr, &client_len);
		if (new_socket < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::perror("Accept failed");
				return (1);
			}
			return (2);
		}
		struct pollfd	tmp = {new_socket, POLLIN, 0};
		this->_sockets.push_back(tmp);
		this->_users.insert(std::make_pair(new_socket, User(new_socket, client_addr)));
		send(new_socket, ":127.0.0.1 001 test :LETS GO CA MARCHE\r\n", 41, 0);
		std::cout << "New connection, socket fd is " << new_socket << std::endl;
	} while (new_socket != -1);
	return (0);
}

int		Server::new_msg(int &i)
{
	char	msg[1024];
	int		ret = -1;

	ret = recv(this->_sockets[i].fd, &msg, sizeof(msg), 0);
	if (ret < 0)
	{
		if (errno == EWOULDBLOCK || errno == EAGAIN)
			return (1);
		std::perror("Error receiving data");
		return (1);
	}
	if (ret == 0)
	{
		delete_socket(this->_sockets[i].fd, i);
		i--;
		return (2);
	}
	msg[ret] = '\0';
	std::cout << "Data received from " << this->_sockets[i].fd << ": " << msg << std::endl;
	std::string msg_s(msg);
	this->monitor_cmd(this->parsing_msg(msg_s), this->_sockets[i].fd);
	return (0);
}

std::vector<std::vector<std::string> >	Server::parsing_msg(std::string msg)
{
	std::vector<std::vector<std::string> > 	big_v;
	std::istringstream						sstream(msg);
	std::string								s;

	while (std::getline(sstream, s, '\n'))
	{
		std::istringstream			sstream2(s);
		std::string					s2;
		std::vector<std::string>	tmp;
		while (std::getline(sstream2, s2, ' '))
			tmp.push_back(s2);
		big_v.push_back(tmp);
	}
	return (big_v);
}

/*	CMD	*/
void	Server::monitor_cmd(std::vector<std::vector<std::string> > input, int user_fd)
{
	for (std::vector<std::vector<std::string> >::iterator it = input.begin(); it != input.end(); it++)
	{
		std::string	tmp((*it)[0]);
		cmd_f			tmp_func = 0;

		tmp_func = this->_cmd_functions.find(tmp)->second;
		if (tmp_func != this->_cmd_functions.end()->second)
		{
			(*it).erase((*it).begin());
			(this->*tmp_func)((*it), this->_users.find(user_fd)->second);
		}
	}
}

int		Server::cmd_password(std::vector<std::string> params, User user)
{
	(void)params;
	(void)user;
	std::cout << "JE SUIS DANS LE CMD PASSWORD" << std::endl;
	return (0);
}
