/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:27:53 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/21 11:21:21 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Request.hpp"

Request::Request() {}

// Function to parse an HTTP request
void Request::parseHttpRequest(const std::string& requestBuffer, int new_socket) {

    if (requestBuffer.find("Transfer-Encoding:chunked") != std::string::npos)
        processChunk(requestBuffer);
    // Split the request into lines
    else
    {
        std::istringstream requestStream(requestBuffer);
        std::string line;
    
        // Read the first line (request line)
        if (!std::getline(requestStream, line)) {
            // Handle an empty or incomplete request
            setResponseStatus("400 Bad Request");
            return ;
        }
        std::istringstream requestLineStream(line);
        if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
            // Handle invalid request line
            setResponseStatus("400 Bad Request");
            return;
        }
        //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
        if (_path == "/favicon.ico") {
            // Handle it as needed (status), or simply return an empty request
            return ;
        }
        std::string forBody;
        bool isContentLengthFound = false;
        std::size_t contentLength = 0;
        // Read and parse headers
        while (std::getline(requestStream, line) && !line.empty()) {
            forBody += line + "\n";
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                std::string headerName = line.substr(0, pos);
                std::string headerValue = line.substr(pos + 1);
                // Remove leading/trailing whitespaces from header values
                headerValue.erase(0, headerValue.find_first_not_of(" \t"));
                headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
                _header[headerName] = headerValue;
                if (_method == "POST" && headerName == "Content-Length")
                {
                    try {
                        char* endptr;
                        const char* headerValueCStr = headerValue.c_str();
                        unsigned long parsedContentLength = strtoul(headerValueCStr, &endptr, 10);
    
                        if (parsedContentLength == ULONG_MAX) { /*endptr == headerValueCStr || *endptr != '\0' || */
                            // Handle invalid Content-Length value
                            setResponseStatus("400 Bad Request");
                            return;
                        }
                        contentLength = parsedContentLength;
                        isContentLengthFound = true;
                    } catch (const std::exception& e) {
                        // Handle invalid Content-Length value
                        setResponseStatus("400 Bad Request");
                        return;
                    }
                }
            }
        }
        (void)new_socket;
        if (_method == "POST") {
            if (!isContentLengthFound) {
                // Handle missing Content-Length header for POST requests
                setResponseStatus("411 Length Required");
                return;
            }
            std::size_t bodyPos = forBody.find("\r\n\r\n");
            if (bodyPos != std::string::npos) {
                // Extract the body
                std::string requestBody = forBody.substr(bodyPos + 4);
                // Write the body to a file
                _bodyFile = "BodyOfRequest.txt";
                std::ofstream outfile(_bodyFile, std::ofstream::binary);
                outfile.write(requestBody.c_str(), contentLength);
                outfile.close();
            }
            else {
                // Handle missing Content-Length header for POST requests
                setResponseStatus("411 Length Required");
                return ;
            }
        }
        setResponseStatus("200 OK");
    }
}

    // std::cout << "******************************" << std::endl;
    // std::cout << _header.size() << std::endl;
    // std::cout << "******************************" << std::endl;
    // for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); ++it) {
    //         r this task. We will provide our
    // }
    // std::cout << std::endl;
    // std::cout << "******************************" << std::endl;
    // std::cout << forBody << std::endl;
    // std::cout << "******************************" << std::endl;
    
void		Request::processChunk(const std::string &buffer)
{
    // nrd requests vector and check in function of the socket (fd)
	std::string	head = buffer.substr(0, buffer.find("\r\0\r\n"));
	std::string	chunks = buffer.substr(buffer.find("\r\n\r\n") + 4, buffer.length() - 1);
	std::string	subchunk = chunks.substr(0, 100);
	std::string	body = "";
	int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	size_t		i = 0;
	while (chunksize)
	{
		i = chunks.find("\r\n", i) + 2;
		body += chunks.substr(i, chunksize);
		i += chunksize + 2;
		subchunk = chunks.substr(i, 100);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}
	std::string requestBuffer = head + "\r\n\r\n" + body + "\r\n\r\n"; // NJM3Ha kamla onsiftha l constructor again and clear the old one
    std::istringstream requestStream(requestBuffer);
    std::string line;
    
    // Read the first line (request line)
    if (!std::getline(requestStream, line)) {
        // Handle an empty or incomplete request
        setResponseStatus("400 Bad Request");
        return ;
    }
    std::istringstream requestLineStream(line);
    if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
        // Handle invalid request line
        setResponseStatus("400 Bad Request");
        return;
    }
    //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
    if (_path == "/favicon.ico") {
        // Handle it as needed (status), or simply return an empty request
        return ;
    }
    std::string forBody;
    bool isContentLengthFound = false;
    std::size_t contentLength = 0;
    // Read and parse headers
    while (std::getline(requestStream, line) && !line.empty())
    {
        forBody += line + "\n";
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 1);
            // Remove leading/trailing whitespaces from header values
            headerValue.erase(0, headerValue.find_first_not_of(" \t"));
            headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
            _header[headerName] = headerValue;
            if (_method == "POST" && headerName == "Content-Length")
            {
                try {
                    char* endptr;
                    const char* headerValueCStr = headerValue.c_str();
                    unsigned long parsedContentLength = strtoul(headerValueCStr, &endptr, 10);

                    if (parsedContentLength == ULONG_MAX) { /*endptr == headerValueCStr || *endptr != '\0' || */
                        // Handle invalid Content-Length value
                        setResponseStatus("400 Bad Request");
                        return;
                    }
                    contentLength = parsedContentLength;
                    isContentLengthFound = true;
                } catch (const std::exception& e) {
                    // Handle invalid Content-Length value
                    setResponseStatus("400 Bad Request");
                    return;
                }
            }
        }
    }
    if (_method == "POST") {
        if (!isContentLengthFound) {
            // Handle missing Content-Length header for POST requests
            setResponseStatus("411 Length Required");
            return;
        }
        std::size_t bodyPos = forBody.find("\r\n\r\n");
        if (bodyPos != std::string::npos) {
            // Extract the body
            std::string requestBody = forBody.substr(bodyPos + 4);
            // Write the body to a file
            _bodyFile = "BodyOfRequest.txt";
            std::ofstream outfile(_bodyFile, std::ofstream::binary);
            outfile.write(requestBody.c_str(), contentLength);
            outfile.close();
        }
        else {
            // Handle missing Content-Length header for POST requests
            setResponseStatus("411 Length Required");
            return ;
        }
    }
    setResponseStatus("200 OK");
}

std::string Request::getPath() const
{
    return this->_path;
}

std::string Request::getMethod() const
{
    return this->_method;
}

std::string Request::getHttpVersion() const
{
    return this->_httpVersion;
}

std::string Request::getResponseStatus() const
{
    return this->_responseStatus;
}

void Request::setResponseStatus(const std::string& status) {
    _responseStatus = status;
}

Request::~Request() {}
