#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "../include/Exception.hpp"
#include "../include/Protocol.hpp"

class Client
{
    private:
        //copy ile assignment olabilir kalsın şimdilik

        int _clientfd;
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _password;
        int _type;
        int _port;
        char *_ip;

        bool _isRegistered;
        bool _isOperator;
        bool _isPasswordProtected;

    public:
        Client();
        ~Client();
        Client(int fd);

        //Setter
        inline void setClientfd(int clientfd) { _clientfd = clientfd;}
        inline void setNickname(std::string nickname) { _nickname = nickname; }
        inline void setUsername(std::string username) { _username = username; }
        inline void setRealname(std::string realname) { _realname = realname;}
        inline void setPassword(std::string password) { _password = password;}
        inline void setType(int type) { _type = type;}
        inline void setPort(int port) { _port = port;}
        inline void setIp(char *ip) { _ip = ip;}

        inline void setIsRegistered(bool isRegistered) { _isRegistered = isRegistered;}
        inline void setIsOperator(bool isOperator) { _isOperator = isOperator;}
        inline void setIsPasswordProtected(bool isPasswordProtected) { _isPasswordProtected = isPasswordProtected;}

        //Getter
        inline int getClientfd() const { return _clientfd;}
        std::string getNickname() const { return _nickname;}
        std::string getUsername() const { return _username;}
        std::string getRealname() const { return _realname;}
        std::string getPassword() const { return _password;}
        int getType() const { return _type;}
        int getPort() const { return _port;}
        char *getIp() const { return _ip;}

        bool getIsRegistered() const { return _isRegistered;}
        bool getIsOperator() const { return _isOperator;}
        bool getIsPasswordProtected() const { return _isPasswordProtected;}
};

#endif