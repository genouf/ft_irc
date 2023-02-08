#include "../includes/Server.hpp"

/*	CONSTRUCTOR / DESTRUCTOR	*/
Server::Server(int port, std::string password)
{
	this->_password = password;
	try
	{
		this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (this->_sockfd < 0)
			throw InvalidServerException("Error creating socket");
	}
	catch(const Server::InvalidServerException& e)
	{
		std::cerr << e.what() << std::endl;
	}
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(port);
	this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	try
	{
		if (bind(this->_sockfd, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) < 0)
			throw InvalidServerException("Error binding socket");
	}
	catch(const Server::InvalidServerException& e)
	{
		std::cerr << e.what() << '\n';
	}
	try
	{
		if (listen(this->_sockfd, MAX_QUEUED) < 0)
			throw InvalidServerException("Error listening socket");
	}
	catch(const Server::InvalidServerException& e)
	{
		std::cerr << e.what() << '\n';
	}
	struct pollfd tmp = {this->_sockfd, POLLIN, 0};
	this->_users.push_back(User(tmp));
	this->_users[0].setNick("server");
	this->_users[0].setName("server");
}

Server::~Server()
{
	if (this->_sockfd != -1)
		close(this->_sockfd);
}

/*	GETTER	*/
int Server::getSocket() const { return(this->_sockfd); }

/*	FUNCTIONS	*/
void	Server::run()
{
	while (42)
	{
		int	ret = poll()
	}
}