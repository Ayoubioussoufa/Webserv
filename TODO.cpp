//TODO: Request still needs to be fixed in the file and the body when it comes to the POST method

/*
* *After parsing the HTTP request, your web server should typically perform the following steps to handle the request and generate an appropriate response:

! Request Validation and Routing:
Validate the parsed request to ensure it's well-formed and contains all required information.
Implement a routing mechanism to determine how to handle the request based on the request path, HTTP method, and other factors.
Map the request to a specific handler or controller based on the routing rules.

! Authentication and Authorization:
Authenticate the client if your application requires it. This might involve checking credentials, tokens, or session information.
Perform authorization checks to determine if the authenticated client has permission to access the requested resource or perform the requested action.

! Business Logic Processing:
Execute the appropriate business logic based on the request and the route.
If it's a GET request, retrieve data or generate a response based on the requested resource.
For POST and PUT requests, process incoming data and update the server's state or database accordingly.
Handle any other relevant HTTP methods and actions.

! Response Generation:
Create an HTTP response with the appropriate status code and headers.
Set the content type and length headers based on the response data.
Include any required cookies or session information.
Generate the response body, which could be HTML, JSON, XML, or any other content type.

! Error Handling:
Implement error handling to catch exceptions or unexpected situations during request processing.
Return meaningful error responses with appropriate status codes and error messages.

! Sending the Response:
Send the generated response back to the client using the established client socket.
Ensure the response is properly formatted as per the HTTP protocol.

! Logging and Monitoring:
Log important events and actions, both for debugging purposes and for monitoring server health.
Implement metrics and monitoring solutions to track server performance and usage.

! Clean Up:
Release any acquired resources, such as closing database connections or cleaning up temporary files.
Close the client socket if it's not designed for persistent connections (e.g., HTTP/1.1 with Keep-Alive).

!Repeat for Additional Requests:
Your server should continue listening for and handling incoming requests in a loop.

!Testing:
Implement unit tests and integration tests to verify the correctness of your server and its handling of various types of requests and scenarios.

!Optimization and Scaling:
As your application grows, optimize code and database queries for better performance.
Consider load balancing and horizontal scaling strategies to handle increased traffic.

!Documentation:
Maintain clear and up-to-date documentation for your API or web service, describing the available endpoints, request/response formats, and usage instructions.

!Security:
Implement security best practices, including input validation, protection against common web vulnerabilities (e.g., SQL injection, XSS), and regular security audits.
Remember that the specific steps and requirements may vary depending on your project's goals and technology stack. Building a web server is a complex task, and continuous development, testing, and maintenance are essential for a robust and reliable system.

*/

// jam3a kolchi
void	run(void)
{
	std::string	dot[3] = {".  ", ".. ", "..."};
	int			n = 0;

	while (1)
	{
		fd_set		reading_set;
		fd_set		writing_set;
		struct timeval	timeout;
		int				ret = 0;

		while (ret == 0)
		{
			timeout.tv_sec  = 1;
			timeout.tv_usec = 0;
			ft_memcpy(&reading_set, &_fd_set, sizeof(_fd_set));
			FD_ZERO(&writing_set);
			for (std::vector<int>::iterator it = _ready.begin() ; it != _ready.end() ; it++)
				FD_SET(*it, &writing_set);

			std::cout << "\rWaiting on a connection" << dot[n++] << std::flush;
			if (n == 3)
				n = 0;

			ret = select(_max_fd + 1, &reading_set, &writing_set, NULL, &timeout);
		}

		if (ret > 0)
		{
			for (std::vector<int>::iterator it = _ready.begin() ; ret && it != _ready.end() ; it++)
			{
				if (FD_ISSET(*it, &writing_set))
				{
					long	ret = _sockets[*it]->send(*it);

					if (ret == 0)
						_ready.erase(it);
					else if (ret == -1)
					{
						FD_CLR(*it, &_fd_set);
						FD_CLR(*it, &reading_set);
						_sockets.erase(*it);
						_ready.erase(it);
					}

					ret = 0;
					break;
				}
			}

			if (ret)
				std::cout << "\rReceived a connection !   " << std::flush;

			for (std::map<long, Server *>::iterator it = _sockets.begin() ; ret && it != _sockets.end() ; it++)
			{
				long	socket = it->first;

				if (FD_ISSET(socket, &reading_set))
				{
					long	ret = it->second->recv(socket);

					if (ret == 0)
					{
						it->second->process(socket, _config);
						_ready.push_back(socket);
					}
					else if (ret == -1)
					{
						FD_CLR(socket, &_fd_set);
						FD_CLR(socket, &reading_set);
						_sockets.erase(socket);
						it = _sockets.begin();
					}
					ret = 0;
					break;
				}
			}

			for (std::map<long, Server>::iterator it = _servers.begin() ; ret && it != _servers.end() ; it++)
			{
				long	fd = it->first;

				if (FD_ISSET(fd, &reading_set))
				{
					long	socket = it->second.accept();

					if (socket != -1)
					{
						FD_SET(socket, &_fd_set);
						_sockets.insert(std::make_pair(socket, &(it->second)));
						if (socket > _max_fd)
							_max_fd = socket;
					}
					ret = 0;
					break;
				}
			}
		}
		else
		{
			std::cerr << RED << "Problem with select !" << RESET << std::endl;
			for (std::map<long, Server *>::iterator it = _sockets.begin() ; it != _sockets.end() ; it++)
				it->second->close(it->first);
			_sockets.clear();
			_ready.clear();
			FD_ZERO(&_fd_set);
			for (std::map<long, Server>::iterator it = _servers.begin() ; it != _servers.end() ; it++)
				FD_SET(it->first, &_fd_set);
		}

		n = 0;
	}
}

// for port verification between servers
std::vector<t_listen>				Config::getAllListens() const {
	std::vector<t_listen>	ret;

	for (std::vector<ConfigServer>::const_iterator server = this->_servers.begin(); server != this->_servers.end(); server++) {
		std::vector<t_listen>	listenVec = server->getListen();
		for (std::vector<t_listen>::iterator listen = listenVec.begin(); listen != listenVec.end(); listen++) {
			std::vector<t_listen>::iterator i = ret.begin();
			for ( ; i != ret.end(); i++)
				if (listen->host == i->host && listen->port == i->port)
					break ;
			if (i == ret.end())
				ret.push_back(*listen);
		}
	}
	return ret;
}

// kat2mr port o server
int		Cluster::setup(void)
{
	std::vector<t_listen>	vect = _config.getAllListens();

	FD_ZERO(&_fd_set);
	_fd_size = vect.size();
	_max_fd = 0;

	for ( std::vector<t_listen>::const_iterator lstn = vect.begin() ; lstn != vect.end() ; lstn++ )
	{
		Server		serv(*lstn);
		long		fd;

		if (serv.setup() != -1)
		{
			fd = serv.getFD();
			FD_SET(fd, &_fd_set);
			_servers.insert(std::make_pair(fd, serv));
			if (fd > _max_fd)
				_max_fd = fd;
			std::cout << "Setting up " << lstn->host << ":" << lstn->port << "..." << std::endl;
		}
	}

	if (_max_fd == 0)
	{
		std::cerr << RED << "Could not setup cluster !" << RESET << std::endl;
		return (-1);
	}
	else
		return (0);
}