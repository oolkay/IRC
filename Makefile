CC = c++
FLAGS = -std=c++98 -Wall -Wextra -Werror

all: compile

compile:
	$(CC) $(FLAGS) ircserver.cpp server.cpp client.cpp -o ircserver

run:
	./ircserver