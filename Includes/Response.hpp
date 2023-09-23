/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:34:48 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/23 11:03:39 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

class Response : public Request
{
    public:
        Response();
        Response(const Response& other);
        Response& operator=(const Response& other);
        void    Function();
        const char* get_content_type(const char* path);
        ~Response();
};
