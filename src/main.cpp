#include "../include/Server.hpp"

int main(int argc, char const *argv[])
{
    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <password> <port>" << std::endl;
        return 1;
    }
    try
    {
        Server server(argv[1], atoi(argv[2]));

        server.run();
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}