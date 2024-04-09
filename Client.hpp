#ifndef CLIENT_HPP
#define CLIENT_HPP


class Client
{
    private:
        int _fd;

    public:
        void setFd(int fd);
        int getFd() { return _fd; }


};


#endif