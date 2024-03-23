#ifndef EXECUTER_HPP
#define EXECUTER_HPP

#include <iostream>
#include <map>
#include "Parser.hpp"

class Executer
{
    private:
        std::map<std::string, std::string> commands;
    public:
        Executer();
        ~Executer();
        void ft_executeCommand(Parser cmd);
};

#endif