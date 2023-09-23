/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:32:08 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/23 11:05:08 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"
#include "Configuration.hpp"
#include "Response.hpp"

class Client
{
    private : 
        // std::string _method;
        // std::string _path;
        // std::string _fileExtention;
        // std::string _contentType;
        // std::string _httpVersion;
        int         _socketId;
        Configuration   _client_server;
    public : 
        Response    response;
        Client();
        Client(const Client& other);
        Client& operator=(const Client& other);
        // Client(std::string method, std::string path, std::string fileExtention, std::string contentType, std::string httpVersion, int socketId);
        // const std::string&  GetMethod( void ) const;
        // const std::string&  GetPath( void ) const;
        // const std::string&  GetFileExtention( void ) const;
        // const std::string&  GetConetType( void ) const;
        // const std::string&  GetHttpVersion( void ) const;
        const int&          GetSocketId( void ) const;
        Configuration&      getServer();
        void    set_server(Configuration p);
        void    set_socket(int socket);
        ~Client();
};