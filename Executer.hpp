#ifndef EXECUTER_HPP
#define EXECUTER_HPP

#include <iostream>
#include <map>
#include "Parser.hpp"
#include "Server.hpp"


class Server;

class Executer
{
    private:
        std::map<std::string, std::string> commands;
    public:
        Executer();
        ~Executer();
        void ft_executeCommand(Parser cmd, Server& sv, int clientfd);
        void ft_msg(Server& sv, Parser cmd, int clientfd);
};

#endif