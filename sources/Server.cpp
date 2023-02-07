#include "Server.hpp"

Server::Server(int port, std::string password)
{
	// SOCKET
	// AF_INET = IPv4
	// SOCK_STREAM = TCP
	// 0 = protocol
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


	// BIND SOCKET
	// AF_INET = IPv4
	this->_addr.sin_family = AF_INET;
	// htons = host to network short
	// 6667 = port
	this->_addr.sin_port = htons(port);
	// htonl = host to network long
	// INADDR_ANY = any address
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

	// LISTEN
	// socket is ready to accept connections
	// n = number of connections
	try
	{
		if (listen(this->_sockfd, MAX_QUEUED) < 0)
			throw InvalidServerException("Error listening socket");
	}
	catch(const Server::InvalidServerException& e)
	{
		std::cerr << e.what() << '\n';
	}

	this->_password = password;
}

Server::~Server()
{
	if (this->_sockfd != -1)
		close(this->_sockfd);
}
