#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"
#include "Utils.hpp"

class Client;

class Channel
{
    private:
        std::string _name;
        std::vector<Client> _clients;
        std::vector<Client> _operators;
        std::vector<Client> _invList;
        std::vector<Client> _banList;
        std::string _topic;
        std::vector<char> _modes;
        std::string _key;


    public:
        Channel();
        ~Channel();

        //Setter
        inline void setName(std::string name) { _name = name; }
        inline void addClient(Client& client) { _clients.push_back(client); }
        inline void addOperator(Client& client) { _operators.push_back(client); }
        inline void addInvList(Client& client) { _invList.push_back(client); }
        inline void addBanList(Client& client) { _banList.push_back(client); }
        inline void setTopic(std::string topic) { _topic = topic; }
        inline void setModes(std::vector<char> modes) { _modes = modes; }
        inline void setKey(std::string key) { _key = key; }


        //Getter
        inline std::string getName() const { return _name; }
        inline std::vector<Client>& getClients()  { return _clients; }
        inline std::vector<Client>& getOperators()  { return _operators; }
        inline std::vector<Client>& getInvList()  { return _invList; }
        inline std::vector<Client>& getBanList()  { return _banList; }
        inline std::string getTopic() const { return _topic; }
        inline std::vector<char> getModes() const { return _modes; }

        //Functions
        void removeClient(Client& client);
        void removeOperator(Client& client);
        void removeInvList(Client& client);
        void removeBanList(Client& client);
        bool isClientInChannel(Client& client);
        bool isClientOperator(Client& client);
        bool isClientInInvList(Client& client);
        bool isClientInBanList(Client& client);
        bool isChannelPasswordProtected();
        void sendToAll(std::string message);

        


};

#endif