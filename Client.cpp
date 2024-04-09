#include "Client.hpp"


Client::Client()
{
    _fd = -1;
}

Client::Client(int fd)
{
    _fd = fd;
}

// Client::Client(int fd, std::string realName, std::string nickName)
// {
//     _fd = fd;
//     _realName = realName;
//     _nickName = nickName;
// }


void Client::setFd(int fd)
{
    _fd = fd;
}

void Client::setRealName(std::string realName)
{
    _realName = realName;
}

void Client::setNickName(std::string nickName)
{
    _nickName = nickName;
}

void Client::setChannel(std::string channel)
{
    _channel = channel;
}

