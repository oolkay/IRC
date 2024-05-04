#include "../include/Server.hpp"

int main()
{
    try
    {
        Server server;

        server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}