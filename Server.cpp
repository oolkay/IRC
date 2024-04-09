#include "Server.hpp"

#define QUEUE_SIZE 3


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
    struct sockaddr_in *client_addr = new struct sockaddr_in();
    socklen_t client_addr_size = sizeof(*client_addr);
    memset(client_addr, 0, client_addr_size);
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
        clientfd = ft_acceptConnection();
        char buffer[1024];
        memset(buffer, 0, 1024);
        int pid = fork();
        if (pid == 0)
        {
            int recvResult = recv(clientfd, buffer, 1024, 0);
            while (recvResult > 0)
            {
                Parser parser;
                Executer exec;
                parser.ft_parseLine(buffer);
                if (parser.getCmd() == "exit")
                {
                    ft_sendData(clientfd, (char *)"Exiting...\n");
                    close(clientfd);
                    exit(0);
                }
                exec.ft_executeCommand(parser);
                std::string response = "PONG\n";
                ft_sendData(clientfd, (char *)response.c_str());
                memset(buffer, 0, 1024);
                recvResult = recv(clientfd, buffer, 1024, 0);
            }
            if (recvResult == -1)
            {
                std::cout << "recv failed" << std::endl;
                close(clientfd);
                exit(1);
            }
            exit(0);
        }
        close(clientfd);
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


    
    return (0);
}
