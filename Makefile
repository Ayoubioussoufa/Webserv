NAME = webserv
CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address
# IMPLEMENTATIONS = Socket.cpp Request.cpp Response.cpp $(addprefix ./Implementations/, $(IMPLEMENTATIONS))
CONFIG = Servers.cpp Configuration.cpp Location.cpp
SRCS = $(addprefix ./Config/, $(CONFIG)) \
		main.cpp
OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)
	$(RM) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)