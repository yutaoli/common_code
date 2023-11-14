#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <string>
#include <vector>
class StringUtil
{
    public:
static int Spilt(const std::string &str, const std::string &delimiter, std::vector<std::string> &out);
};
#endif