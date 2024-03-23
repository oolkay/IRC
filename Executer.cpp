#include "Executer.hpp"

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
    if (commands.find(cmd.getCmd()) == commands.end())
    {
        std::cerr << "Error: command not found" << std::endl;
        return;
    }
    if (cmd.getCmd() == "msg")
    {
        std::cout << "Message: " << cmd.getArg() << std::endl;
    }
    else
    {
        std::cerr << "Error: command not implemented" << std::endl;
    }
}
