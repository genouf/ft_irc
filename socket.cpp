#include "Socket.hpp"

int main()
{
	Socket sock(6667, "");
	std::cout << "Socket created and binded" << std::endl;

	// LISTEN
	// socket is ready to accept connections
	// n = number of connections
	if (listen(sock.getSocket(), 1) < 0)
	{
		std::perror("Error listening socket");
		close(sock.getSocket());
		return (1);
	}
	std::cout << "Socket listening" << std::endl;

	// ACCEPT
	// accept connection
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int sock_client = accept(sock.getSocket(), (struct sockaddr *)&client_addr, &client_addr_len);
	if (sock_client < 0)
	{
		std::perror("Error accepting socket");
		close(sock.getSocket());
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
		close(sock.getSocket());
		return (1);
	}
	std::cout << "Data sent" << std::endl;

	close(sock_client);
	close(sock.getSocket());
	return (0);
}
