#include "../includes/Server.hpp"
#include "poll.h"
#include <vector>

int main()
{
	Server sock(6667, "");

	std::cout << "Socket created and binded" << std::endl;
	std::cout << "Socket listening" << std::endl;
	sock.run();
	//std::vector<pollfd> fds;
	//struct pollfd	tmp = {sock.getSocket(), POLLIN, 0};
	//fds.push_back(tmp);
	//while (42)
	//{
	//	int ret = poll(fds.data(), fds.size(), -1);
	//	if (ret < 0)
	//	{
	//		std::perror("Error polling");
	//		close(sock.getSocket());
	//		return (1);
	//	}
	//	for (size_t i = 0; i < fds.size(); i++)
	//	{
	//		if (fds[i].revents & POLLHUP)
	//		{
	//			std::cout << "DISCONNECTED BY POLLHUP" << fds[i].fd << std::endl;
	//			close(fds[i].fd);
	//			fds.erase(fds.begin() + i);
	//			i--;
	//			continue;
	//		}
	//		if (fds[i].revents & POLLIN)
	//		{
	//			if (fds[i].fd == sock.getSocket())
	//			{
	//				struct sockaddr_in client_addr;
	//				socklen_t client_addr_len = sizeof(client_addr);
	//				int sock_client = accept(sock.getSocket(), (struct sockaddr *)&client_addr, &client_addr_len);
	//				if (sock_client < 0)
	//				{
	//					std::perror("Error accepting socket");
	//					close(sock.getSocket());
	//					return (1);
	//				}
	//				std::cout << "Socket accepted " << sock_client << std::endl;
	//				struct pollfd	tmp = {sock_client, POLLIN, 0};
	//				fds.push_back(tmp);
	//			}
	//			else
	//			{
	//				char msg[5196];
	//				int ret = recv(fds[i].fd, &msg, sizeof(msg), 0);
	//				if (ret < 0)
	//				{
	//					std::perror("Error receiving data");
	//					close(fds[i].fd);
	//					close(sock.getSocket());
	//					return (1);
	//				}
	//				if (ret == 0)
	//				{
	//					std::cout << "Socket closed " << fds[i].fd << std::endl;
	//					close(fds[i].fd);
	//					fds.erase(fds.begin() + i);
	//					i--;
	//					continue;
	//				}
	//				std::cout << "Data received from " << i << ": " << msg << std::endl;
	//			}
	//		}
	//	}
	//}

	return (0);
}
