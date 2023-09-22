/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:59:09 by sben-ela          #+#    #+#             */
/*   Updated: 2023/09/22 11:54:31 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"
#include "../Includes/Configuration.hpp"
#include "Client.hpp"
#define BUFFER_SIZE 1024
class Client;
void    ft_Response(const Client &client);


