/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:26:06 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/14 17:26:50 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Location.hpp"

Location::Location() : _pattern_exists(false) {}

std::vector<std::string>    Location::Tokenizations(std::string line)
{
    std::vector<std::string> result;
    std::istringstream iss(line);
    std::string token;
    
    while (iss >> token) {
        result.push_back(token);
    }
    return result;
}

Location::Location(std::string path, TokenVectsIter& begin, TokenVectsIter& end) : _pattern_exists(false)
{
    // Initialize the Location object using the provided iterators (assuming they point to strings).
    // This is a simplified example; you should adapt it based on your configuration format.
    if (!path.empty())
        InitPattern(path);
    while (begin != end)
    {
        std::string line = *begin;
        std::vector<std::string> token = Tokenizations(line);
        if (token[0] == "limit_except")
        {
            // Extract and set limit_except values
            ++begin;
            size_t i = 1;
            if (begin == end)
                throw std::string("Invalid limit_except");
            while (i < token.size())
            {
                InitLimitExcept(token[i]);
                i++;
            }
        }
        else if (token[0] == "autoindex")
        {
            ++begin;
            if (begin != end && token.size() == 2)
                InitAutoIndex(token[1]);
            else
                throw std::string("Invalid autoindex");
        }
        else if (token[0] == "client_body_size")
        {
            ++begin;
            if (begin != end && token.size() == 2)
                InitClientBodySize(token[1]);
            else
                throw std::string("Invalid Client body size");
        }
        else if (token[0] == "root")
        {
            ++begin;
            if (begin != end && token.size() == 2)
                InitRoot(token[1]);
            else
                throw std::string("Invalid root");
        }
        else if (token[0] == "index")
        {
            ++begin;
            if (begin != end && token.size() == 2)
                InitIndex(token[1]);
            else
                throw std::string("Invalid Index");
        }
        else if (token[0] == "cgi")
        {
            // Extract and set CGI settings
            ++begin;
            if (token.size() == 3 && begin != end)
                InitCgi(token[1], token[2]);
            else
                throw std::string("Invalid cgi arguments");
        }
        else if (token[0] == "upload_path")
        {
            // Extract and set upload
            ++begin;
            if (begin != end && token.size() == 2)
                InitUpload(token[1]);
            else
                throw std::string("Invalid upload path");
        }
        else if (token[0] == "error_page")
        {
            ++begin;
            if (token.size() == 3)
                InitErrorPage(token[1], token[2]);
            else
                throw std::string("Invalid error page arguments");
        }
        else if (token[0] == "redirect")
        {
            // Extract and set redirect
            ++begin;
            if (begin != end)
            {
                InitRedirect(token[1]);
            }
        }
        // Handle other tokens as needed
        ++begin;
    }
}

Location::Location(const Location& other)
    : _pattern(other._pattern),
      _limit_except(other._limit_except),
      _cgi(other._cgi), _upload(other._upload),
      _redirect(other._redirect),
      _pattern_exists(other._pattern_exists) {}

Location& Location::operator=(const Location& other)
{
    if (this != &other)
    {
        _pattern = other._pattern;
        _limit_except = other._limit_except;
        _cgi = other._cgi;
        _upload = other._upload;
        _redirect = other._redirect;
        _pattern_exists = other._pattern_exists;
    }
    return *this;
}

void Location::InitIndex(std::string value)
{
    _index = value;
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

void Location::InitErrorPage(std::string code, std::string path)
{
    // Implement this method to initialize error pages.
    // You would need to parse and store error pages based on your needs.
    int error_code = atoi(code.c_str());
    std::string error_page_path = path;
    if ((error_code >= 100 && error_code <= 599) && !path.empty())
    {
        // Store the parsed values in the _error_pages map
        _error_pages[error_code] = error_page_path;
    }
    else
    {
        // Handle parsing error if needed
        std::string str = "Error parsing error page: " + code + " ";
        throw std::string(str.append(path));
    }
}

void Location::InitCgi(std::string path, std::string lang)
{
    // Implement this method to initialize CGI settings.
    // This is a simplified example; adjust it based on your configuration format.
    // Split the input value into individual CGI settings
    // based on your format (e.g., key=value pairs separated by space).
    // You may use a custom parsing logic here.
    
    // For demonstration, let's assume a simple space-separated key=value format.
        if (!lang.empty() && !path.empty())
        {
            // Store the CGI setting in the _cgi map.
            _cgi[lang] = path;
        }
        else
        {
            // Handle parsing error if needed.
            std::string str = "Error parsing CGI setting: " + path + " ";
            throw std::string(str.append(lang));
        }
}

void Location::InitRoot(std::string value)
{
    _root = value;
    _root_exists = true;
}

void Location::InitClientBodySize(std::string value)
{
    // Use a stringstream to parse the input string as an integer
    std::istringstream iss(value);
    int client_max_body_size = 0;

    // Attempt to extract an integer from the string
    if (!(iss >> client_max_body_size))
    {
        // Handle parsing error if the extraction fails
        std::string error_msg = "Error parsing client max body size: " + value;
        throw std::string(error_msg);
    }

    // Check if the parsed value is valid (greater than zero)
    if (client_max_body_size < 0)
    {
        std::string error_msg = "Invalid client max body size: " + value;
        throw std::string(error_msg);
    }

    // Store the parsed value in the _client_max_body_size member
    _client_max_body_size = static_cast<size_t>(client_max_body_size);
}

void Location::InitAutoIndex(std::string value)
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
        std::string error_msg = "Error parsing AutoIndex: " + value;
        throw std::string(error_msg);
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

std::string Location::getpattern() const
{
    return _pattern;
}

std::string Location::getRoot() const
{
    return _root;
}

size_t Location::getClientMaxBodySize() const
{
    return _client_max_body_size;
}

bool Location::getAutoIndex() const
{
    return _AutoIndex;
}

std::vector<std::string> Location::getLimit_except() const
{
    return _limit_except;
}

std::string Location::getIndex() const
{
    return _index;
}

std::map<std::string, std::string> Location::getCgi() const
{
    return _cgi;
}

std::map<int, std::string> Location::getErrorPages() const
{
    return _error_pages;
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

// friend std::ostream& operator<<(std::ostream& o, const Location obj);

std::ostream& operator<<(std::ostream& o, Location obj)
{
    o << "Pattern: " << obj._pattern << std::endl;
    o << "Uploads: " << obj.getUpload() << std::endl;
    o << "Index: ";
    for (size_t i = 0; i < obj.getIndex().size(); ++i)
    {
        o << obj.getIndex()[i];
        if (i < obj.getIndex().size() - 1)
        {
            o << ", "; // Add a comma if it's not the last element
        }
    }
    o << std::endl;
    // std::copy(obj.getIndex().begin(), obj.getIndex().end(), std::ostream_iterator<std::string>(o, ", "));
    // Output other members as needed
    return o;
}