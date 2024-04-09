#include "Parser.hpp"
#include <iostream>

Parser::Parser()
{
    line = "";
    cmd = "";
    arg = "";
    args.clear();
}

Parser::~Parser()
{
    line = "";
    cmd = "";
    arg = "";
    args.clear();
}

void Parser::ft_parseLine(std::string line)
{
    this->line = line;
    std::string cmd;
    std::string arg;
    std::string args;
    if (line[0] == '/')
    {
        cmd = line.substr(1,  line.find(' ') - 1);
        if (cmd[cmd.size() - 1] == '\n')
            cmd = cmd.substr(0, cmd.size() - 1);
        arg = line.substr(cmd.size() + 1, line.size() - 1);
        args = line.substr(1);
        ft_parseCmd(cmd);
        ft_parseArg(arg);
        ft_parseArgs(args);
        // ft_printParser();
    }
    else{}
        //designi düşünmedim
}

void Parser::ft_printParser()
{
    std::cout << "line: " << line << std::endl;
    std::cout << "cmd: " << cmd << std::endl;
    std::cout << "arg: " << arg << std::endl;
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
    {
        std::cout << "args: " << *it << std::endl;
    }
}

void Parser::ft_parseCmd(std::string cmd)
{
    this->cmd = cmd;
}

void Parser::ft_parseArg(std::string arg)
{
    this->arg = arg;
}

void Parser::ft_parseArgs(std::string args)
{
    std::string::size_type pos = 0;
    std::string::size_type end = 0;
    while (pos != std::string::npos)
    {
        end = args.find(' ', pos);
        if (end == std::string::npos)
        {
            args = args.substr(pos);
            this->args.push_back(args);
            break;
        }
        this->args.push_back(args.substr(pos, end - pos));
        pos = end + 1;
    }
}

void Parser::ft_printLine()
{
    std::cout << "cmd: " << cmd << std::endl;
    std::cout << "arg: " << arg << std::endl;
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
    {
        std::cout << "args: " << *it << std::endl;
    }
}