#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include "Client.hpp"
// #include "Channel.hpp"
#include <fcntl.h>
#include <unistd.h>
#include "Protocol.hpp"


#define QUEUE_SIZE 10 


class Client;
// class Channel;

class Server
{
    private:
        int _serverfd;
        struct sockaddr_in _server_addr;
        std::vector<Client> _clients;
        // std::vector<Channel> _channels;

        Server& operator=(const Server& other);
        Server(const Server& other);

    public:
        Server();
        ~Server();
        void run();
        void createIpv4Socket();
        void bindSocket();
        void listenSocket();
        void closeSocket();
        void setSocketOptions();

        void acceptClient();
        void removeClient(Client& client);        
        void addClient(Client& client);

        //Getter
        int getServerfd() const;
        struct sockaddr_in getServerAddr() const;
        std::vector<Client&> getClients() const;
        // std::vector<Channel&> getChannels() const;



        


};

#endif