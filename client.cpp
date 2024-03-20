#include "client.hpp"

client::client()
{
    clientfd = -1;
    nickname = "";
    username = "";
    realname = "";
    mod = "";
}

client::client(int clientfd, std::string nickname, std::string username, std::string realname, std::string mod)
{
    this->clientfd = clientfd;
    this->nickname = nickname;
    this->username = username;
    this->realname = realname;
    this->mod = mod;
}

void client::setClientfd(const int clientfd)
{
    this->clientfd = clientfd;
}

void client::setClientAddr(const int port)
{
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    client_addr.sin_addr.s_addr = INADDR_ANY;
}

void client::setClient_addr(const struct sockaddr_in client_addr)
{
    this->client_addr = client_addr;
}

void client::setNickname(const std::string nickname)
{
    this->nickname = nickname;
}

void client::setUsername(const std::string username)
{
    this->username = username;
}

void client::setRealname(const std::string realname)
{
    this->realname = realname;
}

void client::setMod(const std::string mod)
{
    this->mod = mod;
}

int client::getClientfd() const
{
    return (clientfd);
}

struct sockaddr_in client::getClient_addr() const
{
    return (client_addr);
}

std::string client::getNickname() const
{
    return (nickname);
}

std::string client::getUsername() const
{
    return (username);
}

std::string client::getRealname() const
{
    return (realname);
}

std::string client::getMod() const
{
    return (mod);
}

client::~client()
{
    close(clientfd);
}
