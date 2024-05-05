#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include "Client.hpp"
#include "Utils.hpp"
#include "Channel.hpp"
#include <fcntl.h>
#include <unistd.h>
#include "Protocol.hpp"


#define QUEUE_SIZE 10 


class Client;
class Channel;

class Server
{
    private:
        int _serverfd;
        struct sockaddr_in _server_addr;
        std::vector<Client> _clients;
        std::map<std::string, void (Server::*)(std::string , Client&)> _commands;
        std::vector<Channel> _channels;

        fd_set _readfds;
        fd_set _writefds;

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

        int acceptClient();
        void removeClient(Client& client);        
        void addClient(Client& client);

        //Getter
        int getServerfd() const;
        struct sockaddr_in getServerAddr() const;
        std::vector<Client>& getClients() { return _clients; };
        std::vector<Channel>& getChannels() { return _channels; }; ;


        void monitorizeClients(fd_set& tmpread, fd_set& tmpwrite);
        void monitorizeReads(fd_set& tmpread);
        void monitorizeWrites(fd_set& tmpwrite);

        void processCommand(std::string buffer, Client& client);

        void initCommands();

        int isInChannel(Client& client);

        void pass(std::string buffer, Client& client);
        void nick(std::string buffer, Client& client);
        void user(std::string buffer, Client& client);
        void join(std::string buffer, Client& client);
        void privmsg(std::string buffer, Client& client);
        void topic(std::string buffer, Client& client);
        void quit(std::string buffer, Client& client);
        void part(std::string buffer, Client& client);
        void mode(std::string buffer, Client& client);
        void list(std::string buffer, Client& client);
        void kick(std::string buffer, Client& client);
        void invite(std::string buffer, Client& client);
        void who(std::string buffer, Client& client);
        void op(std::string buffer, Client& client);
        void ping(std::string buffer, Client& client);
        void notice(std::string buffer, Client& client);
        void whois(std::string buffer, Client& client);
        

        


};

#endif