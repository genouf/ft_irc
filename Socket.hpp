#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <iostream>
#include <cstring>

class Socket
{
	private:
		int sockfd;
		struct sockaddr_in addr;

	public:
		Socket(int port, std::string password);
		~Socket();

		int getSocket() const
		{
			return(this->sockfd);
		};

		class InvalidSocketException
		{
			public:
				InvalidSocketException(std::string error) : e(error + ": " + std::strerror(errno))
				{}

				const char *what() const throw()
				{
					return(e.c_str());
				}

			private:
				std::string e;
		};
};

Socket::Socket(int port, std::string password)
{
	// SOCKET
	// AF_INET = IPv4
	// SOCK_STREAM = TCP
	// 0 = protocol
	try
	{
		this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (this->sockfd < 0)
			throw InvalidSocketException("Error creating socket");
	}
	catch(const Socket::InvalidSocketException& e)
	{
		std::cerr << e.what() << std::endl;
	}


	// BIND SOCKET
	// AF_INET = IPv4
	this->addr.sin_family = AF_INET;
	// htons = host to network short
	// 6667 = port
	this->addr.sin_port = htons(port);
	// htonl = host to network long
	// INADDR_ANY = any address
	this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	try
	{
		if (bind(this->sockfd, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
			throw InvalidSocketException("Error binding socket");
	}
	catch(const Socket::InvalidSocketException& e)
	{
		std::cerr << e.what() << '\n';
	}
}

Socket::~Socket()
{
	if (this->sockfd != -1)
		close(this->sockfd);
}

