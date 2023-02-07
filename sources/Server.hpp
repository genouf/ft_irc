#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <iostream>
#include <cstring>

#define MAX_QUEUED 1000

class Server
{
	/*	MEMBER VAR	*/
	private:
		int					_sockfd;
		struct sockaddr_in	_addr;
		std::string			_password;

	public:
		/*	CONSTRUCTORS / DESTRUCTOR	*/
		Server(int port, std::string password);
		virtual ~Server();

		/*	GETTER	*/
		int getSocket() const
		{
			return(this->_sockfd);
		};

		/*	EXCEPTIONS	*/
		class InvalidServerException
		{
			public:
				InvalidServerException(std::string error) : e(error + ": " + std::strerror(errno))
				{}

				const char *what() const throw()
				{
					return(e.c_str());
				}

			private:
				std::string e;
		};
};

