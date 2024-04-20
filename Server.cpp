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
        throw new std::runtime_error("Error: socket failed");
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
        throw new std::runtime_error("Error: setsockopt failed");
    if (fcntl(serverfd, F_SETFL, O_NONBLOCK) == -1)
        throw new std::runtime_error("Error: fcntl failed");
}

void Server::ft_bindSocket()
{
    int bindResult = ::bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bindResult == -1)
    {
        close(serverfd);
        throw new std::runtime_error("Error: bind failed");
    }
}

void Server::ft_listenPort()
{
    int listenResult = listen(serverfd, QUEUE_SIZE);
    if (listenResult == -1)
    {
        throw new std::runtime_error("Error: listen failed");
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
                continue;
            else
                throw new std::runtime_error("Error: accept failed");
        }
        Client c = ft_createClient(clientfd);
        clients.insert(std::pair<int, Client>(clientfd, c));
        break;
    }
    return (clientfd);
}

int Server::ft_sendData(int clientfd, std::string data)
{
    int sendResult = send(clientfd, data.c_str(), data.size(), 0);
    if (sendResult == -1)
        throw new std::runtime_error("Error: send failed");
    return (sendResult);
}

void Server::ft_addClient(int fd)
{
    Client client(fd);
    clients.insert(std::pair<int, Client>(fd, client));
}

void Server::ft_removeClient(int fd)
{
    clients.erase(fd);
}

void Server::ft_handleConnection(int clientfd)
{
    char buffer[1024];
    memset(buffer, 0, 1024);
    int recvResult = recv(clientfd, buffer, 1024, 0);
    Client client(clients[clientfd]);
    int pid = fork();
    if (pid == -1)
        throw new std::runtime_error("Error: fork failed");
    if (pid == 0) //somehow this forks clientlerin birbirlerini bloklamasını önlüyor bro idk how
    {
        while (recvResult > 0)
        {
            Parser parser;
            Executer exec;
            parser.ft_parseLine(buffer);
            exec.ft_executeCommand(parser, *this, clientfd);
            std::string response = "RESPONSE: PONG\n";
            ft_sendData(clientfd, (char *)response.c_str());
            memset(buffer, 0, 1024);
            recvResult = recv(clientfd, buffer, 1024, 0);
        }
        if (recvResult == -1)
            throw new std::runtime_error("Error: recv failed");
        exit(0);
    }

}

void Server::ft_runserver()
{
    int clientfd;
    fd_set readfds;
    fd_set currentfds;
    FD_ZERO(&currentfds);
    FD_SET(serverfd, &currentfds);
    // FD_SETSIZE OPTİMUM DEĞİL O YÜZDEN SONRA DÜŞÜN 

    while (true)
    {
        // try{
            readfds = currentfds; // bu satır cok önemli select fonksiyonu bakacagı seti bok ettiği için her seferinde currentfds yi set ediyoruz
            if (select(FD_SETSIZE, &readfds, NULL, NULL, NULL) == -1)
                throw new std::runtime_error("Error: select failed");
            for (int i = 0; i < FD_SETSIZE; i++)
            {
                if (FD_ISSET(i, &readfds))
                {
                    if (i == serverfd)
                    {
                        clientfd = ft_acceptConnection();
                        if (clientfd != -1)
                            FD_SET(clientfd, &currentfds);
                    }
                    else
                    {
                        ft_handleConnection(i);
                        // close(i);
                        // ft_removeClient(i);
                        // FD_CLR(i, &currentfds);
                    }
                }
            }
        // }
        // catch(const std::exception& e)
        // {
        //     std::cerr << e.what() << '\n';
        // }
    }
}


void Server::ft_sendToAllClients(char *data)
{
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        ft_sendData(it->first, data);
    }
}

Client& Server::getClientByNick(std::string nick)
{
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        std::cout << it->second.getNickName() << " " << nick << std::endl;
        if (it->second.getNickName() == nick)
            return it->second;
    }
    throw new std::invalid_argument("Error: client not found");
}

Client Server::ft_createClient(int fd)
{
    char nickname[10];
    
    if (ft_sendData(fd, "Enter your nickname: ") == -1)
        throw new std::runtime_error("Error: send failed");
    if (read(fd, nickname, 10) == -1) //10 can be defined as NICK_LEN
        throw new std::runtime_error("Error: read failed");
    std::string n(nickname);
    if (!n.empty()&& n[n.size() - 1] == '\n')
        n = n.substr(0, n.size() - 1);
    Client client(fd);
    client.setNickName(n);
    return client;
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
