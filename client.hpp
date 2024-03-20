#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "irc.hpp"

class client
{
    private:
        int clientfd;
        struct sockaddr_in client_addr;
        std::string nickname;
        std::string username;
        std::string realname;
        std::string mod;
    public:
        client();
        client(int clientfd, std::string nickname, std::string username, std::string realname, std::string mod);
        void setClientfd(const int clientfd);
        void setClientAddr(const int port);
        void setClient_addr(const struct sockaddr_in client_addr);
        void setNickname(const std::string nickname);
        void setUsername(const std::string username);
        void setRealname(const std::string realname);
        void setMod(const std::string mod);
        int getClientfd() const;
        struct sockaddr_in getClient_addr() const;
        std::string getNickname() const;
        std::string getUsername() const;
        std::string getRealname() const;
        std::string getMod() const;
        ~client();
};



#endif