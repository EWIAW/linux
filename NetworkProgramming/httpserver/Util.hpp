#include <iostream>

class Util
{
public:
    static std::string getOneLine(std::string &buffer, const std::string &sep)
    {
        ssize_t n = buffer.find(sep);
        if (n == std::string::npos)
            return "";

        std::string sub = buffer.substr(0, n);
        buffer.erase(0, sub.size() + sep.size());
        return sub;
    }
};