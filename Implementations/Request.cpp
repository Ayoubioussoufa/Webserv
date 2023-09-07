/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:27:53 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/07 12:19:25 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Request.hpp"

Request::Request() {}

// Function to parse an HTTP request
void Request::parseHttpRequest(const std::string& requestBuffer, int new_socket) {

    // Split the request into lines
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
    while (std::getline(requestStream, line, '\n') && !line.empty()) {
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

                    if (endptr == headerValueCStr || *endptr != '\0' || parsedContentLength == ULONG_MAX) {
                        // Handle invalid Content-Length value
                        setResponseStatus("400 Bad Request--");
                        return;
                    }
                    contentLength = parsedContentLength;
                    isContentLengthFound = true;
                } catch (const std::exception& e) {
                    // Handle invalid Content-Length value
                    setResponseStatus("400 Bad Request*");
                    return;
                }
            }
        }
    }
    std::cout << "******************************" << std::endl;
    for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); ++it) {
            std::cout << it->first << it->second;
    }
    std::cout << std::endl;
    std::cout << "******************************" << std::endl;
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
            std::ofstream outfile("BodyOfRequest.txt", std::ofstream::binary);
            outfile.write(requestBody.c_str(), contentLength);
            outfile.close();
        }
        else {
            // Handle missing Content-Length header for POST requests
            setResponseStatus("411 Length Required");
            return ;
        }
    }
            // const char* contentLengthStr = _header["Content-Length"].c_str();
            // int contentLength = atoi(contentLengthStr);
            // char* buffer = new char[contentLength];
            // int bytesRead = 0;
            // int totalBytesRead = 0;
            // while (totalBytesRead < contentLength) {
            //     bytesRead = recv(new_socket, buffer + totalBytesRead, contentLength - totalBytesRead, 0);
            //     if (bytesRead <= 0) {
            //         perror("Read error");
            //         setResponseStatus("500 Internal Server Error");
            //         delete[] buffer;
            //         return;
            //     }
            //     totalBytesRead += bytesRead;
            //     std::cout << "Received " << totalBytesRead << " bytes out of " << contentLength << std::endl;
            // }
            // int bytesRead = recv(new_socket, buffer, contentLength, 0);
            // if (bytesRead < 0 || (bytesRead != contentLength && bytesRead != 0)) {
            //     // Handle an error reading the request body
            //     setResponseStatus("500 Internal Server Error");
            //     delete[] buffer;
            //     return ;
    setResponseStatus("200 OK");
    return ;
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
