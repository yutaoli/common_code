#ifndef CC_REGEX_H
#define CC_REGEX_H

#include<string>
#include <vector>

struct GroupResult
{
    GroupResult()
    {
        postion = 0;
        length = 0;
    }
    std::string str;
    unsigned int postion;
    unsigned int length;
};

struct Match
{
    std::vector<GroupResult> groups; // 0:match, other:group
};

typedef std::vector<Match> Matchs;

class CcRegex
{
public:
static int CRegexSearchMatchAll(const std::string &input, const std::string &re, Matchs &out_matchs);
static int CppRegexSearchMatchAll(const std::string &input, const std::string &re, Matchs &out_matchs);
};
#endif