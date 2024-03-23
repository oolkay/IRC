#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

class Parser
{
    private:
        std::string line;
        std::string cmd;
        std::string arg;
        std::vector<std::string> args;
    public:
        Parser();
        ~Parser();
        void ft_parseLine(std::string line);
        void ft_parseCmd(std::string cmd);
        void ft_parseArg(std::string arg);
        void ft_parseArgs(std::string args);
        void ft_printLine();
        void ft_printParser();
        std::string getCmd() { return cmd; };
        std::string getArg() { return arg; };
        std::vector<std::string> getArgs() { return args; };      
};

#endif
