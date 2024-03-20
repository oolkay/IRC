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

int ft_createIPv4Socket()
{
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1)
    {
        std::cerr << "Error: socket failed" << std::endl;
        return (-1);
    }
    return (serverfd);
}

void ft_setIPv4Adress(struct sockaddr_in *server_addr, int port)
{
    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    server_addr->sin_addr.s_addr = INADDR_ANY;
}

void ft_setSocketOptions(int serverfd)
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

void ft_bindSocket(int serverfd, struct sockaddr_in *server_addr)
{
    int bindResult = ::bind(serverfd, (struct sockaddr *)server_addr, sizeof(*server_addr));
    if (bindResult == -1)
    {
        close(serverfd);
        std::cerr << "Error: bind faailed" << strerror(errno) <<  std::endl;
        exit(1);
    }
}

void ft_listenPort(int serverfd)
{
    int listenResult = listen(serverfd, QUEUE_SIZE);
    if (listenResult == -1)
    {
        std::cerr << "Error: listen failed" << std::endl;
        exit(1);
    }
}

int ft_acceptConnection(int serverfd)
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

int ft_sendData(int clientfd, char *data)
{
    int sendResult = send(clientfd, data, strlen(data), 0);
    if (sendResult == -1)
    {
        std::cerr << "Error: send failed" << std::endl;
        return (-1);
    }
    return (sendResult);
}

int main()
{
    int serverfd = ft_createIPv4Socket();
    struct sockaddr_in server_addr;
    ft_setIPv4Adress(&server_addr, 50000);
    ft_setSocketOptions(serverfd);
    ft_bindSocket(serverfd, &server_addr);
    ft_listenPort(serverfd);


    // fd_set readfds;
    // FD_ZERO(&readfds);
    // FD_SET(serverfd, &readfds);
    // int maxfd = serverfd;
    // select(serverfd + 1, &readfds, NULL, NULL, NULL);
    // if (FD_ISSET(serverfd, &readfds))
    // {
    //     std::cout << "New connection" << std::endl;
    // }
    int clientfd;
    while (true)
    {
        std::cout << "Waiting for connection" << std::endl;
        clientfd = ft_acceptConnection(serverfd);
        char *data = strdup("Hello, world!");
        ft_sendData(clientfd, data);
    }
    
    close(clientfd);
    close(serverfd);
    
    return (0);
}