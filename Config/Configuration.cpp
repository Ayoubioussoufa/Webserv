/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:26:09 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/13 19:59:51 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Configuration.hpp"

Configuration::Configuration()
        : _client_max_body_size(0),  _AutoIndex(false), _root_exists(false),
        _host_exists(false), _port_exists(false) {}

Configuration::Configuration(TokenVectsIter begin, TokenVectsIter end)
    : _client_max_body_size(0),  _AutoIndex(false), _root_exists(false),
    _host_exists(false), _port_exists(false)
{
    initAttributes(begin, end);
}
// ! PAUSE HNAYA !!!!! 
void Configuration::initAttributes(TokenVectsIter begin, TokenVectsIter end)
{
    while (begin != end)
    {
        std::string token = *begin;
        if (token == "host")
        {
            ++begin;
            if (begin != end)
                InitHost(*begin);
        }
        else if (token == "port")
        {
            ++begin;
            if (begin != end)
                InitPort(*begin);
        }
        else if (token == "server_name")
        {
            ++begin;
            if (begin != end)
                InitServerName(*begin);
        }
        else if (token == "error_page")
        {
            ++begin;
            if (begin != end)
                InitErrorPage(*begin);
        }
        else if (token == "index")
        {
            // Handle location blocks
            ++begin;
            if (begin != end)
            {
                // Find the closing curly brace of the location block.
                TokenVectsIter endIt = std::find(begin, end, "}");
                // Create a Location object and add it to the vector.
                // Location location(*this, begin, endIt);
                // _locations.push_back(location);
                // Move the iterator to the next position after the location block.
                begin = endIt + 1;
            }
        }
        else
        {
            // Handle other configuration settings if needed.
            ++begin;
        }
    }
}

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

void Configuration::InitErrorPage(std::string value)
{
    // Implement this method to initialize error pages.
    // You would need to parse and store error pages based on your needs.
    int error_code;
    std::string error_page_path;

    // Parse the input string to extract error code and error page path
    std::istringstream iss(value);
    if (iss >> error_code >> error_page_path)
    {
        // Store the parsed values in the _error_pages map
        _error_pages[error_code] = error_page_path;
    }
    else
    {
        // Handle parsing error if needed
        std::cerr << "Error parsing error page: " << value << std::endl;
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
        std::cerr << "Error parsing client max body size: " << value << std::endl;
    }
}

void Configuration::InitAutoIndex(std::string value)
{
    // Implement this method to initialize the AutoIndex flag.
    // You would need to parse and store the flag based on your needs.

    // Convert the input string to lowercase for case-insensitive comparison
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    // Check if the value is "true" or "false"
    if (value == "true")
        _AutoIndex = true;
    else if (value == "false")
        _AutoIndex = false;
    else
    {
        // Handle parsing error if needed
        std::cerr << "Error parsing AutoIndex: " << value << std::endl;
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

// std::ostream& operator<<(std::ostream& o, const Configuration obj)
// {
//     o << "Host: " << obj.getHost() << std::endl;
//     o << "Port: " << obj.getPort() << std::endl;
//     o << "Server Name: " << obj.getServerNames() << std::endl;
    
//     // Output location blocks
//     std::vector<Location> locations = obj.getLocations();
//     for (size_t i = 0; i < locations.size(); ++i)
//     {
//         o << "Location Pattern: " << locations[i].getpattern() << std::endl;
//         // Output other location properties as needed
//     }
    
//     return o;
// }
// This code defines the Configuration class with member functions for initialization and access to its attributes. It also handles parsing location blocks and uses the Location class to store and manage them.

Configuration::~Configuration() {}
