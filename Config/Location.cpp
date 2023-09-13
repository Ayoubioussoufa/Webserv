/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:26:06 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/13 10:56:20 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Location.hpp"

// Location::Location();
// Location::Location(const CommonEntity& base, TokenVectsIter& begin, TokenVectsIter& end);
// Location::Location(const Location& other);
// Location&                    Location::operator=(const Location& other);
// void                         Location::InitPattern(std::string value);
// void                         Location::InitLimitExcept(std::string value);
// void                         Location::InitCgi(std::string value);
// void                         Location::InitUpload(std::string value);
// void                         Location::InitRedirect(std::string value);
// std::string                  Location::getPattren() const;
// std::vector<std::string>     Location::getLimit_except() const;
// std::map<std::string, std::string>             Location::getCgi() const;
// std::string                  Location::getUpload() const;
// std::string                  Location::getRedirect() const;
// Location::~Location();
// friend std::ostream& Location::operator<<(std::ostream& o, Location obj);

Location::Location() : _pattern_exists(false) {}
// !
Location::Location(const CommonEntity& base, TokenVectsIter& begin, TokenVectsIter& end)
    : CommonEntity(base), _pattern_exists(false)
{
    // Initialize the Location object using the provided iterators (assuming they point to strings).
    // This is a simplified example; you should adapt it based on your configuration format.
    while (begin != end)
    {
        std::string token = *begin;
        if (token == "pattern")
        {
            // Extract and set the pattern
            ++begin;
            if (begin != end)
            {
                InitPattern(*begin);
            }
        }
        else if (token == "limit_except")
        {
            // Extract and set limit_except values
            ++begin;
            while (begin != end && *begin != ";")
            {
                InitLimitExcept(*begin);
                ++begin;
            }
        }
        else if (token == "cgi")
        {
            // Extract and set CGI settings
            ++begin;
            if (begin != end)
            {
                InitCgi(*begin);
            }
        }
        else if (token == "upload")
        {
            // Extract and set upload
            ++begin;
            if (begin != end)
            {
                InitUpload(*begin);
            }
        }
        else if (token == "redirect")
        {
            // Extract and set redirect
            ++begin;
            if (begin != end)
            {
                InitRedirect(*begin);
            }
        }
        // Handle other tokens as needed
        ++begin;
    }
}

Location::Location(const Location& other)
    : CommonEntity(other), _pattern(other._pattern),
      _limit_except(other._limit_except),
      _cgi(other._cgi), _upload(other._upload),
      _redirect(other._redirect),
      _pattern_exists(other._pattern_exists) {}

Location& Location::operator=(const Location& other)
{
    if (this != &other)
    {
        CommonEntity::operator=(other);
        _pattern = other._pattern;
        _limit_except = other._limit_except;
        _cgi = other._cgi;
        _upload = other._upload;
        _redirect = other._redirect;
        _pattern_exists = other._pattern_exists;
    }
    return *this;
}

void Location::InitPattern(std::string value)
{
    _pattern = value;
    _pattern_exists = true;
}

void Location::InitLimitExcept(std::string value)
{
    _limit_except.push_back(value);
}

void Location::InitCgi(std::string value) // !
{
    // Implement this method to initialize CGI settings.
    // This is a simplified example; adjust it based on your configuration format.
    std::vector<std::string> cgiTokens;
    // Split the input value into individual CGI settings
    // based on your format (e.g., key=value pairs separated by space).
    // You may use a custom parsing logic here.
    
    // For demonstration, let's assume a simple space-separated key=value format.
    std::istringstream iss(value);
    std::string token;
    while (std::getline(iss, token, ' '))
    {
        // Split each token into key and value based on '=' separator.
        size_t equalPos = token.find('=');
        if (equalPos != std::string::npos)
        {
            std::string key = token.substr(0, equalPos);
            std::string value = token.substr(equalPos + 1);
            // Store the CGI setting in the _cgi map.
            _cgi[key] = value;
        }
        else
        {
            // Handle parsing error if needed.
            std::cerr << "Error parsing CGI setting: " << token << std::endl;
        }
    }
}

void Location::InitUpload(std::string value)
{
    _upload = value;
}

void Location::InitRedirect(std::string value)
{
    _redirect = value;
}

std::string Location::getPattren() const
{
    return _pattern;
}

std::vector<std::string> Location::getLimit_except() const
{
    return _limit_except;
}

std::map<std::string, std::string> Location::getCgi() const
{
    return _cgi;
}

std::string Location::getUpload() const
{
    return _upload;
}

std::string Location::getRedirect() const
{
    return _redirect;
}

Location::~Location() {}

friend std::ostream& operator<<(std::ostream& o, const Location obj);

std::ostream& operator<<(std::ostream& o, const Location obj)
{
    o << "Pattern: " << obj._pattern << std::endl;
    o << "Root: " << obj.getRoot() << std::endl;
    o << "Index: ";
    std::copy(obj.getIndex().begin(), obj.getIndex().end(), std::ostream_iterator<std::string>(o, ", "));
    o << std::endl;
    // Output other members as needed
    return o;
}