# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <cstring>
# include <cstdlib>
# include <iostream>
# include <vector>
# include <map>
# include <poll.h>
# include <unistd.h>
# include <fcntl.h>
# include <sstream>
# include <string>
# include <cerrno>
# include <ctime>

#define QUEUE_SIZE 3

class Server
{
    private:
        int serverfd;
        struct sockaddr_in server_addr;
    public:
        Server();
        ~Server();
        int ft_createIPv4Socket();
        void ft_setIPv4Adress(int port);
        void ft_setSocketOptions();
        void ft_bindSocket();
        void ft_listenPort();
        int ft_acceptConnection();
        int ft_sendData(int clientfd, char *data);
        void ft_runserver();
};

//BOK YAPICI
Server::Server()
{
    serverfd = -1;
    memset(&server_addr, 0, sizeof(server_addr));
}

//BOK YIKICI
Server::~Server()
{
    close(serverfd);

}

int Server::ft_createIPv4Socket()
{
    this->serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1)
    {
        std::cerr << "Error: socket failed" << std::endl;
        return (-1);
    }
    return (serverfd);
}

void Server::ft_setIPv4Adress(int port)
{
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
}

void Server::ft_setSocketOptions()
{
    int q = 1;
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &q, sizeof(int)) == -1)
    {
        std::cerr << "Error: setsockopt failed" << strerror(errno) << std::endl;
        exit(1);
    }
    if (fcntl(serverfd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Error: fcntl failed" << std::endl;
        exit(1);
    }
}

void Server::ft_bindSocket()
{
    int bindResult = ::bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bindResult == -1)
    {
        close(serverfd);
        std::cerr << "Error: bind faailed" << strerror(errno) <<  std::endl;
        exit(1);
    }
}

void Server::ft_listenPort()
{
    int listenResult = listen(serverfd, QUEUE_SIZE);
    if (listenResult == -1)
    {
        std::cerr << "Error: listen failed" << std::endl;
        exit(1);
    }
}

int Server::ft_acceptConnection()
{
    struct sockaddr_in *client_addr;
    socklen_t client_addr_size = sizeof(*client_addr);
    int clientfd;
    while (true)
    {
        clientfd = accept(serverfd, (struct sockaddr *)client_addr, &client_addr_size);
        if (clientfd == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                continue;
            }
            else
            {
                close(serverfd);
                std::cerr << "Error: accept failed" << strerror(errno) <<std::endl;
                exit(1);
            }
        }
        break;
    }
    return (clientfd);
}

int Server::ft_sendData(int clientfd, char *data)
{
    int sendResult = send(clientfd, data, strlen(data), 0);
    if (sendResult == -1)
    {
        std::cerr << "Error: send failed" << std::endl;
        return (-1);
    }
    return (sendResult);
}

void Server::ft_runserver()
{
    int clientfd;
    while (true)
    {
        std::cout << "Waiting for connection" << std::endl;
        clientfd = ft_acceptConnection();
        char *data = strdup("Hello, world!");
        ft_sendData(clientfd, data);
    }
}

int main()
{
    Server server;


    server.ft_createIPv4Socket();
    server.ft_setIPv4Adress(50000);
    server.ft_setSocketOptions();
    server.ft_bindSocket();
    server.ft_listenPort();
    server.ft_runserver();


    // fd_set readfds;
    // FD_ZERO(&readfds);
    // FD_SET(serverfd, &readfds);
    // int maxfd = serverfd;
    // select(serverfd + 1, &readfds, NULL, NULL, NULL);
    // if (FD_ISSET(serverfd, &readfds))
    // {
    //     std::cout << "New connection" << std::endl;
    // }
    
    return (0);
}
