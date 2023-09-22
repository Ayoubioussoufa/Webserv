/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:27:53 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/22 19:42:13 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Request.hpp"

Request::Request() {}

// Function to parse an HTTP request
int Request::parseHttpRequest(const std::string& requestBuffer, int new_socket) {
    (void)new_socket;
    // std::cout << requestBuffer.find("Transfer-Encoding: chunked") << std::endl;
    // std::cout << requestBuffer << std::endl;
    int i = processChunk(requestBuffer);
    (void)i;
    // if (!i)
    //     return parseRequest();
    return i;
}

size_t Request::customFind(const std::vector<char>& vec, const std::string& str)
{
    if (vec.empty() || str.empty())
        return std::string::npos; // Not found
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == str[0])
        { // If the first character matches
            bool found = true;
            for (size_t j = 0; j < str.length(); ++j) {
                if (i + j >= vec.size() || vec[i + j] != str[j])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i; // Found at position i
        }
    }
    return std::string::npos; // Not found
}

std::vector<char> Request::customSubstr(const std::vector<char>& vec, size_t start, size_t length)
{
    std::vector<char> result;

    if (start >= vec.size()) {
        return result; // Return an empty vector if the start position is out of bounds
    }
    for (size_t i = start; i < start + length && i < vec.size(); ++i) {
        result.push_back(vec[i]);
    }
    return result;
}

int		Request::processChunk(const std::string &buffer)
{
    //_outfile.open(_filename.c_str(), std::ios::out | std::ios::binary);
    std::vector<char>	chunks; // chunks f file
    // nrd requests vector and check in function of the socket (fd)
    if (customFind(_header_before, "\n\r\n") == std::string::npos)
    {
        int i = 0;
	    while (buffer[i])
            _header_before.push_back(buffer[i++]);
        size_t pos = customFind(_header_before, "\r\n\r\n");
        if (pos != std::string::npos)
        {
            chunks = customSubstr(_header_before, pos + 4, _header_before.size());
            _header_before = customSubstr(_header_before, 0, pos + 4); // ! changeable
            for (std::vector<char>::iterator it = _header_before.begin(); it != _header_before.end(); it++)
            {
                std::cout << *it;
            }
            std::cout << "----------------" << std::endl;
            for (std::vector<char>::iterator it = chunks.begin(); it != chunks.end(); it++)
            {
                std::cout << *it;
            }
            std::cout << std::endl;
            int j = parseHeaders();
            if (!j)
                return 0;
        }
    }
    else
	{
        while (buffer[i])
            chunks.push_back(buffer[i++]);
    }
    if (!chunks.empty())
    {
        processBody(chunks);
    }
    // _body += chunks;
    // std::cout << "********************************" << std::endl;
    // std::cout << _header_before << std::endl;
    // std::cout << "------------------------------------------" << std::endl;
    // std::cout << _body << " WOW " << _body.length() << std::endl;
    // std::cout << "********************************" << std::endl;
    /*if (!_body.empty())
    {
        size_t pos = _header_before.find("Content-Length: ");
        // std::cout << "Content length pos : " << pos << std::endl;
        if (pos != std::string::npos)
        {
            // std::cout << "********************************" << std::endl;
            pos += strlen("Content-length:"); // !7iydt espace hnaya might add it later
            // std::cout << "Content length pos skipped : " << pos << std::endl;
            size_t end_pos = _header_before.find("\r", pos);
            // std::cout << pos << "   " << end_pos << std::endl;
            // std::cout << "********************************" << std::endl;
            if (end_pos != std::string::npos)
            {
                std::string length = _header_before.substr(pos, end_pos - pos);
                size_t len = strtol(length.c_str(), NULL, 10);
                // std::cout << "Content-Length as integer: " << len << std::endl;
                processBody();
                // std::cout << _body.length() << std::endl;
                if (len == _body.length())
                {
                    ;
                    // std::cout << "+++++++++++++++++++++++++++++" << std::endl; 
                    // std::cout << _body << std::endl << std::endl;
                    // _header_before = _header_before + "\n\r\n" + _body + "\r\n\r\n";
                    // std::cout << _header_before << std::endl;
                    // std::cout << "+++++++++++++++++++++++++++++" << std::endl; 
                    return 0;
                }
                else
                    return 1;
            }
        }
    }
    else
        return 0;*/
    return 1;
}

void    Request::processBody(std::vector<char>& vec)
{
    std::vector<char>	subchunk = customSubstr(vec, 0, 100);
	std::string	body = "";
	int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	size_t		i = 0;
	while (chunksize)
	{
		i = _body.customFind("\r\n", i) + 2;
		body += _body.substr(i, chunksize);
		i += chunksize + 2;
		subchunk = _body.substr(i, 100);
		chunksize = strtol(subchunk.c_str(), NULL, 16);
	}
    if (_outfile.is_open())
    {
        _outfile << body;
    }
}

    // std::string body = _body;
    // _body.clear();
    // std::istringstream ss(body);
    // std::string line;
    // while (std::getline(ss, line) && !line.empty())
    // {
    //     int length = strtol(line.c_str(), NULL, 16);
    //     if (length)
    //     {
    //         if (std::getline(ss, line) && !line.empty())
    //         {
                
    //         }
    //     }
    // }

int    Request::parseHeaders()
{
    string header = vectorCharToString(_header_before);
    std::istringstream requestStream(header);
    std::string line;

    // Read the first line (request line)
    if (!std::getline(requestStream, line)) {
        // Handle an empty or incomplete request
        setResponseStatus("400 Bad Request");
        return 0;
    }
    std::istringstream requestLineStream(line);
    if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
        // Handle invalid request line
        setResponseStatus("400 Bad Request");
        return 0;
    }
    //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
    if (_path == "/favicon.ico") {
        // Handle it as needed (status), or simply return an empty request
        return 0;
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
            _headers[headerName] = headerValue;
        }
    }
    if (_method == "GET")
        return 0;
}

std::string Request::vectorCharToString(const std::vector<char>& vec)
{
    std::string result;
    result.reserve(vec.size()); // Reserve space for efficiency
    for (size_t i = 0; i < vec.size(); ++i) {
        result.push_back(vec[i]);
    }
    return result;
}

/*int Request::parseRequest()
{
    // std::cout << "----------------+++++++++++++++++++++++++++" << std::endl;
    // std::cout << _header_before << std::endl;
    // std::cout << "----------------+++++++++++++++++++++++++++" << std::endl;
    // return 1;
    std::istringstream requestStream(_header_before);
    std::string line;

    // Read the first line (request line)
    if (!std::getline(requestStream, line)) {
        // Handle an empty or incomplete request
        setResponseStatus("400 Bad Request");
        return 0;
    }
    std::istringstream requestLineStream(line);
    if (!(requestLineStream >> _method >> _path >> _httpVersion)) {
        // Handle invalid request line
        setResponseStatus("400 Bad Request");
        return 0;
    }
    //This splitting is achieved by using the >> operator, which is used to extract values from the input stream (requestLineStream in this case) based on whitespace (spaces or tabs) as the delimiter.
    if (_path == "/favicon.ico") {
        // Handle it as needed (status), or simply return an empty request
        return 0;
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
            _headers[headerName] = headerValue;
            if (_method == "POST" && headerName == "Content-Length")
            {
                try {
                    char* endptr;
                    const char* headerValueCStr = headerValue.c_str();
                    unsigned long parsedContentLength = strtoul(headerValueCStr, &endptr, 10);

                    if (parsedContentLength == ULONG_MAX) { endptr == headerValueCStr || *endptr != '\0' ||
                        // Handle invalid Content-Length value
                        setResponseStatus("400 Bad Request");
                        return 0;
                    }
                    contentLength = parsedContentLength;
                    isContentLengthFound = true;
                } catch (const std::exception& e) {
                    // Handle invalid Content-Length value
                    setResponseStatus("400 Bad Request");
                    return 0;
                }
            }
        }
    }
    // std::cout << "*******************" << std::endl;
    // for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
    // {
    //     std::cout << it->first << " " << it->second << std::endl;
    // }
    // std::cout << std::endl;
    // std::cout << "*******************" << std::endl;
    // (void)new_socket;
    if (_method == "POST") {
        if (!isContentLengthFound) {
            // Handle missing Content-Length header for POST requests
            setResponseStatus("411 Length Required");
            return 0;
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
            return 0;
        }
    }
    setResponseStatus("200 OK");
    return (0);
}*/

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


    // if ()
	// std::string	subchunk = chunks.substr(0, 100);
	// std::string	body = "";
	// int			chunksize = strtol(subchunk.c_str(), NULL, 16);
	// size_t		i = 0;
	// while (chunksize)
	// {
	// 	i = chunks.find("\r\n", i) + 2;
	// 	body += chunks.substr(i, chunksize);
	// 	i += chunksize + 2;
	// 	subchunk = chunks.substr(i, 100);
	// 	chunksize = strtol(subchunk.c_str(), NULL, 16);
	// }
	// std::string requestBuffer = head + "\r\n\r\n" + body + "\r\n\r\n"; // NJM3Ha kamla onsiftha l constructor again and clear the old one
    // std::cout << requestBuffer << std::endl;
    // ! if content length == body.size means the request is done !
    // return 0;