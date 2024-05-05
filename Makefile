SRC = src/Client.cpp src/Exception.cpp src/Server.cpp src/main.cpp src/Utils.cpp src/Channel.cpp
NAME = irc

all: compile run


compile:
	@echo "Compiling..."
	@c++ -std=c++98 -Wall -Wextra -Werror $(SRC) -o $(NAME)

run:
	@echo "Running..."
	@./$(NAME) 123 50000