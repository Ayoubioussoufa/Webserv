/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:32:08 by aybiouss          #+#    #+#             */
/*   Updated: 2023/10/23 18:08:31 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Includes/Webserv.hpp"
#include "Configuration.hpp"
#include "Response.hpp"

struct   Methods
{
    bool _get;
    bool _post;
    bool _delete;
};


class Client
{
    private:
        int             _socketId;
        char**          _env;
        size_t          _locationIndex;
        std::string     _targetPath;
        Configuration   _client_server;
        std::map<std::string, std::string> _mapEnv;
    public :
        std::vector<Configuration>  _duplicated_servers;
        std::map<int, std::string> _defaultErrorPages;
        std::string                _CgiHeader;
        std::string                _CgiFile;
        int                        _content_fd;
        int                        _status;
        std::time_t                _cgiTimer;
        int                        _cgiPid;
        ssize_t                    _responseStatus;
        Methods                    methods;
        bool                       _isFavicon;
        Response                   response;
        bool                       _contentType;
        int                        _childExitStatus;
        int                        _waitStatus;
        bool                       _contentLenght;
        std::string                _root;
        Client();
        Client(const Client& other);
        Client& operator=(const Client& other);
        const int&          GetSocketId( void ) const;
        const Configuration&      getServer() const;
        void    set_server(Configuration p);
        void    set_socket(int socket);
        std::string getCookie( void );
        std::string GenerateDirectoryListing(const std::string& directoryPath);
        void    initDefaultErrorPages( void );
        void    readCgiHeader( int fd );
        void    SendHeader(int fd);
        const std::stringstream&    getFileSize(int fd);
        std::string get_content_type(const std::string& Path);
        void    fullMapEnv();
        void    fullEnv();
        void    deleteEnv();
        void    setTargetPath( void );
        size_t  getLocationIndex( void );
        void    initLocationIndex( void );
        void    SendErrorPage(int errorNumber);
        void    ft_delete( void );
        void    handleDirectory(const std::string& filePath);
        void    DirectoryHasIndexFile(const std::string& indexFile);
        void    checkIndexFile(const std::string& indexFile, const std::string& targetPath);
        void    Reply( void );
        void    ft_send( void );
        void    ft_Response( void );
        std::string findKey(const std::string &key);
        std::string getCurrentDirectory();
        ~Client();
};
