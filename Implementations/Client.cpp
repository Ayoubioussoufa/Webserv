/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:32:21 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/23 11:22:38 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Client.hpp"

Client::Client()
{}

// Client::Client(std::string method, std::string path, std::string fileExtention, std::string contentType, std::string httpVersion, int socketId)
// {
//     _method = method;
//     _path = path;
//     _fileExtention = fileExtention;
//     _contentType = contentType;
//     _httpVersion = httpVersion;
//     _socketId = socketId;
// }

void    Client::set_server(Configuration p)
{
    _client_server = p;
}
void    Client::set_socket(int socket)
{
    _socketId = socket;
}

Configuration&      Client::getServer()
{
    return _client_server;
}

const int& Client::GetSocketId( void ) const
{       
    return (_socketId);
}

// const std::string& Client::GetMethod( void ) const
// {
//     return (_method);
// }

// const std::string& Client::GetPath( void ) const
// {
//     return (_path);
// }

// const std::string& Client::GetConetType( void ) const
// {
//     return (_contentType);
// }

// const std::string& Client::GetHttpVersion( void ) const
// {
//     return (_httpVersion);
// }

// const std::string&  Client::GetFileExtention( void ) const
// {
//     return (_fileExtention);
// }

Client::Client(const Client& other)
{
    if (this != &other)
    {
        this->_socketId = other._socketId;
        this->_client_server = other._client_server;
    }
}

Client& Client::operator=(const Client& other)
{
    if (this != &other)
    {
        this->_socketId = other._socketId;
        this->_client_server = other._client_server;
        this->response = other.response;
    }
    return *this;
}

Client::~Client() {}