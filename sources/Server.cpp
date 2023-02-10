#include "../includes/Server.hpp"
#include "../includes/User.hpp"

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
			//std::perror("Error polling");
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
	// Connection
	this->_cmd_functions["PASS"] = &Server::cmd_password;
	this->_cmd_functions["NICK"] = &Server::cmd_nick;
	this->_cmd_functions["USER"] = &Server::cmd_user;
	this->_cmd_functions["PING"] = &Server::cmd_ping;

	// Channel
	this->_cmd_functions["LIST"] = &Server::cmd_list;
	this->_cmd_functions["JOIN"] = &Server::cmd_join;
	this->_cmd_functions["PART"] = &Server::cmd_part;
	this->_cmd_functions["TOPIC"] = &Server::cmd_topic;
	this->_cmd_functions["NAMES"] = &Server::cmd_names;
	return ;
}

/*	UTILS	*/
bool operator==(const pollfd &lhs, const pollfd &rhs)
{
    return lhs.fd == rhs.fd;
}

void	Server::send_client(std::string msg, int fd)
{
	std::cout << "[SEND] From server to " << fd << ": " << std::endl << msg << std::endl;
	msg.append("\r\n");
	send(fd, msg.c_str(), msg.size(), 0);
	return ;
}

void	Server::delete_socket(pollfd pfd)
{
	std::vector<pollfd>::iterator	tmp;

	close(pfd.fd);
	tmp = std::find(this->_sockets.begin(), this->_sockets.end(), pfd);
	this->_sockets.erase(tmp);
	this->_users.erase(pfd.fd);
}

void	Server::disconnect(User user)
{
	this->send_client(":127.0.0.1 ERROR : You have been disconnected.", user.getPollFd().fd);
	this->delete_socket(user.getPollFd());
	this->_nicks.erase(find(this->_nicks.begin(), this->_nicks.end(), user.getNick()));
}

int		Server::new_socket()
{
	int		new_socket = -1;

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
		this->_users.insert(std::make_pair(new_socket, User(tmp, client_addr)));
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
		delete_socket(this->_sockets[i]);
		i--;
		return (2);
	}
	msg[ret] = '\0';
	std::cout << "[RECEIVE] From client " << this->_sockets[i].fd << " to server: " << msg << std::endl;
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
		{
			if (s2.find("\n") != std::string::npos)
				s2.replace(s2.find("\n"), 1, "");
			if (s2.find("\r") != std::string::npos)
				s2.replace(s2.find("\r"), 1, "");
			tmp.push_back(s2);
		}
		big_v.push_back(tmp);
	}
	return (big_v);
}

/*	CMD	*/
bool	Server::check_pass(std::vector<std::vector<std::string> > input)
{
	for (std::vector<std::vector<std::string> >::iterator it = input.begin(); it != input.end(); it++)
	{
		if ((*it)[0] == "PASS")
			return (true);
	}
	return (false);
}

void	Server::monitor_cmd(std::vector<std::vector<std::string> > input, int user_fd)
{
	User &user = this->_users.find(user_fd)->second;

	if (user.getAut() == false && !this->check_pass(input))
	{
		this->send_client(":127.0.0.1 ERROR : No password supplied.", user.getFd());
		this->disconnect(user);
		return ;
	}
	for (std::vector<std::vector<std::string> >::iterator it = input.begin(); it != input.end(); it++)
	{
		std::string									tmp((*it)[0]);
		cmd_f										tmp_func = 0;
		std::map<std::string, cmd_f>::iterator  	tmp_it;

		tmp_it = this->_cmd_functions.find(tmp);
		if (tmp_it != this->_cmd_functions.end())
		{
			tmp_func = tmp_it->second;
			(*it).erase((*it).begin());
			if (!(this->*tmp_func)((*it), user))
				return ;
		}
	}
	if (user.getAut() == false && user._auth_ok.authentificated())
	{
		user.setAut(true);
		this->send_client(":127.0.0.1 001 " + user.getNick() + " :Welcome to the CGG Network, " + user.getNick() + "[" + user.getUsername() + "@" + "127.0.0.1]", user.getFd());
	}
}

void	Server::add_channel(Channel channel)
{
		this->_channels.insert(std::make_pair(channel.getName(), channel));
}
