#include "../includes/Bot.hpp"
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <numeric>

Bot::Bot(std::string serv_pass) : _nick("cgg_bot")
{
	this->_serv_password = serv_pass;
	if ((this->_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::perror("Error: creating socket");
		exit(1);
	}
	this->_serv_addr.sin_family = AF_INET;
	this->_serv_addr.sin_port = htons(6667);

	struct addrinfo tmp;
	struct addrinfo *servinfo;

	memset(&tmp, 0, sizeof(tmp));
	tmp.ai_family = AF_UNSPEC;
	tmp.ai_socktype = SOCK_STREAM;
	getaddrinfo("127.0.0.1", 0, &tmp, &servinfo);

	this->_serv_addr.sin_addr = ((struct sockaddr_in *)servinfo->ai_addr)->sin_addr;
	freeaddrinfo(servinfo);
	this->auth();
}

Bot::~Bot()
{
	close(this->_sockfd);
}

void	Bot::connect_to_server()
{
	if (connect(this->_sockfd, (struct sockaddr *)&this->_serv_addr, sizeof(this->_serv_addr)) == -1)
	{
		std::perror("Error: connecting to server");
		exit(1);
	}
}

void 	Bot::send_pass()
{
	std::string msg = "PASS " + this->_serv_password + "\n\r";
	send(this->_sockfd, msg.c_str(), msg.length(), 0);
	return ;
}

void 	Bot::send_nick()
{
	std::string msg = "NICK " + this->_nick + "\n\r";
	send(this->_sockfd, msg.c_str(), msg.length(), 0);
	return ;
}

void 	Bot::send_user()
{
	std::string msg = "USER " + this->_nick + " " + this->_nick + " 127.0.0.1 :" + this->_nick + "\n\r";
	send(this->_sockfd, msg.c_str(), msg.length(), 0);
	return ;
}

void 	Bot::auth()
{
	this->connect_to_server();
	this->send_pass();
	this->send_nick();
	this->send_user();
	return ;
}

void 	Bot::run()
{

	this->rcv_msg();
	return ;
}

void 	Bot::rcv_msg()
{
	char buffer[1024];
	struct pollfd pfd;

	pfd.fd = this->_sockfd;
	pfd.events = POLLIN;

	if (poll(&pfd, 1, -1) == -1)
	{
		std::perror("Error: polling socket");
		exit(1);
	}
	int bytes_read;
	bytes_read = recv(this->_sockfd, buffer, 1024, 0);
	if (bytes_read == -1)
	{
		std::perror("Error: reading from socket");
		exit(1);
	}
	if (bytes_read == 0)
	{
		std::perror("Connection closed");
		exit(1);
	}
	buffer[bytes_read] = '\0';
	std::cout << buffer << std::endl;

	if (bytes_read < 100)
	{
		std::vector<std::vector<std::string> > parsed_msg = this->parsing_msg(buffer);
		monitor_cmd(parsed_msg[0]);
	}
	return ;
}

void	Bot::monitor_cmd(std::vector<std::string> arg)
{
	std::string cmd = arg[3];
	std::string	user(arg[0], 1, arg[0].find('!') - 1);

	if (cmd == ":!ping")
	{
		std::string sret = "PRIVMSG " + user + " :pong\r\n";
		send(this->_sockfd, sret.c_str(), sret.length(), 0);
	}
	else if (cmd.find(":!weather") != std::string::npos)
	{
		// Coupe la chaine de caractère à partir de la position 11 et stock la dans city
		std::string city;
		for (std::vector<std::string>::iterator it = arg.begin() + 4; it != arg.end(); it++)
			city += *it + " ";
		pop_back_str(city);
		std::map<std::string, std::string> weather = this->weather(city);
		std::string sret = "PRIVMSG " + user + " :Weather in " + weather["city"] + " : " + weather["temp"] + "°C, " + " feels like: " + weather["feelslike"] + ", " + weather["condition"] + ", wind speed: " + weather["wind"] + ", humidity: " + weather["humidity"] + ", it's " + weather["is_day"] + "\r\n";
		send(this->_sockfd, sret.c_str(), sret.length(), 0);
	}
	else if (cmd.find(":!help") != std::string::npos)
	{
		std::string sret = "PRIVMSG " + user + " :!ping, !weather <city>, !help\r\n";
		send(this->_sockfd, sret.c_str(), sret.length(), 0);
	}
	return ;
}
