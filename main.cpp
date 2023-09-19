/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:25:06 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/19 09:44:00 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Includes/Servers.hpp"
#include "./Includes/Socket.hpp"
//testing !!

void sigpipeHandle(int sig) { if(sig) {}}

int main(int ac, char *av[])
{
    if (argc == 1 || argc == 2) {
		try 
		{
            signal(SIGPIPE, sigpipeHandle);
            Servers configFile;
            std::string		config;
            config = (argc == 1 ? "default.conf" : argv[1]);
            configFile.ConfigFileParse(config);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return (1);
        }
    }
    else 
	{
		std::cerr << "Error: wrong arguments" << std::endl;
		return (1);
	}
}