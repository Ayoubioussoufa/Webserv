/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:27:53 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/23 13:53:52 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Request.hpp"

Request::Request() {
    std::string _ofile = GenerateFile();
}


// Function to parse an HTTP request
int Request::parseHttpRequest(const std::string& requestBuffer, int new_socket) {
    (void)new_socket;
    std::cout << "CHKE7777777777777777777" << std::endl;
    int i = processChunk(requestBuffer);
    std::cout << "CHKE7777777777777777777" << std::endl;
    (void)i;
    // if (!i)
    //     return parseRequest();
    return i;
}

size_t Request::customFind(const std::vector<char>& vec, const std::string& str, size_t start)
{
    size_t vectorSize = vec.size();
    size_t strLength = str.length();
    if (strLength == 0 || start >= vectorSize) {
        return std::string::npos;  // Return 'not found' if the input is invalid.
    }
    for (size_t i = start; i < vec.size(); ++i) {
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

int Request::countNumbersInString(const std::string& inputString) {
    int count = 0;
    for (size_t i = 0; i < inputString.length(); ++i) {
        if (std::isdigit(inputString[i])) {
            count++;
        }
    }
    return count;
}

int		Request::processChunk(const std::string &buffer)
{
    std::vector<char>	chunks;
    int i = 0;
    if (customFind(_header_before, "\n\r\n", 0) == std::string::npos)
    {
	    while (buffer[i])
            _header_before.push_back(buffer[i++]);
        size_t pos = customFind(_header_before, "\r\n\r\n", 0);
        if (pos != std::string::npos)
        {
            chunks = customSubstr(_header_before, pos + 4, _header_before.size());
            _header_before = customSubstr(_header_before, 0, pos + 4); // ! changeable
            for (std::vector<char>::iterator it = _header_before.begin(); it != _header_before.end(); it++)
            {
                std::cout << *it;
            }
            std::cout << "----------------" << std::endl;
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
    _body.insert(_body.end(), chunks.begin(), chunks.end());
    if (!_body.empty())
    {
        if (!_headers["Content-Length"].empty()) // ! possible segfault
        {
                std::string number = _headers["Content-Length"];
                size_t len = strtol(number.c_str(), NULL, 10) + 12;
                std::cout << len << "    " << "BODY SIZE : " << _body.size() << std::endl;
                if (len <= _body.size())
                {
                    std::cout << "---------------------------------------" << std::endl;
                    std::cout << "BEFORE +++++++++++++++++++++++++++++" << std::endl; 
                    for (std::vector<char>::iterator it = _body.begin(); it != _body.end(); it++)
                    {
                        std::cout << *it;
                    }
                    std::cout << std::endl;
                    processBody(_body);
                    // std::cout << "AFTER +++++++++++++++++++++++++++++" << std::endl; 
                    // for (std::vector<char>::iterator it = _body.begin(); it != _body.end(); it++)
                    // {
                    //     std::cout << *it;
                    // }
                    // std::cout << std::endl;
                    // std::cout << "+++++++++++++++++++++++++++++" << std::endl; 
                    // _header_before = _header_before + "\n\r\n" + _body + "\r\n\r\n";
                    // std::cout << _header_before << std::endl;
                    return 0;
                }
                else
                    return 1;
        }
    }
    else
        return 0;
    return 1;
}

std::string Request::GenerateFile() {
    std::string Base = "ABCDEFJHIGKLMNOPQRSTUVWXYZabcdefh12326544";
    std::string file;
    
    // Create the directory if it doesn't exist
    const char* dir_path = "/nfs/homes/aybiouss/Desktop/wepi/bodys/";
    if (mkdir(dir_path, 0777) != 0 && errno != EEXIST) {
        std::cerr << "Failed to create directory: " << strerror(errno) << std::endl;
        return "";  // Return an empty string to indicate failure
    }
    
    for (size_t i = 0; i < FILESIZE; i++) {
        file += Base[rand() % Base.size()];
    }
    _ofile = dir_path + file;
    return _ofile;
}


void    Request::processBody(std::vector<char>& vec)
{
    size_t i = 0;
    std::ofstream file(_ofile.c_str(), std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << strerror(errno) << std::endl;
        return;
    }

    while (i < vec.size()) {
        // Find the position of "\r\n" to get the chunk size
        size_t crlf_pos = customFind(vec, "\r\n", i);

        if (crlf_pos == std::string::npos) {
            std::cerr << "Error: Chunk size not found." << std::endl;
            break;
        }

        // Extract the chunk size string and convert it to an integer
        std::string chunk_size_str(vec.begin() + i, vec.begin() + crlf_pos);
        char* endptr;
        int chunk_size = strtol(chunk_size_str.c_str(), &endptr, 16);

        if (*endptr != '\0' || chunk_size < 0) {
            std::cerr << "Error: Invalid chunk size." << std::endl;
            break;
        }

        // Move the index past the "\r\n"
        i = crlf_pos + 2;

        if (chunk_size == 0) {
            // End of chunks
            break;
        }

        // Check if there is enough data left in the vector
        if (i + static_cast<size_t>(chunk_size) > vec.size()) {
            std::cerr << "Error: Incomplete chunk data." << std::endl;
            break;
        }

        // Write the chunk data to the file
        file.write(&vec[i], chunk_size);

        // Move past the "\r\n" at the end of the chunk
        i += chunk_size + 2;
    }
    std::cout << "File Name : " << _ofile << std::endl;
    file.close();
    // exit(1);
    // std::vector<char>	subchunk = customSubstr(vec, 0, 100);
	// std::string	body = "";
	// int			chunksize = strtol(&subchunk[0], NULL, 16);
	// size_t		i = 0;
	// while (chunksize)
	// {
	// 	i = customFind(vec, "\r\n", i) + 2;
	// 	if (i + static_cast<size_t>(chunksize) <= vec.size()) {
    //         body.insert(body.end(), vec.begin() + i, vec.begin() + i + chunksize);
    //         i += chunksize + 2;
    //         subchunk = customSubstr(vec, i, 100);
    //         chunksize = strtol(&subchunk[0], NULL, 16);
    //     } else {
    //         // Handle case where chunksize is larger than remaining data
    //         std::cerr << "Error: Invalid chunk size or incomplete data." << std::endl;
    //         break;
    //     }
	// }
    // std::ofstream file(_ofile.c_str(), std::ios::out | std::ios::binary);
    // if (file.is_open())
    // {
    //     file.write(&body[0], body.size());
    //     file.close();
    // }
    // else
    //     std::cerr << "Failed to open the file." << strerror(errno) << std::endl;
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
    std::string header = vectorCharToString(_header_before);
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
    return 1;
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

Request::Request(const Request& other)
    :   _method(other._method),
        _path(other._path),
        _httpVersion(other._httpVersion),
        _header_before(other._header_before),
        _body(other._body),
        _headers(other._headers),
        _bodyFile(other._bodyFile),
        _ofile(other._ofile) {}

Request& Request::operator=(const Request& other)
{
    if (this != &other)
    {
        _method = other._method;
        _path = other._path;
        _httpVersion = other._httpVersion;
        _header_before = other._header_before;
        _body = other._body;
        _headers = other._headers;
        _bodyFile = other._bodyFile;
        _ofile = other._ofile;
    }
    return *this;
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

const std::string& Request::getPath() const
{
    return this->_path;
}

const std::string& Request::getMethod() const
{
    return this->_method;
}

const std::string& Request::getHttpVersion() const
{
    return this->_httpVersion;
}

const std::string& Request::getResponseStatus() const
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