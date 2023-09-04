/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:25:06 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/04 17:34:03 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/ConfigFile.hpp"
#include "./Includes/Socket.hpp"

//testing !!

int main(int ac, char *av[])
{
    if (ac > 2) {
        std::cerr << "Usage: " << av[0] << " [configuration_file]" << std::endl;
        return 1; // Exit with an error code
    }
    else if (ac == 2)
    {
        ConfigFile configFile;
        configFile.opening(av[1]);
        for (std::map<std::string, std::vector<std::string> >::iterator it = configFile.config.begin(); it != configFile.config.end(); it++) {
            std::cout << it->first << "   ";
            // for (size_t i = 0; i < it->second.size(); i++) {
            //     std::cout << it->second[i] << " ";
            // }
            std::cout << std::endl;
        }
        std::cout << "------------------" << std::endl;
        Socket s;
        s.function();
    }
    else
    {
        ;
        //parse default path
    }
    return 0;
}