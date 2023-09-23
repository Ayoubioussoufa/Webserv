/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:07:10 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/23 13:51:08 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"

class Request
{
    protected:
        std::string     _method;
        std::string     _path;
        std::string     _httpVersion;
        std::string     _responseStatus;
        std::vector<char>     _header_before;
        std::vector<char>     _body;
        std::map<std::string, std::string> _headers;
        const char*     _bodyFile;
        std::string     _ofile;
        int             _chunksize;
    public:
        Request();
        Request(const Request& other);
        Request& operator=(const Request& other);
        int             parseHttpRequest(const std::string& requestBuffer, int new_socket);
        const std::string&     getPath() const;
        const std::string&     getMethod() const;
        const std::string&     getHttpVersion() const;
        const std::string&     getResponseStatus() const;
        void            setResponseStatus(const std::string& status);
        int             processChunk(const std::string& buffer); // int socket ?
        int             parseRequest();
        void            processBody(std::vector<char>& vec);
        size_t          customFind(const std::vector<char>& vec, const std::string& str, size_t start);
        int            parseHeaders();
        std::string     GenerateFile();
        int             countNumbersInString(const std::string& inputString);
        std::string     vectorCharToString(const std::vector<char>& vec);
        std::vector<char> customSubstr(const std::vector<char>& vec, size_t start, size_t length);
        ~Request();
};