#include "Executer.hpp"
#include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <cstdlib>

Executer::Executer()
{
    commands["msg"] = "msg";
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

void Executer::ft_executeCommand(Parser cmd)
{
    if (cmd.getCmd() == "msg")
    {
        std::cout << "Message: " << cmd.getArg() << std::endl;
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
