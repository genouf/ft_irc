#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <iostream>

int main(void)
{
	int socket_client = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_client < 0)
	{
		std::perror("Error creating socket");
		return (1);
	}

	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(6667);
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (connect(socket_client, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
	{
		std::perror("Error connecting socket");
		close(socket_client);
		return (1);
	}
	std::cout << "Socket connected" << std::endl;

	char msg[] = "Hello World!";
	if (send(socket_client, &msg, sizeof(msg), 0) < 0)
	{
		std::perror("Error sending data");
		close(socket_client);
		return (1);
	}
	std::cout << "Data sent" << std::endl;

	while (true)
	{}

	close(socket_client);
	return (0);
}
