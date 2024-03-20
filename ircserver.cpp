#include "server.hpp"

void ft_error(std::string str)
{
    std::cerr << str << std::endl;
}

bool ft_isdigit(char *str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i]) == 0)
            return (false);
    }
    return (true);
}

bool argsCheck(int argc, char **argv)
{
    if (argc != 3) {
        ft_error("Usage: ./ircserver <port> <password>");
        return (false);
    }
    if (!ft_isdigit(argv[1])) {
        ft_error("Port must be a number");
        return (false);
    }
    if (argv[2] == NULL || *argv[2] == '\0') {
        ft_error("Password is null or empty");
        return (false);
    }
    return (true);
}

int main(int argc, char **argv)
{
    if (argsCheck(argc, argv) == false)
        return (1);
    server sv(atoi(argv[1]), atoi(argv[2]));


    // if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 1) {
    //     std::cout << "Connection failed" << std::endl;
    //     return (1);
    // }
    // std::cout << "Connected to server" << std::endl;
    // //receive message from server
    // char buffer[1024];
    // if (recv(sockfd, buffer, sizeof(buffer), 0) == -1) {
    //     std::cout << "Error in receiving message" << std::endl;
    //     return (1);
    // }
    // std::cout << "Message from server: " << buffer << std::endl;
    // close(sockfd);

    return (0);
}