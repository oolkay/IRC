#include "../include/Server.hpp"

int main()
{
    try
    {
        Server server;

        server.run();
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}