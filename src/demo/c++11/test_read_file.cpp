#include <chrono>
#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <string.h>
#include <unordered_map>

typedef struct
{
    std::string str_cust_code;                          // 客户代码
    std::string str_cuacc_code;                         // 资产账户
    std::map<std::string, std::string> map_stk_trdacct; // 证券账户
    std::map<std::string, std::string> map_stk_pbu;     // 交易单元
    std::string str_sessionid;                          // 会话凭证
    std::string str_int_org;                            // 内部机构
    char ch_acct_type;                                  // 账户类型
    std::string str_cuacct_attr;                        // 资产账户属性
    std::string str_channelid;                          // 业务通道
    std::string str_flash_port;                         // 软加速端口
    char ch_flash_mode;                                 // 软加速启用标志
    std::string str_auth_data;                          // 密码密文
} StApiAcct;

static int Spilt(const std::string &str, const std::string &delimiter, std::vector<std::string> &out)
{
  out.clear();
  if (str.empty() || delimiter.empty())
    return 0;

  size_t pos1 = 0;
  size_t pos2 = 0;
  while ((pos2 = str.find(delimiter, pos1)) != std::string::npos)
  {
    out.push_back(str.substr(pos1, pos2 - pos1));
    pos1 = pos2 + delimiter.size();
  }
  if (pos1 <= str.size())
  {
    out.push_back(str.substr(pos1, str.size() - pos1));
  }
  return 0;
}

typedef struct timediff
{
    timediff(std::string prompt_)
    {
        prompt = prompt_;
        begin.tv_sec = 0;
        begin.tv_nsec = 0;
        clock_gettime(CLOCK_REALTIME, &begin);
    }

    ~timediff()
    {
        end.tv_sec = 0;
        end.tv_nsec = 0;
        clock_gettime(CLOCK_REALTIME, &end);

        if (end.tv_nsec > begin.tv_nsec)
        {
            printf("%s,timediff:sec:%lu,nsec:%lu\n", prompt.c_str(), end.tv_sec - begin.tv_sec, end.tv_nsec - begin.tv_nsec);
        }
        else
        {
            printf("%s,timediff:sec:%lu,nsec:%lu\n", prompt.c_str(), end.tv_sec - begin.tv_sec - 1, 1000000000UL + end.tv_nsec - begin.tv_nsec);
        }
    }

    struct timespec begin;
    struct timespec end;
    std::string prompt;
    /*
       uint64_t getnowtime64()
       {
       struct timespec now={0,0};
       clock_gettime(CLOCK_REALTIME, &now);
       return now.tv_sec << 32 | (now.tv_nsec & 0xffffffff);
       }
     */
} timediff_t;

int ReadByifstream(std::string filename)
{
    timediff_t timediff(__FUNCTION__);

    size_t acct_total = 0;
    std::map<std::string, StApiAcct> accts;

    std::ifstream ifs_file;
    ifs_file.open(filename.c_str(), std::ios::in);
    if (!ifs_file.is_open() || !ifs_file.good())
    {
        printf("file:%s open error", filename.c_str());
        return false;
    }

    // 逐行读取csv文件
    std::string str_acct_line;
    std::getline(ifs_file, str_acct_line);
    std::vector<std::string> vec_acct_row;
    char buf[64] = {0};
    while (std::getline(ifs_file, str_acct_line))
    {
        vec_acct_row.clear();
        std::string str_acct_cell;
        std::stringstream ss_line_stream(str_acct_line);
        // 逐个单元格读取当前行
        while (std::getline(ss_line_stream, str_acct_cell, ','))
        {
            unsigned int buf_index = 0;
            for (unsigned int i = 0; i < str_acct_cell.size() && buf_index < sizeof(buf); i++)
            {
                if (str_acct_cell[i] == '\"' || str_acct_cell[i] == ' ')
                    continue;

                buf[buf_index] = str_acct_cell[i];
                buf_index++;
            }
            buf[buf_index] = 0;
            vec_acct_row.push_back(buf);
        }

        if (vec_acct_row.size() != 7)
            continue;
        acct_total++;

        std::map<std::string, StApiAcct>::iterator itr_info = accts.find(vec_acct_row[1]);
        if (itr_info != accts.end())
        {
            itr_info->second.map_stk_trdacct[vec_acct_row[2]] = vec_acct_row[3];
            itr_info->second.map_stk_pbu[vec_acct_row[2]] = vec_acct_row[4];
        }
        else
        {
            StApiAcct st_info;
            st_info.str_cust_code = vec_acct_row[0];
            st_info.str_cuacc_code = vec_acct_row[1];
            st_info.map_stk_trdacct[vec_acct_row[2]] = vec_acct_row[3];
            st_info.map_stk_pbu[vec_acct_row[2]] = vec_acct_row[4];
            st_info.str_int_org = vec_acct_row[5];
            st_info.str_auth_data = vec_acct_row[6];
            accts.insert(std::move(make_pair(st_info.str_cuacc_code, st_info)));
        }
    }

    ifs_file.close();
    return 0;
}

int ReadByfread(std::string filename)
{
    timediff_t timediff(__FUNCTION__);

#ifdef _MSC_VER
    constexpr size_t errmsg_size = 1024;
    char msg[errmsg_size] = {0};
#endif
    std::string errmsg;
    FILE *fp = NULL;

    fp = fopen(filename.c_str(), "r");
    if (fp == NULL)
    {
#ifdef _MSC_VER
        strerror_s(msg, errmsg_size, errno);
        errmsg = msg;
#else
        errmsg = strerror(errno);
#endif

        printf("fopen error,filename:%s,errno:%d,msg:%s", filename.c_str(), errno, errmsg.c_str());
        return -1;
    }

    constexpr int kBufSize = 1024;
    char buf[kBufSize] = {0};
    size_t acct_total = 0;
    std::map<std::string, StApiAcct> accts;
    std::vector<std::string> vec_acct_row;
    std::map<std::string, StApiAcct>::iterator itr_info;

    // 逐行读取
    while(fgets(buf, kBufSize, fp) != NULL)
    {
        
        size_t end = 0;
        size_t len = strlen(buf);
        for(unsigned int i = 0; i < len; i++)
        {
            if (buf[i] == '\"' || buf[i] == ' ' || buf[i] == '\n') continue;

            buf[end] = buf[i];
            end++;
        }
        buf[end] = 0;

        //printf("%s\n",buf);

        Spilt(buf, ",", vec_acct_row);

        if (vec_acct_row.size() != 7)
            continue;


        itr_info = accts.find(vec_acct_row[1]);
        if (itr_info != accts.end())
        {
            itr_info->second.map_stk_trdacct[vec_acct_row[2]] = vec_acct_row[3];
            itr_info->second.map_stk_pbu[vec_acct_row[2]] = vec_acct_row[4];
        }
        else
        {
            StApiAcct st_info;
            st_info.str_cust_code = vec_acct_row[0];
            st_info.str_cuacc_code = vec_acct_row[1];
            st_info.map_stk_trdacct[vec_acct_row[2]] = vec_acct_row[3];
            st_info.map_stk_pbu[vec_acct_row[2]] = vec_acct_row[4];
            st_info.str_int_org = vec_acct_row[5];
            st_info.str_auth_data = vec_acct_row[6];
            accts.insert(std::move(make_pair(st_info.str_cuacc_code, st_info)));
        }
        acct_total++;
    }
    printf("acct_total:%d\n",acct_total);

    fclose(fp);
    return 0;
}

int ReadByfreadUnordermap(std::string filename)
{
    timediff_t timediff(__FUNCTION__);

#ifdef _MSC_VER
    constexpr size_t errmsg_size = 1024;
    char msg[errmsg_size] = {0};
#endif
    std::string errmsg;
    FILE *fp = NULL;

    fp = fopen(filename.c_str(), "r");
    if (fp == NULL)
    {
#ifdef _MSC_VER
        strerror_s(msg, errmsg_size, errno);
        errmsg = msg;
#else
        errmsg = strerror(errno);
#endif

        printf("fopen error,filename:%s,errno:%d,msg:%s", filename.c_str(), errno, errmsg.c_str());
        return -1;
    }

    constexpr int kBufSize = 1024;
    char buf[kBufSize] = {0};
    size_t acct_total = 0;
    std::unordered_map<std::string, StApiAcct> accts;
    std::vector<std::string> vec_acct_row;
    std::unordered_map<std::string, StApiAcct>::iterator itr_info;
    accts.reserve(100 * 100 * 10000);

    // 逐行读取
    while(fgets(buf, kBufSize, fp) != NULL)
    {
        
        size_t end = 0;
        size_t len = strlen(buf);
        for(unsigned int i = 0; i < len; i++)
        {
            if (buf[i] == '\"' || buf[i] == ' ' || buf[i] == '\n') continue;

            buf[end] = buf[i];
            end++;
        }
        buf[end] = 0;

        //printf("%s\n",buf);
        Spilt(buf, ",", vec_acct_row);

        if (vec_acct_row.size() != 7)
            continue;


        itr_info = accts.find(vec_acct_row[1]);
        if (itr_info != accts.end())
        {
            itr_info->second.map_stk_trdacct[vec_acct_row[2]] = vec_acct_row[3];
            itr_info->second.map_stk_pbu[vec_acct_row[2]] = vec_acct_row[4];
        }
        else
        {
            StApiAcct st_info;
            st_info.str_cust_code = vec_acct_row[0];
            st_info.str_cuacc_code = vec_acct_row[1];
            st_info.map_stk_trdacct[vec_acct_row[2]] = vec_acct_row[3];
            st_info.map_stk_pbu[vec_acct_row[2]] = vec_acct_row[4];
            st_info.str_int_org = vec_acct_row[5];
            st_info.str_auth_data = vec_acct_row[6];
            accts.insert(std::move(make_pair(st_info.str_cuacc_code, st_info)));
        }
        acct_total++;
    }
    printf("acct_total:%d\n",acct_total);

    fclose(fp);
    return 0;
}

int main()
{
    std::string filename("lr_100w.csv");
    //ReadByifstream(filename);
    ReadByfread(filename);
    ReadByfreadUnordermap(filename);
    return 0;
}