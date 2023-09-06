#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// Function to create and set up a TCP socket for a client
int createClientSocket(const char *serverAddress, const char *port) {
    struct addrinfo hints, *res;
    int sockfd;

    // Set up the addrinfo hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6, whichever is available
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    // Get address info for the server
    int status = getaddrinfo(serverAddress, port, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        exit(1);
    }

    // Create a socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Connect to the server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        exit(1);
    }

    freeaddrinfo(res); // Free the linked list

    return sockfd;
}

// Function to create and set up a TCP socket for a server
int createServerSocket(const char *port) {
    struct addrinfo hints, *res;
    int sockfd;

    // Set up the addrinfo hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Use IPv4 or IPv6, whichever is available
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE; // Use local IP address

    // Get address info for the server
    int status = getaddrinfo(nullptr, port, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        exit(1);
    }

    // Create a socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Bind the socket to the address and port
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        exit(1);
    }

    freeaddrinfo(res); // Free the linked list

    // Listen for incoming connections
    if (listen(sockfd, 10) == -1) {
        perror("listen");
        exit(1);
    }

    return sockfd;
}

int main() {
    const char *serverAddress = "localhost"; // Replace with the server's address
    const char *port = "8080"; // Replace with the desired port number

    // Client
    int clientSocket = createClientSocket(serverAddress, port);
    std::cout << "Connected to the server." << std::endl;

    // Server
    int serverSocket = createServerSocket(port);
    std::cout << "Server is listening for connections on port " << port << "." << std::endl;

    // Accept connections from clients
    while (true) {
        struct sockaddr_storage clientAddr;
        socklen_t clientAddrSize = sizeof clientAddr;

        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            perror("accept");
            continue;
        }

        std::cout << "Accepted a new connection." << std::endl;

        // Handle client communication (send/receive data) on the new clientSocket

        close(clientSocket); // Close the client socket when done
    }

    close(serverSocket); // Close the server socket when done

    return 0;
}
