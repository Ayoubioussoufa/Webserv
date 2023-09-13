/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:25:06 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/13 19:52:08 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/Servers.hpp"

//testing !!

int main(int ac, char *av[])
{
    if (ac > 2) {
        std::cerr << "Usage: " << av[0] << " [configuration_file]" << std::endl;
        return 1; // Exit with an error code
    }
    Servers configFile;
    if (ac == 1)
    {
        std::string file = "defaultconf_file";
        configFile.ConfigFileParse(file);
    }
    else
        configFile.ConfigFileParse(av[1]);
    // Socket s;
    // s.function();
    return 0;
}