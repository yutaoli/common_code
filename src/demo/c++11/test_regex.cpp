#include <regex>
#include <vector>

#include <regex.h> // c regex
#include <sys/types.h>

// https://regex101.com/
// https://regexper.com/

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

int CRegexSearchMatchAll(const std::string &input, const std::string &re, Matchs &out_matchs)
{
    regex_t reg;
    char errmsg[512] = {0};
    int iRet = 0;
    if (0 != (iRet = regcomp(&reg, re.data(), REG_EXTENDED))) 
    {
        regerror(iRet,&reg,errmsg,sizeof(errmsg));
        printf("regcomp iRet:%d,errmsg:%s\n",iRet,errmsg);
        return -1;
    }
    
    out_matchs.clear();
    unsigned iOffset = 0;
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
                printf("regexec iRet:%d,errmsg:%s\n",iRet,errmsg);
                iRet = status;
                goto end;
            }
            else if (status == REG_NOMATCH)
            {
                iRet = 0;
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
    return iRet;
}
int CppRegexSearchMatchAll(const std::string &input, const std::string &re, Matchs &out_matchs)
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

int TestRegexSearchMatchOne()
{
    const std::string input = "ABC:1->   PQR:2;;;   XYZ:3<<<";
    // const std::string input =  "ABC:1->   PQR:2;;;   XYZ:3<<<PQR:2;;;";
    const std::regex r(R"((\w+):(\w+);)");
    std::smatch m;
    if (std::regex_search(input, m, r))
    {
        printf("m size:%u\n", m.size());

        for (unsigned int i = 0; i < m.size(); i++)
        {
            printf("m[%u]:%s\n", i, m[i].str().c_str());
        }
    }
    printf("%s======\n", __FUNCTION__);
    return 0;
}

int TestRegexSearch_MatchMoreThanOne()
{
    const std::string input = "ABC:1->   PQR:2;;;   XYZ:3<<<";
    const std::regex r(R"((\w+):(\d))");
    std::vector<std::smatch> matches{
        std::sregex_iterator{input.begin(), input.end(), r},
        std::sregex_iterator{}};

    printf("matches size:%u\n", matches.size());
    for (unsigned int v_i = 0; v_i < matches.size(); v_i++)
    {
        std::smatch &match = matches[v_i];

        for (unsigned int i = 0; i < match.size(); i++)
        {
            printf("%u:position:%u,length:%u,match[%u]:%s,str(%u):%s\n",
                   v_i, match.position(i), match.length(i), i, match[i].str().c_str(), i, match.str(i).c_str());
        }
    }

    printf("%s======\n", __FUNCTION__);
    return 0;
}

int TestRegexSearch_MatchMoreThanOne_New()
{
    //const std::string input = "hello.exe,,,a.jpg,this is a bird.css";
    //const std::string re = "\\.([a-zA-Z0-9]+)$";
    //int n_group = 3;

     //const std::string input = "hello@gmail-example.com;world@gmail.com;test@gmail.com;";
     //const std::string re = "(\\w+([-+.]\\w+)*@\\w+([-+.]\\w+)*).com;";

    //const std::string input = "http://10.203.60.5:9122/heartbeat?param=4";
    //const std::string re = "(?:([a-z]+):\\/\\/)?(?:\\[([\\d:]+)\\]|([^:\\/?#]+))(?::(\\d+))?";
    //const std::string re = "(?:([a-z]+):\\/\\/)?(?:\\[([[[:digit:]]:]+)\\]|([^:\\/?#]+))(?::([[:digit:]]+))?";

    //const std::string input = "ABC:1->   PQR:2;;;   XYZ:3<<<";
    //const std::string re = "(\\w+):([[:digit:]])";//const std::string re = "(\\w+):(\\d)";
    
    const std::string input = "http://10.203.60.5:9122/heartbeat?param=4&b=3";
    //const std::string re = "^([A-Za-z]+):\\/\\/([0-9.\\-A-Za-z]+)(:([[:digit:]]+))?(\\/([^?#]*))?(\?([^#]*))?$";
    const std::string re = "^([A-Za-z]+):\\/\\/([0-9A-Za-z\\.\\-]+)(:([0-9]+))?(\\/([^?#]*))?(\\?([^#]*))?$";

    Matchs matches;
    CppRegexSearchMatchAll(input, re, matches);

    printf("CppRegexSearchMatchAll matches size:%u\n", matches.size());
    for (unsigned int v_i = 0; v_i < matches.size(); v_i++)
    {
        Match &match = matches[v_i];

        for (unsigned int i = 0; i < match.groups.size(); i++)
        {
            GroupResult &result = match.groups[i];
            printf("%u:position:%u,length:%u,match[%u]:%s\n",
                   v_i, result.postion, result.length, i, result.str.c_str());
        }
    }

    printf("CppRegexSearchMatchAll======\n", __FUNCTION__);

    matches.clear();
    int ret = CRegexSearchMatchAll(input, re, matches);
    printf("\n");
    printf("CRegexSearchMatchAll matches size:%u,ret:%d\n", matches.size(), ret);
    for (unsigned int v_i = 0; v_i < matches.size(); v_i++)
    {
        Match &match = matches[v_i];

        for (unsigned int i = 0; i < match.groups.size(); i++)
        {
            GroupResult &result = match.groups[i];
            printf("%u:position:%u,length:%u,match[%u]:%s\n",
                   v_i, result.postion, result.length, i, result.str.c_str());
        }
    }

    printf("CRegexSearchMatchAll======\n", __FUNCTION__);

    printf("%s======\n", __FUNCTION__);
    return 0;
}

int TestRegexMatch_VsRegexSearch()
{
    const std::string input = "ABC:1->   PQR:2;;;   XYZ:3<<<";
    // const std::string input =  "ABC:1->   PQR:2;;;   XYZ:3<<<PQR:2;;;";
    // const std::regex r(R"((\w+):(\w+);)");// 等价于 const std::regex r("(\\w+):(\\w+);")
    const std::regex r("(\\w+):(\\w+);");
    std::smatch m;
    const char *promt = "regex_match";
    if (std::regex_match(input, m, r))
    {
        printf("%s m size:%u\n", promt, m.size());

        for (unsigned int i = 0; i < m.size(); i++)
        {
            printf("m[%u]:%s\n", i, m[i].str().c_str());
        }
        printf("%s done.\n", promt);
    }
    else
    {
        printf("%s return false, %s done.\n", promt, promt);
    }

    promt = "regex_search";
    if (std::regex_search(input, m, r))
    {
        printf("%s m size:%u\n", promt, m.size());

        for (unsigned int i = 0; i < m.size(); i++)
        {
            printf("m[%u]:%s\n", i, m[i].str().c_str());
        }
        printf("%s done.\n", promt);
    }
    else
    {
        printf("%s return false, %s done.\n", promt, promt);
    }
    printf("%s======\n", __FUNCTION__);
    return 0;
}

int TestRegexSearch_MatchMoreThanOneGroupNotContinues()
{
    // const std::string input = "ABC:1->   PQR:2;;;   XYZ:3<<<";
    // const std::regex r(R"((\w+):(\d))");
    const std::string input = "hello@gmail-example.com;world@gmail.com;test@gmail.com;";
    const std::regex r("(\\w+([-+.]\\w+)*@\\w+([-+.]\\w+)*).com;");
    std::vector<std::smatch> matches{
        std::sregex_iterator{input.begin(), input.end(), r},
        std::sregex_iterator{}};

    printf("matches size:%u\n", matches.size());
    for (unsigned int v_i = 0; v_i < matches.size(); v_i++)
    {
        std::smatch &match = matches[v_i];

        for (unsigned int i = 0; i < match.size(); i++)
        {
            printf("%u:position:%u,length:%u,match[%u]:%s,str(%u):%s\n",
                   v_i, match.position(i), match.length(i), i, match[i].str().c_str(), i, match.str(i).c_str());
        }
    }

    printf("%s======\n", __FUNCTION__);
    return 0;
}
int TestCRegex()
{
    int status, i;
    int cflags = REG_EXTENDED;
    const size_t nmatch = 10;
    regmatch_t pmatch[nmatch];
    regex_t reg;

    // const char* pattern = "(\\w+([-+.]\\w+)*@\\w+([-+.]\\w+)*).com;";
    // const char *buf = "hello@gmail.com;world@gmail.com;test@gmail.com;";
    const char *pattern = "(\\w+([-+.]\\w+)*@\\w+([-+.]\\w+)*).com;";
    const char *buf = "hello@gmail-example.com;world@gmail.com;test@gmail.com;";
    // const char* pattern = "(\\w+):(\\d)";
    // const char *buf = "ABC:1->   PQR:2;;;   XYZ:3<<<";
    // const char *buf = "ABC:1";

    regcomp(&reg, pattern, cflags);
    int offset = 0;
    int len = strlen(buf);
    while (offset < len)
    {
        status = regexec(&reg, buf, nmatch, pmatch, 0);
        if (status == REG_NOMATCH)
        {
            printf("no match\n");
            goto end;
        }
        else if (status == 0)
        {
            printf("match: \n");
            for (unsigned i = 0; i < nmatch; ++i)
            {
                if (pmatch[i].rm_so == -1 || pmatch[i].rm_eo == -1)
                {
                    continue;
                }
                std::string str(buf + pmatch[i].rm_so, pmatch[i].rm_eo - pmatch[i].rm_so);
                printf("str%u:%s,position:%d\n", i, str.c_str(), pmatch[i].rm_so);
            }
        }
        buf += pmatch[0].rm_eo;
        offset += pmatch[0].rm_eo;
    }
end:
    regfree(&reg);

    printf("%s======\n", __FUNCTION__);
    return 0;
}

int TestCRegexDeom()
{
    const char *pattern = "(\\w+([-+.]\\w+)*@\\w+([-+.]\\w+)*).com;";
    const char *buf = "hello@gmail-example.com;world@gmail.com;test@gmail.com;";

    const char *s = buf;
    const char *re = pattern;
    regex_t regex;
    int nmatch = 5;
    regmatch_t pmatch[1];
    regoff_t off, len;

    if (regcomp(&regex, re, REG_EXTENDED))
        exit(EXIT_FAILURE);

    printf("String = \"%s\"\n", pattern);
    printf("Matches:\n");

    for (unsigned int i = 0; i > 0; i++)
    {
        if (regexec(&regex, s, nmatch, pmatch, 0))
            break;

        off = pmatch[0].rm_so + (s - pattern);
        len = pmatch[0].rm_eo - pmatch[0].rm_so;
        printf("#%zu:\n", i);
        printf("offset = %jd; length = %jd\n", (intmax_t)off,
               (intmax_t)len);
        printf("substring = \"%.*s\"\n", len, s + pmatch[0].rm_so);

        s += pmatch[0].rm_eo;
    }
    return 0;
}

int main()
{
    TestRegexSearchMatchOne();
    TestRegexSearch_MatchMoreThanOne();
    TestRegexSearch_MatchMoreThanOne_New();

    TestRegexMatch_VsRegexSearch();
    TestRegexSearch_MatchMoreThanOneGroupNotContinues();
    TestCRegex();
    TestCRegexDeom();
    return 0;
}
/*
[liyutao@fsptMaster:/home/liyutao/data/software/common_code/src/demo/c++11]$ ./test_regex
m size:3
m[0]:PQR:2;
m[1]:PQR
m[2]:2
TestRegexSearchMatchOne======
matches size:3
0:position:0,length:5,match[0]:ABC:1,str(0):ABC:1
0:position:0,length:3,match[1]:ABC,str(1):ABC
0:position:4,length:1,match[2]:1,str(2):1
1:position:10,length:5,match[0]:PQR:2,str(0):PQR:2
1:position:10,length:3,match[1]:PQR,str(1):PQR
1:position:14,length:1,match[2]:2,str(2):2
2:position:21,length:5,match[0]:XYZ:3,str(0):XYZ:3
2:position:21,length:3,match[1]:XYZ,str(1):XYZ
2:position:25,length:1,match[2]:3,str(2):3
TestRegexSearch_MatchMoreThanOne======
regex_match return false, regex_match done.
regex_search m size:3
m[0]:PQR:2;
m[1]:PQR
m[2]:2
regex_search done.
TestRegexMatch_VsRegexSearch======
matches size:3
0:position:0,length:24,match[0]:hello@gmail-example.com;,str(0):hello@gmail-example.com;
0:position:0,length:19,match[1]:hello@gmail-example,str(1):hello@gmail-example
0:position:55,length:0,match[2]:,str(2):
0:position:11,length:8,match[3]:-example,str(3):-example
1:position:24,length:16,match[0]:world@gmail.com;,str(0):world@gmail.com;
1:position:24,length:11,match[1]:world@gmail,str(1):world@gmail
1:position:55,length:0,match[2]:,str(2):
1:position:55,length:0,match[3]:,str(3):
2:position:40,length:15,match[0]:test@gmail.com;,str(0):test@gmail.com;
2:position:40,length:10,match[1]:test@gmail,str(1):test@gmail
2:position:55,length:0,match[2]:,str(2):
2:position:55,length:0,match[3]:,str(3):
TestRegexSearch_MatchMoreThanOneGroupNotContinues======
match:
主规则的str---------hello@gmail-example.com;

str----------hello@gmail-example

str-----------example

TestCRegex======


*/