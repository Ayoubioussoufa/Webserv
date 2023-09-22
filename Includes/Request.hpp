/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:07:10 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/22 19:34:42 by aybiouss         ###   ########.fr       */
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
        std::string     _body;
        std::map<std::string, std::string> _headers;
        const char*     _bodyFile;
    public:
        Request();
        int             parseHttpRequest(const std::string& requestBuffer, int new_socket);
        std::string     getPath() const;
        std::string     getMethod() const;
        std::string     getHttpVersion() const;
        std::string     getResponseStatus() const;
        void            setResponseStatus(const std::string& status);
        int             processChunk(const std::string& buffer); // int socket ?
        int             parseRequest();
        void            processBody(std::vector<char>& vec);
        size_t          customFind(const std::vector<char>& vec, const std::string& str);
        void            parseHeaders();
        std::string     vectorCharToString(const std::vector<char>& vec);
        std::vector<char> customSubstr(const std::vector<char>& vec, size_t start, size_t length);
        ~Request();
};