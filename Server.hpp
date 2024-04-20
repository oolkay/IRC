#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <cstring>
# include <cstdlib>
# include <iostream>
# include <poll.h>
# include <unistd.h>
# include <fcntl.h>
# include <sstream>
# include <string>
# include <map>
# include <cerrno>
# include <ctime>
# include "Parser.hpp"
# include "Executer.hpp"
# include "Client.hpp"


class Server
{
    private:
        int serverfd;
        struct sockaddr_in server_addr;
        std::map<int, Client> clients;
        //std::map<std::string, Channel> channels; ??do we need myfriend
    public:
        Server();
        ~Server();
        int ft_createIPv4Socket();
        void ft_setIPv4Adress(int port);
        void ft_setSocketOptions();
        void ft_bindSocket();
        void ft_listenPort();
        int ft_acceptConnection();
        int ft_sendData(int clientfd, std::string data);
        void ft_runserver();
        void ft_handleConnection(int clientfd);

        /*CLIENT FUNCTIONS*/
        void ft_addClient(int fd);
        void ft_removeClient(int fd);
        void ft_sendToAllClients(char *data);
        Client& getClientByNick(std::string nick);
        std::map<int, Client>& getClients() { return clients; }
        Client ft_createClient(int fd);

};



#endif