#include "../includes/Server.hpp"
#include "../includes/User.hpp"
#include <sstream>
#include <numeric>

/*	CONSTRUCTOR / DESTRUCTOR	*/
Server::Server(int port, std::string password)
{
	int on = 1;

	this->_password = password;
	this->_oper_password = ADMIN_PASS;
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd < 0)
	{
		std::perror("Error creating socket");
		exit(1);
	}
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&on, sizeof(on)))
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
		std::perror("Error binding socket");
		close(this->_sockfd);
		exit(1);
	}

	if (listen(this->_sockfd, MAX_QUEUED) < 0)
	{
		std::perror("Error listening socket");
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
	int ret = poll(this->_sockets.data(), this->_sockets.size(), -1);
	// this->monitor_ping();
	if (ret < 0)
	{
		//std::perror("Error polling");
		return (1);
	}
	for (int i = 0; i < (int)this->_sockets.size(); i++)
	{
		if (this->_sockets[i].revents == 0)
			continue;
		if (this->_sockets[i].revents != POLLIN)
		{
			std::perror("Error about revents");
			this->disconnect(this->_users[this->_sockets[i].fd]);
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
		this->manage_send();
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
	this->_cmd_functions["PONG"] = &Server::cmd_pong;
	this->_cmd_functions["QUIT"] = &Server::cmd_quit;
	this->_cmd_functions["OPER"] = &Server::cmd_oper;

	// Channel
	this->_cmd_functions["LIST"] = &Server::cmd_list;
	this->_cmd_functions["JOIN"] = &Server::cmd_join;
	this->_cmd_functions["PART"] = &Server::cmd_part;
	this->_cmd_functions["TOPIC"] = &Server::cmd_topic;
	this->_cmd_functions["NAMES"] = &Server::cmd_names;
	this->_cmd_functions["PRIVMSG"] = &Server::cmd_privmsg;
	this->_cmd_functions["INVITE"] = &Server::cmd_invite;
	this->_cmd_functions["KICK"] = &Server::cmd_kick;

	//Other
	this->_cmd_functions["WHO"] = &Server::cmd_who;
	this->_cmd_functions["motd"] = &Server::cmd_motd;
	this->_cmd_functions["NOTICE"] = &Server::cmd_notice;
	this->_cmd_functions["info"] = &Server::cmd_info;
	this->_cmd_functions["CAP"] = &Server::cmd_cap;

	//Operator
	this->_cmd_functions["kill"] = &Server::cmd_kill;
	this->_cmd_functions["MODE"] = &Server::cmd_mode;
	return ;
}

/*	UTILS	*/
bool operator==(const pollfd &lhs, const pollfd &rhs)
{
    return lhs.fd == rhs.fd;
}

std::map<int, User>::iterator Server::find_user_by_name(std::string name)
{
	for (std::map<int, User>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		if (it->second.getNick() == name)
			return (it);
	}
	return (this->_users.end());
}


void	Server::add_client(std::string msg, User user)
{
	send_struct tmp(false, msg, user);
	this->_send_queue.push_back(tmp);
	return ;
}

void	Server::add_client(std::string msg, User user_from, User user_to)
{
	send_struct tmp(true, msg, user_from, user_to);
	this->_send_queue.push_back(tmp);
	return ;
}

void	Server::manage_send()
{
	for (std::vector<send_struct>::iterator it = this->_send_queue.begin(); it != this->_send_queue.end(); it++)
	{
		if (this->_users.find(it->user_to.getFd()) == this->_users.end())
			continue;
		if (it->mode == true && this->_users.find(it->user_from.getFd()) == this->_users.end())
			continue;
		if (it->mode == false)
			this->send_client(it->msg, it->user_to);
		else
			this->send_client(it->msg, it->user_from, it->user_to);
	}
	this->_send_queue.clear();
}


void	Server::send_client(std::string msg, User user)
{
	std::string from;

	if (user.isAut())
		from = ":" + user.getNick() + "!" + user.getUsername() + "@" + user.getIp();
	else
		from = ":localhost";
	from += " " + msg;
	std::cout << "[SEND] From server to " << user.getFd() << ": " << std::endl << from << std::endl;
	from.append("\r\n");
	send(user.getFd(), from.c_str(), from.size(), 0);
	return ;
}

void	Server::send_client(std::string msg, User user_from, User user_to)
{
	std::string from;

	if (user_from.isAut())
		from = ":" + user_from.getNick() + "!" + user_from.getUsername() + "@" + user_from.getIp();
	else
		from = ":localhost";
	from += " " + msg;
	std::cout << "[SEND] From server to " << user_to.getFd() << ": " << std::endl << from << std::endl;
	from.append("\r\n");
	send(user_to.getFd(), from.c_str(), from.size(), 0);
	return ;
}

void	Server::delete_socket(pollfd pfd)
{
	std::vector<pollfd>::iterator	tmp;

	if (pfd.fd != -1)
		close(pfd.fd);
	tmp = std::find(this->_sockets.begin(), this->_sockets.end(), pfd);
	this->_sockets.erase(tmp);
	this->_users.erase(pfd.fd);
}

void	Server::disconnect(User user)
{
	this->send_client("ERROR : You have been disconnected.", user);
	this->delete_socket(user.getPollFd());
	if (user.auth_ok.nick == true)
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

void	Server::pop_back_str(std::string &str)
{
	std::string::iterator it = str.begin() + str.size();
	str.erase(it, str.end());
	return ;
}

int		Server::new_msg(int &i)
{
	char	msg[1024];
	int		ret = -1;
	User	&user = this->_users.find(this->_sockets[i].fd)->second;
	int		user_fd = user.getFd();

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
		std::vector<std::string> tmp;
		tmp.push_back(":User timed out");
		this->cmd_quit(tmp, this->_users.find(this->_sockets[i].fd)->second);
		i--;
		return (2);
	}
	msg[ret] = '\0';
	std::cout << "[RECEIVE] From client " << this->_sockets[i].fd << " to server: " << msg << std::endl;
	std::string msg_s(msg);
	if (msg_s.find('\n') != std::string::npos && msg_s.size() == 1)
	{
		this->pop_back_str(user.get_input());
		user.get_input().append("\r\n");
	}
	else
		user.get_input().append(msg_s);
	if (user.get_input().find('\n') != std::string::npos)
	{
		std::size_t pos = user.get_input().find("  ");
		while (pos != std::string::npos) {
			user.get_input().replace(pos, 2, " ");
			pos = user.get_input().find("  ");
		}
		std::cout << "MONITOR : " <<  user.get_input() << std::endl;
		this->monitor_cmd(this->parsing_msg(user.get_input()), this->_sockets[i].fd);
		if (this->_users.find(user_fd) != this->_users.end())
			user.get_input().clear();
	}
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

std::vector<std::string> Server::params_channel(std::string params)
{
	std::vector<std::string> 	result;
	std::istringstream			sstream(params);
	std::string					s;

	while(std::getline(sstream, s, ','))
	{
		if (s.find("\n") != std::string::npos)
			s.replace(s.find("\n"), 1, "");
		if (s.find("\r") != std::string::npos)
			s.replace(s.find("\r"), 1, "");
		result.push_back(s);
	}
	return (result);
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

	for (std::vector<std::vector<std::string> >::iterator it = input.begin(); it != input.end(); it++)
	{
		std::string									tmp((*it)[0]);
		cmd_f										tmp_func = 0;
		std::map<std::string, cmd_f>::iterator  	tmp_it;

		if (tmp.empty())
			continue;
		tmp_it = this->_cmd_functions.find(tmp);
		if (tmp_it != this->_cmd_functions.end())
		{
			tmp_func = tmp_it->second;
			if (user.isAut() == false && (*((*it).begin()) != "PASS" && *((*it).begin()) != "NICK" && *((*it).begin()) != "USER") && *((*it).begin()) != "CAP")
				return ;
			(*it).erase((*it).begin());
			if (!(this->*tmp_func)((*it), user))
				return ;
		}
		else
			this->add_client("421 " + user.getNick() + " " + tmp + " :Unknown command", user);
	}
	if (!user.isAut() && user.auth_ok.authentificated())
	{
		user.setStatus(AUTHENTIFICATED);
		user.ping_info.token = user.getNick();
		this->add_client("001 " + user.getNick() + " :Welcome to the CGG Network, " + user.getNick() + "[" + user.getUsername() + "@" + "127.0.0.1]", user);
		//send all User available on server
		std::string AllUsers;
		for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); it++)
		{
			if (it->second.getOp())
				AllUsers.append("@");
			AllUsers.append(it->second.getNick() + " ");
		}
		this->add_client("001 " + user.getNick() + " :Users on the server : " + AllUsers, user);
		std::vector<std::string> params;
		this->cmd_motd(params, user);
		this->cmd_info(params, user);
		// this->send_ping(user);
	}
}

void	Server::add_channel(Channel channel)
{
		this->_channels.insert(std::make_pair(channel.getName(), channel));
}

bool	Server::isUser(std::string const &nick)
{
	for (std::map<int, User>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		if (it->second.getNick() == nick)
			return (true);
	}
	return (false);
}

bool	Server::isChannel(std::string channel)
{
	if (channel[0] != '#')
		channel.insert(0, "#");
	if (this->_channels.find(channel) != this->_channels.end())
		return (true);
	return (false);
}

int	Server::get_user_fd(std::string nick)
{
	for (std::map<int, User>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
	{
		if (it->second.getNick() == nick)
			return (it->first);
	}
	return (-1);
}

int		Server::cmd_cap(std::vector<std::string> params, User &user)
{
	(void)params;
	(void)user;
	return (1);
}
