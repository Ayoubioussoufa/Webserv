/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:22:39 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/05 09:40:26 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Configuration
{
    public:
        typedef void (Configuration::*methods)(std::string);
    private:
        std::string                                      _host;
        std::string                                      _port;
        bool                                             _host_exists;
        bool                                             _port_exists;
        // static std::map<std::string, std::string>     _host_port_map;
        std::string                                      _server_name;
        std::vector<Location>                            _locations;
    public:
        Configuration();
        // Configuration operator()(std::string host, std::string  port);
        Configuration(TokenVectsIter& begin, TokenVectsIter& end);
        void initAttributes(TokenVectsIter& begin, TokenVectsIter& end);
        Configuration(const Configuration& other);
        Configuration& operator=(const Configuration& other);
        void InitHost(std::string value);
        void InitPort(std::string value);
        void InitServerName(std::string value);
        // static    void check_dup(std::string host, std::string port);
        std::string                 getHost() const;
        std::string                    getPort() const;
        std::string    getServerNames() const;
        std::vector<Location>        getLocations() const;
        friend std::ostream& operator<<(std::ostream& o, Configuration obj);
        ~Configuration();
};