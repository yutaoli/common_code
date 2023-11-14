#include "string_util.h"

int StringUtil::Spilt(const std::string &str, const std::string &delimiter, std::vector<std::string> &out)
{
    out.clear();
    if(str.empty() || delimiter.empty()) return 0;

    size_t pos1 = 0;
    size_t pos2 = 0;
    while((pos2 = str.find(delimiter, pos1)) != std::string::npos)
    {
        out.push_back(str.substr(pos1, pos2-pos1));
        pos1 = pos2 + delimiter.size();
    }
    if(pos1 <= str.size())
    {
        out.push_back(str.substr(pos1, str.size() - pos1));
    }
    return 0;
}