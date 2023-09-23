#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int MAX_CLIENTS = 10;

class Configuration {
public:
    // Add your configuration details here
};

class Client {
public:
    // Add your client details and methods here
};

class Servers {
public:
    // Add your server details and methods here
    int AllServers();
};

int main() {
    Servers server;
    server.AllServers();
    return 0;
}

int Servers::AllServers() {
    int maxFd = 0;
    fd_set read_fds;
    int server_fd;
    fd_set write_fds;
    int yes = 1;
    std::vector<int> clientsocket;
    std::map<int, Configuration> serverSockets;

    // ... Your server setup code ...

    while (true) {
        // ... Your code for handling new client connections ...

        for (std::vector<Client>::iterator its = _client.begin(); its != _client.end(); its++) {
            if (FD_ISSET(its->GetSocketId(), &tmp_read)) {
                char buffer[1024] = {0};
                ssize_t bytesRead = recv(its->GetSocketId(), buffer, sizeof(buffer), 0);
                if (bytesRead < 0) {
                    perror("Error reading from socket");
                    exit(EXIT_FAILURE);
                } else if (bytesRead == 0) {
                    // Handle client disconnect
                    close(its->GetSocketId());
                    // Remove the client from _client vector
                    _client.erase(its);
                } else {
                    // Handle received data (assuming it's HTTP request)
                    // Parse HTTP headers and body here
                    // Example: Check if it's an image upload
                    if (strstr(buffer, "Content-Type: image") != NULL) {
                        // Assuming image data starts after an empty line
                        char* imageDataStart = strstr(buffer, "\r\n\r\n");
                        if (imageDataStart != NULL) {
                            // Calculate the start of image data
                            imageDataStart += 4; // Move past "\r\n\r\n"
                            // Calculate the size of image data received
                            int imageDataSize = bytesRead - (imageDataStart - buffer);
                            // Here, you can save the image data to a file or process it
                            // imageDataStart points to the start of binary image data
                            // imageDataSize is the size of the image data
                            // You can save or process the image here
                            // Example:
                            // FILE* imageFile = fopen("image.jpg", "wb");
                            // fwrite(imageDataStart, 1, imageDataSize, imageFile);
                            // fclose(imageFile);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
