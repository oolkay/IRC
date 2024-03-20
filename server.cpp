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


int main()
{
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1)
    {
        std::cerr << "Error: socket failed" << std::endl;
        return (1);
    }
    int q = 0;
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &q, sizeof(int)) == -1)
    {
        std::cerr << "Error: setsockopt failed" << std::endl;
        return (1);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8081);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (fcntl(serverfd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Error: fcntl failed" << std::endl;
        return (1);
    }
    if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        close(serverfd);
        std::cerr << "Error: bind failed" << std::endl;
        return (1);
    }
    if (listen(serverfd, 10) == -1)
    {
        std::cerr << "Error: listen failed" << std::endl;
        return (1);
    }
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(serverfd, &readfds);
    int maxfd = serverfd;
    select(serverfd + 1, &readfds, NULL, NULL, NULL);
    if (FD_ISSET(serverfd, &readfds))
    {
        std::cout << "New connection" << std::endl;
    }
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int clientfd = accept(serverfd, (struct sockaddr *)&client_addr, &client_addr_size);
    FD_SET(clientfd, &readfds);
    if (clientfd == -1)
    {
        close(serverfd);
        std::cerr << "Error: accept failed" << std::endl;
        return (1);
    }
    char *msg = strdup("Hello, World! from server\r\n\n");
    send(clientfd, msg, strlen(msg), 0);
    close(serverfd);
    close(clientfd);
    FD_CLR(clientfd, &readfds);
    
    return (0);
}