/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:11:31 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/20 14:23:53 by aybiouss         ###   ########.fr       */
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
        bool nospace = false;
        for (size_t i = 0; i < line.length(); ++i) {
            if (!std::isspace(static_cast<unsigned char>(line[i]))) {
                nospace = true;
            }
        }
        if (nospace)
            block.push_back(line);// If any non-whitespace character is found
    }
    File.close();
    if (_servers.size() > 1)
        checkServers();
    AllServers();
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
    fd_set read_fds; //fd_set is a data structure used to manage file descriptors for I/O operations.
        // Fill up a fd_set structure with the file descriptors you want to know when data comes in on.
    int server_fd;
    fd_set write_fds;
    int yes = 1;
    std::vector<int>    clientsocket; // ! need a client class
    std::map<int, int> serverSockets;
    int i(0);
    for (std::vector<Configuration>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        struct addrinfo hints, *p, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET; 
        hints.ai_socktype = SOCK_STREAM;
        // hints.ai_flags = AI_PASSIVE; // !
        std::ostringstream s;
        s << it->getPort();
        if (getaddrinfo(it->getHost().c_str(), s.str().c_str(), &hints, &res) != 0)
        {
            std::cerr << "Error resolving hostname: " << it->getHost() << std::endl;
            continue;
        }
        for (p = res; p != NULL; p = p->ai_next)
        {
            if ((server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                perror("server: socket");
                continue;
            }   
            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                perror("setsockopt");
                exit(1);
            }   
            if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1) {
                close(server_fd);
                perror("server: bind");
                continue;
            }   
            break;
        }
        freeaddrinfo(res);
        if (p == NULL) {
            fprintf(stderr, "server: failed to bind\n");
            exit(1);
        }
        // sockets.setnonblocking(&server_fd);
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
    }
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    for (std::map<int, int>::iterator it = serverSockets.begin(); it != serverSockets.end(); it++)
    {
        FD_SET(it->second, &read_fds);
    }
    while (true)
    {
        // struct timeval timeout;
        // timeout.tv_sec = 1; // 1 second timeout
        // timeout.tv_usec = 0;
        fd_set tmp_read = read_fds;
        fd_set tmp_write = write_fds;
        int readySockets = select(maxFd + 1, &tmp_read, &tmp_write, NULL, NULL); // !
        if (readySockets < 0)
        {
            perror("Error with select");
            exit(EXIT_FAILURE);
        }
        for (std::map<int, int>::iterator it = serverSockets.begin(); it != serverSockets.end(); it++)
        {
            if (FD_ISSET(it->second, &tmp_read))
            {
                sockaddr_in clientAddr;
                int clientSocketw;
                socklen_t addrlen = sizeof(clientAddr);
                if ((clientSocketw = accept(it->second, (struct sockaddr *)&clientAddr, (socklen_t *)&addrlen)) < 0) // is used to accept this incoming connection. It creates a new socket descriptor (new_socket) for this specific client connection. The client's address information is stored in address.
                {
                    perror("Error accepting connection");
                    continue;
                }
                if (clientSocketw > maxFd)
                    maxFd = clientSocketw;
                clientsocket.push_back(clientSocketw);
                FD_SET(clientSocketw, &read_fds);
            }
        } // ! read then write boucle 3la client
        for (std::vector<int>::iterator its = clientsocket.begin(); its != clientsocket.end(); its++)
        {
            if (FD_ISSET(*its, &tmp_read))
            {
                char buffer[1024] = {0};
                // Read the HTTP request from the client
                ssize_t bytesRead = recv(*its, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead < 0) // ! Error reading from socket: Resource temporarily unavailable, dont know yet why
                {
                    perror("Error reading from socket");
                    exit(EXIT_FAILURE);
                }
                else if (bytesRead == 0)
                {
                    close(*its);
                    exit(EXIT_FAILURE);
                }
                else
                {
                    Response response;
                    response.parseHttpRequest(buffer, *its);
                    printf("%s\n", buffer);
                    FD_CLR(*its, &read_fds);
                    FD_SET(*its, &write_fds);
                }
            }
        }
        for (std::vector<int>::iterator its = clientsocket.begin(); its != clientsocket.end(); its++)
        {
            if (FD_ISSET(*its, &tmp_write))
            {
                // response
                const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
                write(*its, hello, strlen(hello));
                FD_CLR(*its, &write_fds);
                FD_SET(*its, &read_fds);
            }
        }
    }
    for (std::map<int, int>::iterator it = serverSockets.begin(); it != serverSockets.end(); it++)
        close(it->second);
    return 0;
}