/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:54:36 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/18 17:20:09 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Configuration.hpp"
#include "Socket.hpp"

class Servers
{
    private:
        std::vector<Configuration>   _servers;
    public:
        Servers() {};
        int    ConfigFileParse(std::string file);
        void printServerData() const;
        int AllServers();
        ~Servers() {};
};