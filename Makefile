CC = c++
NAME = irc
FLAGS = -std=c++98 #-Wall -Werror -Wextra
SRC = Executer.cpp \
		Parser.cpp \
		Client.cpp \
		Server.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)