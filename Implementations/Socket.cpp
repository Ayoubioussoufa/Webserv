/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:12:03 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/04 17:33:37 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Socket.hpp"

// testing !!!

int Socket::function()
{
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("cannot create socket"); 
        return 0; 
    }
    struct sockaddr_in address;
    const int PORT = 8080; //Where the clients can reach at
    /* htonl converts a long integer (e.g. address) to a network representation */ 
    /* htons converts a short integer (e.g. port) to a network representation */ 
    memset((char *)&address, 0, sizeof(address)); 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(PORT); 
    if (bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0) 
    { 
        perror("bind failed"); 
        return 0; 
    }
    if (listen(server_fd, 3) < 0) 
    { 
        perror("In listen"); 
        exit(EXIT_FAILURE); 
    }
    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        int new_socket;
        socklen_t addrlen = sizeof(address);
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("In accept");            
            exit(EXIT_FAILURE);        
        }
        char buffer[1024] = {0};
        int valread = read( new_socket , buffer, 1024); 
        printf("%s\n", buffer);
        if(valread < 0)
        { 
            printf("No bytes are there to read");
        }
        const char *hello = "Hello from the server"; //IMPORTANT! WE WILL GET TO IT
        write(new_socket , hello , strlen(hello));
        close(new_socket);
    }
    return 0;
}