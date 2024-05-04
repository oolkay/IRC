#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string>
#include <exception>

class Exception : public std::exception
{
    private:
        std::string _message;
    public:
        Exception(std::string message) : _message(message) {}
        virtual const char* what() const throw()
        {
            return _message.c_str();
        }
        virtual ~Exception() throw() {}
};

#endif