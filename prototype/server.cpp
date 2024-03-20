#include "server.hpp"

server::server()
{
    sockfd = -1;
    password = -1;
}

server::server(int port, int password)
{
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    this->password = password;
    setServerAddr(port);
}

void server::setServerAddr(int port)
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (fcntl(sockfd, F_SETFL, SOCK_NONBLOCK) == -1) {
        ft_error("Setting socket to non-blocking failed");
        exit(1);
    }
}

server::~server()
{
    close(sockfd);
}

void server::setSockfd(const int sockfd)
{
    this->sockfd = sockfd;
}

void server::setPassword(const int password)
{
    this->password = password;
}

int server::getSockfd() const
{
    return (sockfd);
}

int server::getPassword() const
{
    return (password);
}

struct sockaddr_in server::getServer_addr() const
{
    return (server_addr);
}

void server::bindServer()
{
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        ft_error("Binding failed");
        exit(1);
    }
}

void server::listenServer()
{
    if (listen(sockfd, 5) == -1) {
        ft_error("Listening failed");
        exit(1);
    }
}

void server::acceptServer()
{
    char *msg = strdup("Hello from server");
    if (msg == NULL) {
        ft_error("Memory allocation failed");
        exit(1);
    }
    int clientfd = accept(sockfd, NULL, NULL);
    if (clientfd == -1) {
        ft_error("Accepting failed");
        free(msg);
        exit(1);
    }
    if (send(clientfd, msg, strlen(msg), 0) == -1) {
        ft_error("Error in sending message");
        free(msg);
        close(clientfd);
        exit(1);
    }
    free(msg);
    close(clientfd);
}

void server::sendServer()
{
    char *msg = strdup("Hello from server");
    if (msg == NULL) {
        ft_error("Memory allocation failed");
        exit(1);
    }
    if (send(sockfd, msg, strlen(msg), 0) == -1) {
        ft_error("Error in sending message");
        free(msg);
        exit(1);
    }
    free(msg);
}

void server::receiveServer()
{
    char buffer[1024];
    if (recv(sockfd, buffer, sizeof(buffer), 0) == -1) {
        ft_error("Error in receiving message");
        exit(1);
    }
    std::cout << "Message from server: " << buffer << std::endl;
}

void server::closeServer()
{
    close(sockfd);
}

