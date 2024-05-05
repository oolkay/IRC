#include "../include/Channel.hpp"

Channel::Channel()
{
    _name = "";
    _topic = "";
    _key = "";
}

Channel::~Channel()
{

}

bool Channel::isClientInChannel(Client& client)
{
    std::vector<Client>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if(*it == client)
            return true;
    }
    return false;
}

bool Channel::isClientOperator(Client& client)
{
    std::vector<Client>::iterator it;

    for (it = _operators.begin(); it != _operators.end(); it++)
    {
        if(*it == client)
            return true;
    }
    return false;
}

bool Channel::isClientInInvList(Client& client)
{
    std::vector<Client>::iterator it;

    for (it = _invList.begin(); it != _invList.end(); it++)
    {
        if(*it == client)
            return true;
    }
    return false;
}

bool Channel::isClientInBanList(Client& client)
{
    std::vector<Client>::iterator it;

    for (it = _banList.begin(); it != _banList.end(); it++)
    {
        if(*it == client)
            return true;
    }
    return false;
}



void Channel::removeClient(Client& client)
{
    for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (*it == client)
        {
            _clients.erase(it);
            break;
        }
    }
}

void Channel::removeOperator(Client& client)
{
    if (isClientOperator(client))
    {
        std::vector<Client>::iterator it;

        for (it = _operators.begin(); it != _operators.end(); it++)
        {
            if (*it == client)
            {
                _operators.erase(it);
                break;
            }
        }
    }
}

void Channel::removeInvList(Client& client)
{
    if (isClientInInvList(client))
    {
        std::vector<Client>::iterator it;

        for (it = _invList.begin(); it != _invList.end(); it++)
        {
            if (*it == client)
            {
                _invList.erase(it);
                break;
            }
        }
    }
}
