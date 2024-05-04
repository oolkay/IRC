#include "../include/Client.hpp"
#include <iostream>
#include <unistd.h>

Client::Client()
{
    _clientfd = -1;
    _nickname = "";
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
    if(_clientfd != -1)
        close(_clientfd);
}