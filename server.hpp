#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.hpp"
#include <cstring>
#include <map>
#include "client.hpp"

class server {
    private:
        int sockfd;
        int password;
        struct sockaddr_in server_addr;
        std::map<int, client> clients;
        server& operator=(const server& other);
        server(const server& other);
    public:
        server();
        server(int port, int password);
        void setServerAddr(int port);
        void setSockfd(const int sockfd);
        void setPassword(const int password);
        int getSockfd() const;
        int getPassword() const;
        struct sockaddr_in getServer_addr() const;
        void bindServer();
        void listenServer();
        void acceptServer();
        void sendServer();
        void receiveServer();
        void closeServer();
        ~server();
};


void ft_error(std::string str);

#endif