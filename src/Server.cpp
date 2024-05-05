#include "../include/Server.hpp"
#include "../include/Exception.hpp"
#include "../include/Channel.hpp"
#include <string.h>
#include <cerrno>

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
    if(setsockopt(_serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw Exception("Socket options setting failed");
}

void Server::bindSocket()
{
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_addr.s_addr = INADDR_ANY;
    _server_addr.sin_port = htons(20000);

    if (fcntl(_serverfd, F_SETFL, O_NONBLOCK) == -1)
        throw Exception("Socket binding failed");
    if (bind(_serverfd, (struct sockaddr*)&_server_addr, sizeof(_server_addr)) == -1)
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

int Server::acceptClient()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));
    int clientfd = accept(_serverfd, (struct sockaddr*)&client_addr, &client_addr_size);
    if(clientfd == -1)
        throw Exception("Client accept failed");
    if (fcntl(clientfd, F_SETFL, O_NONBLOCK) == -1)
        throw Exception("Client accept failed");
    int port = ntohs(client_addr.sin_port);
    char *ip = inet_ntoa(client_addr.sin_addr);

    Client client;

    client.setClientfd(clientfd);
    client.setPort(port);
    client.setIp(ip);
    _clients.push_back(client);
    return clientfd;
}


void Server::run()
{
    fd_set tmpreadfds;
    fd_set tmpwritefds;
    int clientfd;
    bool isReadySelect = true;

    FD_ZERO(&_readfds);
    FD_ZERO(&_writefds);
    FD_ZERO(&tmpreadfds);
    FD_ZERO(&tmpwritefds);
    FD_SET(_serverfd, &_readfds);

    // int maxfd = _serverfd;

    while(true)
    {
        while (isReadySelect)
        {
            tmpreadfds = _readfds; // bu satır cok önemli select fonksiyonu bakacagı seti bok ettiği için her seferinde tmpfds yi set ediyoruz
            tmpwritefds = _writefds;
            if (select(FD_SETSIZE, &tmpreadfds, NULL, NULL, NULL) == -1)
            {
                std::cerr << strerror(errno) << std::endl;   
                throw Exception("Error: select failed");
            }
            isReadySelect = false;
        }

        // IS SERVER SET
        if (FD_ISSET(_serverfd, &tmpreadfds) && !isReadySelect)
        {
            std::cout << "Server is set" << std::endl;
            clientfd = acceptClient();

            FD_SET(clientfd, &_readfds);
            std::cout << "Client accepted" << clientfd <<std::endl;
            isReadySelect = true;
        }
        else if (!isReadySelect) // IS CLIENT SET
        {
            std::cout << "Client is set" << std::endl;
            monitorizeReads(tmpreadfds);
        }
    }
}



void Server::monitorizeClients(fd_set &tmpread, fd_set &tmpwrite)
{
    monitorizeReads(tmpread);
    (void) tmpwrite;
    // monitorizeWrites(tmpwrite);
}

void Server::monitorizeReads(fd_set &tmpread)
{
    std::cout << "Monitorize reads" << std::endl;
    std::vector<Client>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if (FD_ISSET(it->getClientfd(), &tmpread))
        {
            char buffer[1024];
            int read_size = read(it->getClientfd(), buffer, 1024);
            if (read_size <= 0)
            {
                std::cout << "Client disconnected" << std::endl;
                FD_CLR(it->getClientfd(), &_readfds);
                FD_CLR(it->getClientfd(), &_writefds);
                close(it->getClientfd());
                int chIndx = isInChannel(*it);
                if (chIndx != -1)
                    _channels[chIndx].removeClient(*it);
                _clients.erase(it);
                // TextEngine::blue("Client ", TextEngine::printTime(cout)) << a->_ip << ":" << a->getPort() << " disconnected" << std::endl;

            }
            else
            {
                std::cout << "Read size: " << read_size << std::endl;
                buffer[read_size] = '\0';
                processCommand(buffer, *it);

            }
        }
    }
    
}

void Server::processCommand(std::string buffer, Client &client)
{
    std::vector<std::string> tokens = Utils::split(buffer, ' ');
    std::string cmd = tokens[0];
    cmd = Utils::toUpper(cmd);
    std::cout << "Buffer: " << buffer << std::endl;
    std::cout << "Command: " << cmd << std::endl;

    if (_commands.find(cmd) != _commands.end())
        (this->*_commands[cmd])(Utils::join(tokens, " ", 1), client);
}


int Server::isInChannel(Client &client)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].isClientInChannel(client))
            return i;
    }
    return -1;
}

void Server::initCommands()
{
    // _commands["PASS"] = &Server::joinChannel;
    // _commands["NICK"] = &Server::leaveChannel;
    // _commands["USER"] = &Server::listChannels;
    // _commands["PRIVMSG"] = &Server::sendMessage;
}