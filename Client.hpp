#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>

class Client
{
    private:
        int _fd;
        std::string _realName;
        std::string _nickName;
        std::string _channel;

    public:
        Client();
        Client(int fd);
        // ~Client();
        void setFd(int fd);
        void setRealName(std::string realName);
        void setNickName(std::string nickName);
        void setChannel(std::string channel);
        int getFd() { return _fd; }
        std::string getRealName() { return _realName; }
        std::string getNickName() { return _nickName; }
        std::string getChannel() { return _channel; }

};


#endif