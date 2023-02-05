#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <iostream>

int main()
{
	struct sockaddr_in addr;

	// SOCKET
	// AF_INET = IPv4
	// SOCK_STREAM = TCP
	// 0 = protocol
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::perror("Error creating socket");
		return (1);
	}

	// BIND SOCKET
	// AF_INET = IPv4
	addr.sin_family = AF_INET;
	// htons = host to network short
	// 6667 = port
	addr.sin_port = htons(6667);
	// htonl = host to network long
	// INADDR_ANY = any address
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		std::perror("Error binding socket");
		close(sockfd);
		return (1);
	}
	std::cout << "Socket created and binded" << std::endl;

	// CONNECT
	// test connection to the socket
	//if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	//{
	//	std::perror("Error connecting socket");
	//	close(sockfd);
	//	return (1);
	//}
	//std::cout << "Socket connected" << std::endl;

	// LISTEN
	// socket is ready to accept connections
	// n = number of connections
	if (listen(sockfd, 1) < 0)
	{
		std::perror("Error listening socket");
		close(sockfd);
		return (1);
	}
	std::cout << "Socket listening" << std::endl;

	// ACCEPT
	// accept connection
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int sock_client = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
	if (sock_client < 0)
	{
		std::perror("Error accepting socket");
		close(sockfd);
		return (1);
	}
	std::cout << "Socket accepted" << std::endl;

	// SEND
	// send data to the client
	char msg[] = "Hello World!";
	if (send(sock_client, &msg, sizeof(msg), 0) < 0)
	{
		std::perror("Error sending data");
		close(sock_client);
		close(sockfd);
		return (1);
	}
	std::cout << "Data sent" << std::endl;

	close(sock_client);
	close(sockfd);
	return (0);
}
