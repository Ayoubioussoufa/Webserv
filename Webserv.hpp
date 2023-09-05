/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybiouss <aybiouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:25:03 by aybiouss          #+#    #+#             */
/*   Updated: 2023/09/05 16:37:50 by aybiouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include "./Includes/Socket.hpp"
#include "./Includes/Request.hpp"
#include "./Includes/CommonEntity.hpp"
#include "./Includes/ConfigFile.hpp"
#include "./Includes/Configuration.hpp"
#include "./Includes/Location.hpp"
#include "./Includes/Request.hpp"
#include "./Includes/Response.hpp"
#include <cstdlib>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>      // For input and output
#include <cstring>       // For string manipulation and error handling
#include <cstdlib>       // For standard library functions
#include <cerrno>        // For error numbers and error handling
#include <sys/socket.h>  // For socket functions
#include <netinet/in.h>  // For IP address structures and constants
#include <unistd.h>      // For read, write, and close functions
#include <fcntl.h>       // For file control options and non-blocking sockets
#include <sys/select.h>  // For the select function
#include <algorithm>     // For std::min function
#include <string>        // For string manipulation
