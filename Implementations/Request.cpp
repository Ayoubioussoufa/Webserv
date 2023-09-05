/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:27:53 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/05 16:56:58 by aybiouss         ###   ########.fr       */
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
    requestLineStream >> _method >> _path >> _httpVersion;
    //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
    if (_path == "/favicon.ico") {
        // Handle it as needed (status), or simply return an empty request
        return ;
    }
    // Read and parse headers
    while (std::getline(requestStream, line) && !line.empty()) {
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 1);
            // Remove leading/trailing whitespaces from header values
            headerValue.erase(0, headerValue.find_first_not_of(" \t"));
            headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
            _header[headerName] = headerValue;
        }
    }

    if (_method == "POST") {
        bool foundContentLength = false;
        std::string contentLengthStr;
        for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); ++it) {
            if (it->first == "Content-Length") {
                foundContentLength = true;
                contentLengthStr = it->second;
                break;
            }
        }
        // Check if the Content-Length header is present
        if (foundContentLength) {
            const char* contentLengthStr = _header["Content-Length"].c_str();
            int contentLength = atoi(contentLengthStr);
            char* buffer = new char[contentLength];
            // int bytesRead = recv(new_socket, buffer, contentLength, 0);
            // if (bytesRead < 0 || (bytesRead != contentLength && bytesRead != 0)) {
            //     // Handle an error reading the request body
            //     setResponseStatus("500 Internal Server Error");
            //     delete[] buffer;
            //     return ;
            // } // ! IT BLOCKS THE REQUEST ! !!!
            (void)new_socket;
            requestStream.read(buffer, contentLength);
            // Save the request body to a file (or process it as needed)
            std::fstream outputFile("saved.txt"); // ! saved file isnt created either way
            if (outputFile.is_open()) {
                outputFile.write(buffer, contentLength);
                // std::cout << "The File : " << buffer << std::endl;
                // std::string line;
                // while (std::getline(outputFile, line)) {
                //     std::cout << line << std::endl;
                // }
                // std::cout << "************" << std::endl;
                outputFile.close();
            } // ? means the BODY isnt saved in the file !!
            delete[] buffer;
        } else {
            // Handle missing Content-Length header for POST requests
            setResponseStatus("411 Length Required");
            return ;
        }
    }
    // Set a default success response status for the request
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
