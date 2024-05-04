SRC = src/Client.cpp src/Exception.cpp src/Server.cpp src/main.cpp
NAME = irc

all: compile


compile:
	@echo "Compiling..."
	@c++ -std=c++98 -Wall -Wextra -Werror $(SRC) -o $(NAME)