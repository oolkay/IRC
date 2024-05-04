#include "../include/Server.hpp"
#include "../include/Exception.hpp"
#include <string.h>

Server::Server()
{
    createIpv4Socket();
    setSocketOptions();
    bindSocket();
    listenSocket();
}

Server::~Server()
{
    closeSocket();
}

void Server::createIpv4Socket()
{
    _serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverfd == -1)
        throw Exception("Socket creation failed");
}

void Server::setSocketOptions()
{
    int opt = 1;
    if(setsockopt(_serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        throw Exception("Socket options setting failed");
}

void Server::bindSocket()
{
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_addr.s_addr = INADDR_ANY;
    _server_addr.sin_port = htons(6667);

    if (bind(_serverfd, (struct sockaddr*)&_server_addr, sizeof(_server_addr)) == -1)
        throw Exception("Socket binding failed");
    if (fcntl(_serverfd, F_SETFL, O_NONBLOCK) == -1)
        throw Exception("Socket binding failed");
}

void Server::listenSocket()
{
    if (listen(_serverfd, QUEUE_SIZE) == -1)
        throw Exception("Socket listening failed");
}

void Server::closeSocket()
{
    if(_serverfd != -1)
        close(_serverfd);
}

void Server::acceptClient()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int clientfd = accept(_serverfd, (struct sockaddr*)&client_addr, &client_addr_size);
    if(clientfd == -1)
        throw Exception("Client accept failed");

    Client client;

    client.setClientfd(clientfd);
    _clients.push_back(client);
}


void Server::run()
{

    while(true)
    {
        
    }
}