#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <iomanip>
#include <sstream>
#include "../include/Server.hpp"
#include "../include/Client.hpp"

class Server;
class Client;

namespace Utils
{
    std::vector<std::string> split(const std::string &s, char delim);
    std::string trim(const std::string &s, const std::string toTrim);
    std::string join(std::vector<std::string> v, std::string delim, int start = 0 , int end = -1);
    void ut_write(int fd, const std::string &s);
    void ut_send(int fd, const std::string &s);
    void ut_sendall(int fd, const std::string &s, Server &server);
    std::string getTime();
    std::string toUpper(const std::string &s);
}

#endif // UTILS_HPP