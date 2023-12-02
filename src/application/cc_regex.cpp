#include <regex>
#include <regex.h>
#include <sys/types.h>

#include "cc_regex.h"

int CcRegex::CRegexSearchMatchAll(const std::string &input, const std::string &re, Matchs &out_matchs)
{
    regex_t reg;
    char errmsg[512] = {0};
    int ret = 0;
    if (0 != (ret = regcomp(&reg, re.data(), REG_EXTENDED))) 
    {
        regerror(ret,&reg,errmsg,sizeof(errmsg));
        printf("regcomp ret:%d,errmsg:%s\n",ret,errmsg);
        return -1;
    }
    
    out_matchs.clear();
    unsigned iLen = input.size();
    unsigned nmatch = reg.re_nsub + 1;
    regmatch_t pmatch[nmatch];

    int offset = 0;
    const char *buf = input.c_str();
    int len = strlen(buf);
    int status = 0;
    while (offset < len)
    {
        status = regexec(&reg, buf, nmatch, pmatch, 0);
        if (status != 0)
        {
            if (status != REG_NOMATCH)
            {
                regerror(status,&reg,errmsg,sizeof(errmsg));
                printf("regexec ret:%d,errmsg:%s\n",ret,errmsg);
                ret = status;
                goto end;
            }
            else if (status == REG_NOMATCH)
            {
                ret = 0;
                goto end;
            }
        }
        else if (status == 0)
        {
            Match c_match;
            for (unsigned i = 0; i < nmatch; ++i)
            {
                if (pmatch[i].rm_so == -1 || pmatch[i].rm_eo == -1)
                {
                    //printf("%u:rm_so:%d,rm_eo:%d\n",i,pmatch[i].rm_so,pmatch[i].rm_eo);
                    c_match.groups.push_back(GroupResult());
                    continue;
                }

                GroupResult result;
                result.str.assign(buf + pmatch[i].rm_so, pmatch[i].rm_eo - pmatch[i].rm_so);
                result.length = result.str.size();
                result.postion = pmatch[i].rm_so + offset;
                c_match.groups.push_back(result);
                //printf("str%u:%s,position:%d\n", i, result.str.c_str(), pmatch[i].rm_so);
            }
            out_matchs.push_back(c_match);
        }

        offset += pmatch[0].rm_eo;
        buf += pmatch[0].rm_eo;
    }

end:
    // 释放正则表达式
    regfree(&reg);
    return ret;
}
int CcRegex::CppRegexSearchMatchAll(const std::string &input, const std::string &re, Matchs &out_matchs)
{
    try{
        const std::regex r(re.c_str());
        std::vector<std::smatch> matches{
            std::sregex_iterator{input.begin(), input.end(), r},
            std::sregex_iterator{}};

        out_matchs.clear();
        for (unsigned int v_i = 0; v_i < matches.size(); v_i++)
        {
            std::smatch &match = matches[v_i];

            Match cpp_match;
            for (unsigned int i = 0; i < match.size(); i++)
            {
                GroupResult result;
                result.postion = match.position(i);
                result.length = match.length(i);
                result.str = match[i].str();
                cpp_match.groups.push_back(result);
            }
            out_matchs.push_back(cpp_match);
        }
    }
    catch (const std::regex_error &e)
    {
        printf("regex_error:%s\n", e.what());
        return -1;
    }
    return 0;
}