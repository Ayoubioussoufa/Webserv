NAME = webserv
CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
PARSING = ConfigFile.cpp
IMPLEMENTATIONS = Socket.cpp
SRCS = $(addprefix Parsing/, $(PARSING)) \
		$(addprefix Implementations/, $(IMPLEMENTATIONS)) \
		main.cpp
OBJ = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)