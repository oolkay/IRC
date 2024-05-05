#include "../include/Utils.hpp"

namespace Utils
{
    std::vector<std::string> split(const std::string &s, char delim)
    {
        std::vector<std::string> elems;
        int start = 0;
        int end = 0;

        while((size_t)end != std::string::npos)
        {
            end = s.find(delim, start);
            elems.push_back(s.substr(start, end - start));
            start = end + 1;
        }
        return elems;
    }

    std::string trim(const std::string &s, const std::string toTrim)
    {
        size_t start = s.find_first_not_of(toTrim);
        size_t end = s.find_last_not_of(toTrim);
        return s.substr(start, end - start + 1);
    }

    std::string join(std::vector<std::string> v, std::string delim, int start, int end)
    {
        std::string result = "";
        if(end == -1)
            end = v.size();
        for(int i = start; i < end; i++)
        {
            result += v[i];
            if(i != end - 1)
                result += delim;
        }
        return result;
    }

    void ut_write(int fd, const std::string &s)
    {
        write(fd, s.c_str(), s.size());
    }

    void ut_send(int fd, const std::string &s)
    {
        send(fd, s.c_str(), s.size(), 0);
    }

    std::string getTime()
    {
    	time_t now = time(0);
    		tm *ltm = localtime(&now);
    		std::stringstream ss;
    		ss 	<< std::setfill('0') << std::setw(2) << ltm->tm_mday << '-'
    			<< std::setfill('0') << std::setw(2) << (1 + ltm->tm_mon) << '-'
    			<< std::setfill('0') << std::setw(4) << (1900 + ltm->tm_year) << ' '
    			<< std::setfill('0') << std::setw(2) << ltm->tm_hour << ':'
    			<< std::setfill('0') << std::setw(2) << ltm->tm_min << ':'
    			<< std::setfill('0') << std::setw(2) << ltm->tm_sec;
    		return ss.str();
    }

    std::string toUpper(const std::string &s)
    {
        std::string result = s;
        for(size_t i = 0; i < s.size(); i++)
            result[i] = toupper(s[i]);
        return result;
    }

}