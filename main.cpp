/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkehren <gkehren@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 20:04:50 by gkehren           #+#    #+#             */
/*   Updated: 2023/02/05 02:16:12 by gkehren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

int main()
{
	int sockfd;
	struct sockaddr_in addr;

	// SOCKET
	// AF_INET = IPv4
	// SOCK_STREAM = TCP
	// 0 = protocol
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("Error creating socket");
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
		perror("Error binding socket");
		close(sockfd);
		return (1);
	}

	std::cout << "Socket created and binded" << std::endl;

	// CONNECT
	// test connection to the socket
	if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("Error connecting socket");
		close(sockfd);
		return (1);
	}
	std::cout << "Socket connected" << std::endl;

	// LISTEN
	// we should use listen to wait for connections to the socket

	close(sockfd);
	return (0);
}
