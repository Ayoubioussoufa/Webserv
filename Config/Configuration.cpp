/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:26:09 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/14 12:04:51 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Configuration.hpp"

Configuration::Configuration()
        : _client_max_body_size(0),  _AutoIndex(false), _root_exists(false),
        _host_exists(false), _port_exists(false) {}

std::vector<std::string>    Configuration::Tokenization(std::string line)
{
    std::vector<std::string> result;
    std::istringstream iss(line);
    std::string token;
    
    while (std::getline(iss, line)) {
        std::istringstream line_stream(line);
        std::string token;
        while (std::getline(line_stream, token, ' ') || std::getline(line_stream, token, '\t')) {
            if (!token.empty()) {
                result.push_back(token);
            }
        }
    }
    return result;
}

bool Configuration::isStringAllDigits(const std::string& str) {
    // Find the first character that is not a digit
    size_t pos = str.find_first_not_of("0123456789");

    // If pos is equal to std::string::npos, it means there are no non-digit characters
    return (pos == std::string::npos);
}

Configuration::Configuration(TokenVectsIter begin, TokenVectsIter end)
    : _client_max_body_size(0),  _AutoIndex(false), _root_exists(false),
    _host_exists(false), _port_exists(false)
{
    while (begin != end)
    {
        std::string line = *begin;
        std::vector<std::string> token = Tokenization(line);
        if (token[0] == "host" && token.size() == 2)
        {
            ++begin;
            InitHost(token[1]);
        }
        else if (token[0] == "listen")
        {
            ++begin;
            if (isStringAllDigits(token[1]) && token.size() == 2 && atoi(token[1].c_str()) <= 65635)
                InitPort(token[1]);
            else
                throw std::string("Invalid port number"); // !error in config file
        }
        else if (token[0] == "server_name")
        {
            ++begin;
            if (token.size() == 2)
                InitServerName(token[1]);
            else
                throw std::string("Invalid server name"); //! error
        }
        else if (token[0] == "error_page")
        {
            ++begin;
            if (token.size() == 3)
                InitErrorPage(token[1], token[2]);
            else
                throw std::string("Invalid error page arguments"); // ! error
        }
        else if (token[0] == "location")
        {
            ++begin;
            if (begin != end && token.size() == 2)
            {
                // Find the closing curly brace of the location block.
                TokenVectsIter endIt = std::find(begin, end, "}");
                // Create a Location object and add it to the vector.
                Location location(begin, endIt);
                _locations.push_back(location);
                // Move the iterator to the next position after the location block.
                begin = endIt + 1;
            }
            else
                throw std::string("Invalid location !"); //!error
        }
        else
            ++begin;
        // !... LIST GOES ON
    }
}
// ! PAUSE HNAYA !!!!! 

Configuration::Configuration(const Configuration& other)
    : _root(other._root), _host(other._host), _index(other._index),
      _error_pages(other._error_pages), _client_max_body_size(other._client_max_body_size),
      _AutoIndex(other._AutoIndex), _root_exists(other._root_exists), _port(other._port),
      _host_exists(other._host_exists), _port_exists(other._port_exists),
      _server_name(other._server_name), _locations(other._locations) {}

Configuration& Configuration::operator=(const Configuration& other)
{
    if (this != &other)
    {
        _host = other._host;
        _port = other._port;
        _host_exists = other._host_exists;
        _port_exists = other._port_exists;
        _server_name = other._server_name;
        _locations = other._locations;
        _root = other._root;
        _index = other._index;
        _error_pages = other._error_pages;
        _client_max_body_size = other._client_max_body_size;
        _AutoIndex = other._AutoIndex;
        _root_exists = other._root_exists;
    }
    return *this;
}

void Configuration::InitHost(std::string value)
{
    _host = value;
    _host_exists = true;
}

void Configuration::InitPort(std::string value)
{
    _port = value;
    _port_exists = true;
}

void Configuration::InitServerName(std::string value)
{
    _server_name = value;
}

void Configuration::InitRoot(std::string value)
{
    _root = value;
    _root_exists = true;
}
void Configuration::InitIndex(std::string value)
{
    _index.push_back(value);
}

void Configuration::InitErrorPage(std::string code, std::string path)
{
    // Implement this method to initialize error pages.
    // You would need to parse and store error pages based on your needs.
    int error_code = atoi(code.c_str());
    std::string error_page_path = path;
    if (error_code >= 100 && error_code <= 599)
    {
        // Store the parsed values in the _error_pages map
        _error_pages[error_code] = error_page_path;
    }
    else
    {
        // Handle parsing error if needed
        std::string str = "Error parsing error page: " + code + " ";
        throw std::string(str.append(path));
        // ! throw exception ?
    }
}

void Configuration::InitClienBodySize(std::string value)
{
    // Implement this method to initialize client max body size.
    // You would need to parse and store the size based on your needs.

    // Parse the input string as an integer
    std::istringstream iss(value);
    size_t client_max_body_size;
    if (iss >> client_max_body_size)
    {
        // Store the parsed value in the _client_max_body_size member
        _client_max_body_size = client_max_body_size;
    }
    else
    {
        // Handle parsing error if needed
        std::string str = "Error parsing client max body size: ";
        throw std::string(str.append(value));
        // ! throw exception ?
    }
}

void Configuration::InitAutoIndex(std::string value)
{
    // Implement this method to initialize the AutoIndex flag.
    // You would need to parse and store the flag based on your needs.

    // Convert the input string to lowercase for case-insensitive comparison
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    // Check if the value is "true" or "false"
    if (value == "on")
        _AutoIndex = true;
    else if (value == "off")
        _AutoIndex = false;
    else
    {
        // Handle parsing error if needed
        std::cerr << "Error parsing AutoIndex: " << value << std::endl;
        // ! throw exception ?
    }
}

std::string Configuration::getRoot() const
{
    return _root;
}

std::vector<std::string> Configuration::getIndex() const
{
    return _index;
}

std::map<int, std::string> Configuration::getErrorPages() const
{
    return _error_pages;
}

size_t Configuration::getClientMaxBodySize() const
{
    return _client_max_body_size;
}

bool Configuration::getAutoIndex() const
{
    return _AutoIndex;
}

std::string Configuration::getHost() const
{
    return _host;
}

std::string Configuration::getPort() const
{
    return _port;
}

std::string Configuration::getServerNames() const
{
    return _server_name;
}

std::vector<Location> Configuration::getLocations() const
{
    return _locations;
}

// friend std::ostream& operator<<(std::ostream& o, const Configuration obj);

std::ostream& operator<<(std::ostream& o, const Configuration obj)
{
    o << "Host: " << obj.getHost() << std::endl;
    o << "Port: " << obj.getPort() << std::endl;
    o << "Server Name: " << obj.getServerNames() << std::endl;
    
    // Output location blocks
    std::vector<Location> locations = obj.getLocations();
    for (size_t i = 0; i < locations.size(); ++i)
    {
        o << "Location Pattern: " << locations[i].getpattern() << std::endl;
        // Output other location properties as needed
    }
    
    return o;
}
// This code defines the Configuration class with member functions for initialization and access to its attributes. It also handles parsing location blocks and uses the Location class to store and manage them.

Configuration::~Configuration() {}
