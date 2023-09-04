/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:09:26 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/04 17:33:41 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

class ConfigFile
{
    public:
        std::map<std::string, std::vector<std::string> > config;
        int    opening(char *file);
};