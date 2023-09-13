/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:26:09 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/13 10:59:06 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Configuration.hpp"

// Configuration::Configuration();
// // Configuration operator()(std::string host, std::string  port);
// Configuration::Configuration(TokenVectsIter& begin, TokenVectsIter& end);
// void Configuration::initAttributes(TokenVectsIter& begin, TokenVectsIter& end);
// Configuration::Configuration(const Configuration& other);
// Configuration& Configuration:: operator=(const Configuration& other);
// void Configuration::InitHost(std::string value);
// void Configuration::InitPort(std::string value);
// void Configuration::InitServerName(std::string value);
// // static    void check_dup(std::string host, std::string port);
// std::string                 Configuration::getHost() const;
// std::string                    Configuration::getPort() const;
// std::string    Configuration::getServerNames() const;
// std::vector<Location>        Configuration::getLocations() const;
// friend std::ostream& Configuration::operator<<(std::ostream& o, Configuration obj);
// Configuration::~Configuration();

Configuration::Configuration() : _host_exists(false), _port_exists(false) {}

Configuration::Configuration(TokenVectsIter& begin, TokenVectsIter& end)
    : _host_exists(false), _port_exists(false)
{
    initAttributes(begin, end);
}

void Configuration::initAttributes(TokenVectsIter& begin, TokenVectsIter& end)
{
    while (begin != end)
    {
        std::string token = *begin;
        if (token == "host")
        {
            ++begin;
            if (begin != end)
            {
                InitHost(*begin);
            }
        }
        else if (token == "port")
        {
            ++begin;
            if (begin != end)
            {
                InitPort(*begin);
            }
        }
        else if (token == "server_name")
        {
            ++begin;
            if (begin != end)
            {
                InitServerName(*begin);
            }
        }
        else if (token == "location")
        {
            // Handle location blocks
            ++begin;
            if (begin != end)
            {
                // Find the closing curly brace of the location block.
                TokenVectsIter endIt = std::find(begin, end, "}");
                // Create a Location object and add it to the vector.
                Location location(*this, begin, endIt);
                _locations.push_back(location);
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
    : _host(other._host), _port(other._port),
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

friend std::ostream& operator<<(std::ostream& o, const Configuration obj);

std::ostream& operator<<(std::ostream& o, const Configuration obj)
{
    o << "Host: " << obj.getHost() << std::endl;
    o << "Port: " << obj.getPort() << std::endl;
    o << "Server Name: " << obj.getServerNames() << std::endl;
    
    // Output location blocks
    std::vector<Location> locations = obj.getLocations();
    for (size_t i = 0; i < locations.size(); ++i)
    {
        o << "Location Pattern: " << locations[i].getPattren() << std::endl;
        // Output other location properties as needed
    }
    
    return o;
}
// This code defines the Configuration class with member functions for initialization and access to its attributes. It also handles parsing location blocks and uses the Location class to store and manage them.





