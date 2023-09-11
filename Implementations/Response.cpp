/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:27:48 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/11 17:28:29 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Response.hpp"

// ? to know what content type 
const char* Response::get_content_type(const char* path)
{
    const char *last_dot = strrchr(path, '.');
    if (last_dot) {
    if (strcmp(last_dot, ".css") == 0) return "text/css";
    if (strcmp(last_dot, ".csv") == 0) return "text/csv";
    if (strcmp(last_dot, ".gif") == 0) return "image/gif";
    if (strcmp(last_dot, ".htm") == 0) return "text/html";
    if (strcmp(last_dot, ".html") == 0) return "text/html";
    if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
    if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
    if (strcmp(last_dot, ".js") == 0) return "application/javascript";
    if (strcmp(last_dot, ".json") == 0) return "application/json";
    if (strcmp(last_dot, ".png") == 0) return "image/png";
    if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
    if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
    if (strcmp(last_dot, ".txt") == 0) return "text/plain";
    }
    return "application/octet-stream";
} 


void    Response::Function()
{
    // ! the structure of it
    if (_method == "GET")
    {
        if (_path) {
            if () { /*Im gonna search if the path is on the configfile*/
                ;
            } else {
                return setResponseStatus("404 Not Found Response.");
            }
        } else {
            return setResponseStatus("400 Bad Request Response.");
        }
    }
    else if (_method == "POST")
    {
        ;
    }
    else if (_method == "DELETE")
    {
        ;
    }
    else
    {
        ;
    }

    // for (std::map<std::string, std::string>::iterator it = _header.begin(); it != _header.end(); it++)
    // {
    //     std::cout << it->first << it->second << std::endl;
    // }
}