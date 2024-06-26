#include "../include/Client.hpp"
#include <iostream>
#include <unistd.h>

Client::Client()
{
    _clientfd = -1;
    _nickname = "";
    _hostname = "";
    _servername = "";
    _username = "";
    _realname = "";
    _password = -1;
    _type = -1;
    _port = -1;

    _isRegistered = false;
    _isOperator = false;
    _isPasswordProtected = false;

    _ip = NULL;
}

Client::~Client()
{

}

bool Client::operator==(const Client& other) const
{
    return _clientfd == other._clientfd;
}

std::string Client::getUserByHexChat() const {
	std::string strIP = this->_ip;
	return this->_nickname + (this->_username.empty() ? "" : "!~" + this->_username) + (strIP.empty() ? "" : "@" + strIP);
}