#include "../include/Server.hpp"
#include "../include/Exception.hpp"
#include "../include/Channel.hpp"
#include <string.h>
#include <cerrno>

Server::Server(std::string passwd, int port): 
    _svPassword(passwd), _port(port)
{
    createIpv4Socket();
    setSocketOptions();
    bindSocket();
    listenSocket();
    initCommands();
}

Server::~Server()
{
    closeSocket();
}

void Server::createIpv4Socket()
{
    _serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(_serverfd == -1)
        throw Exception("Socket creation failed");
}

void Server::setSocketOptions()
{
    int opt = 1;
    if(setsockopt(_serverfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        throw Exception("Socket options setting failed");
}

void Server::bindSocket()
{
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_addr.s_addr = INADDR_ANY;
    _server_addr.sin_port = htons(_port);

    if (fcntl(_serverfd, F_SETFL, O_NONBLOCK) == -1)
        throw Exception("Socket binding failed");
    if (bind(_serverfd, (struct sockaddr*)&_server_addr, sizeof(_server_addr)) == -1)
        throw Exception("Socket binding failed");
}

void Server::listenSocket()
{
    if (listen(_serverfd, QUEUE_SIZE) == -1)
        throw Exception("Socket listening failed");
}

void Server::closeSocket()
{
    if(_serverfd != -1)
        close(_serverfd);
}

int Server::acceptClient()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    memset(&client_addr, 0, sizeof(client_addr));
    int clientfd = accept(_serverfd, (struct sockaddr*)&client_addr, &client_addr_size);
    if(clientfd == -1)
        throw Exception("Client accept failed");
    if (fcntl(clientfd, F_SETFL, O_NONBLOCK) == -1)
        throw Exception("Client accept failed");
    int port = ntohs(client_addr.sin_port);
    char *ip = inet_ntoa(client_addr.sin_addr);

    Client client;

    client.setClientfd(clientfd);
    client.setPort(port);
    client.setIp(ip);
    _clients.push_back(client);
    return clientfd;
}


void Server::run()
{
    fd_set tmpreadfds;
    fd_set tmpwritefds;
    int clientfd;
    bool isReadySelect = true;

    FD_ZERO(&_readfds);
    FD_ZERO(&_writefds);
    FD_ZERO(&tmpreadfds);
    FD_ZERO(&tmpwritefds);
    FD_SET(_serverfd, &_readfds);

    // int maxfd = _serverfd;

    while(true)
    {
        if (isReadySelect)
        {
            tmpreadfds = _readfds; // bu satır cok önemli select fonksiyonu bakacagı seti bok ettiği için her seferinde tmpfds yi set ediyoruz
            tmpwritefds = _writefds;
            if (select(FD_SETSIZE, &tmpreadfds, NULL, NULL, NULL) == -1)
            {
                std::cerr << strerror(errno) << std::endl;   
                throw Exception("Error: select failed");
            }
            isReadySelect = false;
        }

        // IS SERVER SET
        if (FD_ISSET(_serverfd, &tmpreadfds) && !isReadySelect)
        {
            std::cout << "Server is set" << std::endl;
            clientfd = acceptClient();

            FD_SET(clientfd, &_readfds);
            std::cout << "Client accepted" << clientfd <<std::endl;
            isReadySelect = true;
        }
        else if (!isReadySelect) // IS CLIENT SET
        {
            std::cout << "Client is set" << std::endl;
            monitorizeReads(tmpreadfds);
            isReadySelect = true;
        }
    }
}


void Server::monitorizeClients(fd_set &tmpread, fd_set &tmpwrite)
{
    monitorizeReads(tmpread);
    (void) tmpwrite;
    // monitorizeWrites(tmpwrite);
}

void Server::monitorizeReads(fd_set &tmpread)
{
    std::cout << "Monitorize reads" << std::endl;
    std::vector<Client>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if (FD_ISSET(it->getClientfd(), &tmpread))
        {
            char buffer[1024];
            int read_size = read(it->getClientfd(), buffer, 1024);
            if (read_size <= 0)
            {
                std::cout << "Client disconnected" << std::endl;
                FD_CLR(it->getClientfd(), &_readfds);
                FD_CLR(it->getClientfd(), &_writefds);
                close(it->getClientfd());
                int chIndx = isInChannel(*it);
                if (chIndx != -1)
                    _channels[chIndx].removeClient(*it);
                _clients.erase(it);
                // TextEngine::blue("Client ", TextEngine::printTime(cout)) << a->_ip << ":" << a->getPort() << " disconnected" << std::endl;
            }
            else
            {
                std::cout << "Read size: " << read_size << std::endl;
                buffer[read_size-1] = '\0';
                processCommand(buffer, *it);
            }
        }
    }
    
}

void Server::processCommand(std::string buffer, Client &client)
{
    std::vector<std::string> tokens = Utils::split(buffer, ' ');
    std::string cmd = tokens[0];
    cmd = Utils::toUpper(cmd);
    std::cout << "Buffer: " << buffer << std::endl;
    std::cout << "Command: " << cmd << std::endl;

    if (_commands.find(cmd) != _commands.end())
        (this->*_commands[cmd])(tokens, client);
}


int Server::isInChannel(Client &client)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].isClientInChannel(client))
            return i;
    }
    return -1;
}

void Server::initCommands()
{
    _commands["PASS"] = &Server::pass;
    _commands["NICK"] = &Server::nick;
    _commands["USER"] = &Server::user;
    // _commands["JOIN"] = &Server::join;
    // _commands["PRIVMSG"] = &Server::privmsg;
    // _commands["TOPIC"] = &Server::topic;
    // _commands["QUIT"] = &Server::quit;
    // _commands["PART"] = &Server::part;
    // _commands["MODE"] = &Server::mode;
    // _commands["LIST"] = &Server::list;
    // _commands["KICK"] = &Server::kick;
    // _commands["INVITE"] = &Server::invite;
    // _commands["WHO"] = &Server::who;
    // _commands["OP"] = &Server::op;
    // _commands["PING"] = &Server::ping;
    // _commands["NOTICE"] = &Server::notice;
    // _commands["WHOIS"] = &Server::whois;
}
void Server::texter(std::string message, std::ostream &os, std::string color)
{
    os << WHITE << "[ " <<Utils::getTime() << " ] "<< color << message << RESET << std::endl;
}

bool Server::isNicknameInUse(std::string nickname)
{
    std::vector<Client>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->getNickname() == nickname)
            return true;
    }
    return false;
}

void Server::pass(std::vector<std::string> buffer, Client &client)
{
    std::cout << "Pass command" << std::endl;
    if (client.getIsPasswordProtected())
    {
        std::string message = ERR_ALREADYREGISTRED(client.getUserByHexChat());
        send(client.getClientfd(), message.c_str(), message.size(), 0);
    }
    else
    {
        if (buffer.size() != 2)
        {
            if (buffer.size() < 2){
                std::string message = ERR_NEEDMOREPARAMS(client.getUserByHexChat(),"PASS");
                send(client.getClientfd(), message.c_str(), message.size(), 0);
            }
            else
            {
                std::string message = "1 parameter is allowed for PASS command\n";
                send(client.getClientfd(), message.c_str(), message.size(), 0);
            }
        }
        else if(buffer.size() == 2)
        {
            if (buffer[1] == _svPassword)
            {
                client.setIsPasswordProtected(true);
                std::string message = "Password accepted\n";
                send(client.getClientfd(), message.c_str(), message.size(), 0);
            }
            else
            {
                std::string message = ERR_PASSWDMISMATCH(client.getUserByHexChat());
                send(client.getClientfd(), message.c_str(), message.size(), 0);
            }
        }
    }
}
void Server::nick(std::vector<std::string> buffer, Client &client)
{
	if (!client.getIsPasswordProtected())
	{
		Utils::ut_write(client.getClientfd(), ERR_NOTPASSWORDED(client.getUserByHexChat()));
		return;
	}
	std::string nickname;
	if (buffer.size() < 2)
	{
		Utils::ut_write(client.getClientfd(), ERR_NONICKNAMEGIVEN(client.getUserByHexChat()));
		return;
	}
	else if (buffer[1].size() > 9 || buffer[1].size() < 1)
	{
		Utils::ut_write(client.getClientfd(), ERR_ERRONEUSNICKNAME(client.getUserByHexChat(), buffer[0]));
		return;
	}
	nickname = buffer[1];
	if (nickname[0] == '#')
	{
		Utils::ut_write(client.getClientfd(), ERR_ERRONEUSNICKNAME(client.getUserByHexChat(), nickname));
		return;
	}
	
    if (client.getNickname() != nickname && isNicknameInUse(nickname))
    {
        Utils::ut_write(client.getClientfd(), ERR_NICKNAMEINUSE(client.getUserByHexChat(), nickname));
        return;
    }

	std::string oldnickname = client.getNickname();
	if (!client.getNickname().empty())
        texter("user " + client.getNickname() + " has changed nickname to " + nickname, std::cout, MAGENTA);
	client.setNickname(nickname);
    Utils::ut_write(client.getClientfd(), RPL_NICK(oldnickname, client.getUsername(), client.getIp(), nickname));
	FD_SET(client.getClientfd(), &_writefds);

    std::vector<Channel>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
        std::vector<Client>::iterator cit;

		for (cit = it->getClients().begin(); cit != it->getClients().end(); ++cit)
		{
			if (oldnickname == cit->getNickname())
			{
				cit->setNickname(nickname);
				it->sendToAll(RPL_NICK(oldnickname, client.getUsername(), client.getIp(), nickname));
				// responseAllClientResponseToGui(*cit, *it);
				break;
			}
		}
	}
	if (client.getIsRegistered() == false && !client.getUsername().empty() && !client.getRealname().empty())
	{
		client.setIsRegistered(true);
		Utils::ut_write(client.getClientfd(), RPL_WELCOME(client.getNickname(), client.getUserByHexChat()));
        Server::texter("user " + client.getNickname() + " has been registered", std::cout, MAGENTA);
	}


}

void Server::user(std::vector<std::string> buffer, Client &client)
{
    std::cout << "User command" << std::endl;
    if (!client.getIsPasswordProtected())
    {
        std::string message = ERR_NOTPASSWORDED(client.getUserByHexChat());
        send(client.getClientfd(), message.c_str(), message.size(), 0);
    }
    else if(client.getNickname().empty())
    {
        std::string message = ERR_NONICKNAMEGIVEN(client.getUserByHexChat());
        send(client.getClientfd(), message.c_str(), message.size(), 0);
    }
    else if (client.getIsRegistered())
    {
        std::string message = ERR_ALREADYREGISTRED(client.getUserByHexChat());
        send(client.getClientfd(), message.c_str(), message.size(), 0);
    }
    else
    {
        if (buffer.size() != 5)
        {
            if (buffer.size() < 5)
            {
                std::string message = ERR_NEEDMOREPARAMS(client.getUserByHexChat(),"USER");
                send(client.getClientfd(), message.c_str(), message.size(), 0);
            }
            else
            {
                std::string message = "4 parameters are allowed for USER command\n";
                send(client.getClientfd(), message.c_str(), message.size(), 0);
            }
        }
        else if(buffer.size() == 5)
        {
            client.setUsername(buffer[1]);
            client.setHostname(buffer[2]);
            client.setServername(buffer[3]);
            client.setRealname(buffer[4]);
            client.setIsRegistered(true);
            std::string message = "Successfully registered user\n";
            send(client.getClientfd(), message.c_str(), message.size(), 0);
        }
    }
}