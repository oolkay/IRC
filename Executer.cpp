#include "Executer.hpp"
#include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <cstdlib>

Executer::Executer()
{
    commands["msg"] = "msg";
    commands["nick"] = "nick";

    // commands["NICK"] = "NICK";
    // commands["USER"] = "USER";
    // commands["JOIN"] = "JOIN";
    // commands["PART"] = "PART";
    // commands["PRIVMSG"] = "PRIVMSG";
    // commands["NOTICE"] = "NOTICE";
    // commands["PING"] = "PING";
    // commands["PONG"] = "PONG";
    // commands["QUIT"] = "QUIT";
}

Executer::~Executer()
{
    commands.clear();
}

void Executer::ft_msg(Server& sv, Parser cmd, int clientfd)
{
    std::vector<std::string> args = cmd.getArgs();
    Client client = sv.getClients()[clientfd];
    if (args.size() < 2)
    {
        std::cerr << "Error: not enough arguments" << std::endl;
        return;
    }
    if (args.size() == 3)
    {
        std::string msg = args[2];
        std::string toUser = args[1];
        Client toClient = sv.getClientByNick(toUser);
        if (toClient.getNickName() == "")
        {
            std::cerr << "Error: user not found" << std::endl;
            return;
        }
        std::string response = "Message by " + client.getNickName() + ": " + msg + "\n";
        sv.ft_sendData(toClient.getFd(), (char *)response.c_str());

    }
    else
    {
        std::cout << "Message by " << client.getNickName() << ": "<< args[1] <<  std::endl;
    }

}

void Executer::ft_executeCommand(Parser cmd, Server& sv, int clientfd)
{
    if (cmd.getCmd() == "msg")
    {
        ft_msg(sv, cmd, clientfd);
    }
    else if (cmd.getCmd() == "nick")
    {
        std::vector<std::string> args = cmd.getArgs();
        if (args.size() < 2)
        {
            std::cerr << "Error: not enough arguments" << std::endl;
            return;
        }
        std::string nickname =args[1].substr(0, args[1].size()-1);
        sv.getClients()[clientfd].setNickName(nickname);
        sv.ft_sendData(clientfd, "Nickname updated-> " + sv.getClients()[clientfd].getNickName() + "\n");

    }
    else if (commands.find(cmd.getCmd()) == commands.end())
    {
        std::cerr << "Error: command not found" << std::endl;
        return;
    }

    else
    {
        std::cerr << "Error: command not implemented" << std::endl;
    }
}
