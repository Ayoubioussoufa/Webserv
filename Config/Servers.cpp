/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:11:31 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/19 13:45:21 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Servers.hpp"


// Getting all the blocks !!!
int Servers::ConfigFileParse(std::string file)
{
    std::ifstream File(file.c_str());
    if (!File.is_open()) {
        std::cerr << "Error: Unable to open configuration file." << std::endl;
        return 1;
    }

    std::string line;
    bool insideServerBlock = false;
    std::vector<std::string> block;
    std::stack<char> blockStack;// Stack to keep track of nested blocks
    while (std::getline(File, line))
    {
        std::cout << line << std::endl;
        if (line == "server")
        {
            insideServerBlock = true;
            continue;  // Skip the "server" line
        }
        if (insideServerBlock)
        {
            if (line == "{")
            {
                blockStack.push('{'); // Push a '{' for nested blocks
                continue; // Skip the opening curly brace
            }
            else if (line == "}")
            {
                blockStack.pop(); // Pop a '{' to close the current block
                if (blockStack.empty())
                {
                    insideServerBlock = false;  // Stop when we encounter the closing curly brace
                    // hna ndwz string kamlo (block) bach itparsa f configuration onmchi n9lb 3la next server f configfile
                    Configuration config(block);
                    _servers.push_back(config);
                    // for (size_t i = 0; i < block.size(); ++i) {
                    //     std::cout << block[i] << std::endl;
                    // }
                    block.clear(); // Clear the block for the next server
                }
            }
        }
        for (size_t i = 0; i < line.length(); ++i) {
            if (!std::isspace(static_cast<unsigned char>(line[i]))) {
                block.push_back(line);// If any non-whitespace character is found
            }
        }
    }
    File.close();
    // if (_servers.size() > 1)
    //     checkServers();
    // AllServers();
    return 0;
}

void Servers::checkServers()
{
    std::vector<Configuration>::iterator it1;
    std::vector<Configuration>::iterator it2;
    for (it1 = this->_servers.begin(); it1 != this->_servers.end() - 1; it1++)
	{
		for (it2 = it1 + 1; it2 != this->_servers.end(); it2++)
		{
			if (it1->getPort() == it2->getPort() && it1->getHost() == it2->getHost() && it1->getServerNames() == it2->getServerNames())
				throw std::string("Failed server validation");
		}
	}
}

void Servers::printServerData() const {
    for (std::vector<Configuration>::const_iterator it = _servers.begin(); it != _servers.end(); ++it) {
        std::cout << *it << std::endl;
    }
}

int Servers::AllServers()
{
    int maxFd = 0; //will store the maximum file descriptor value for use in select()
    Socket sockets;
    fd_set read_fds; //fd_set is a data structure used to manage file descriptors for I/O operations.
        // Fill up a fd_set structure with the file descriptors you want to know when data comes in on.
    int server_fd;
    std::map<int, int> serverSockets;
    int i(0);
    for (std::vector<Configuration>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
            perror("Cannot create socket");
            // fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
            exit(EXIT_FAILURE);
        }
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET; // IPv4
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(it->getHost().c_str(), NULL, &hints, &res) != 0)
        {
            std::cerr << "Error resolving hostname: " << it->getHost() << std::endl;
            close(server_fd);
            continue;
        }
        // It creates a socket using socket() with the address family (AF_INET for IPv4) and socket type (SOCK_STREAM for a TCP socket). If socket() fails, it prints an error message using perror() and returns 0.
        struct sockaddr_in address; // is defined to store socket address information.
        // memset((char *)&address, 0, sizeof(address));
        address.sin_family = AF_INET; //address family (sin_family) to AF_INET for IPv4
// ! hnaya l host        // address.sin_addr.s_addr = htonl(INADDR_ANY); //sin_addr.s_addr) to INADDR_ANY to listen on all available network interfaces
        address.sin_port = htons((size_t)(it->getPort()));
        address.sin_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr;
        
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("Bind failed");
            // fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
            exit(EXIT_FAILURE);
        } // binds the socket to the IP address and port defined in address
        sockets.setnonblocking(&server_fd);
        if (listen(server_fd, MAX_CLIENTS) < 0)
        {
            perror("Listen failed");
            // fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
            exit(EXIT_FAILURE);
        } // listens for incoming connections on the server socket (server_fd)
        std::cout << "Listening on port " << it->getPort() << std::endl;
        if (server_fd > maxFd)
            maxFd = server_fd;
        serverSockets[i] = server_fd;
        i++;
        freeaddrinfo(res);
    }
    while (true)
    {
        FD_ZERO(&read_fds);
        for (std::map<int, int>::iterator it = serverSockets.begin(); it != serverSockets.end(); it++)
            FD_SET(it->second, &read_fds);
        int readySockets = select(maxFd + 1, &read_fds, NULL, NULL, NULL);
        if (readySockets < 0)
        {
            perror("Error with select");
            exit(EXIT_FAILURE);
        }
        for (std::map<int, int>::iterator it = serverSockets.begin(); it != serverSockets.end(); it++)
        {
            if (FD_ISSET(it->second, &read_fds))
            {
                sockaddr_in clientAddr;
                int clientSocket;
                socklen_t addrlen = sizeof(clientAddr);
                if ((clientSocket = accept(it->second, (struct sockaddr *)&clientAddr, (socklen_t *)&addrlen)) < 0) {
                    perror("Error accepting connection");
                    continue;
                } // is used to accept this incoming connection. It creates a new socket descriptor (new_socket) for this specific client connection. The client's address information is stored in address.
                sockets.setnonblocking(&clientSocket);
                char buffer[1024] = {0};
                // Read the HTTP request from the client
                ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead < 0) // ! Error reading from socket: Resource temporarily unavailable, dont know yet why
                {
                    perror("Error reading from socket");
                    exit(EXIT_FAILURE);
                }
                else if (bytesRead == 0)
                {
                    close(clientSocket);
                    exit(EXIT_FAILURE);
                }
                else
                {
                    Response response;
                    response.parseHttpRequest(buffer, clientSocket);
                    printf("%s\n", buffer);
                    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
                    write(clientSocket, hello, strlen(hello));
                    close(clientSocket);
                }
                std::cout << "--------------------" << std::endl;
            }
        }
    }
    for (std::map<int, int>::iterator it = serverSockets.begin(); it != serverSockets.end(); it++)
        close(it->second);
    return 0;
}