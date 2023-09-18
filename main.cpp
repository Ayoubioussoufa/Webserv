/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:25:06 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/18 18:45:34 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/Servers.hpp"
#include "./Includes/Socket.hpp"
//testing !!

void sigintHandler(int signal) {
    std::cout << "CTRL+C pressed. Exiting gracefully..." << std::endl;
    // Perform cleanup or exit the program
    exit(signal);
}

int main(int ac, char *av[])
{
    if (ac > 2) {
        std::cerr << "Usage: " << av[0] << " [configuration_file]" << std::endl;
        return 1; // Exit with an error code
    }
    try {
        Servers configFile;
        if (ac == 1)
        {
            std::string file = "defaultconf_file";
            configFile.ConfigFileParse(file);
        }
        else
            configFile.ConfigFileParse(av[1]);
        configFile.printServerData();
        // Socket s;
        // s.function();
    } catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}