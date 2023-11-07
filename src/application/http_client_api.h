#ifndef HTTP_CLIENT_API_H
#define HTTP_CLIENT_API_H

#include "httplib.h"
#include <string>

class HttpClientApi
{
    public:
static int TestPost(const std::string &server_ip, int server_port, 
const std::string &register_ip, int register_port);

static int TestGet(const std::string &server_ip, int server_port); 

static std::string PrintHeaders(const httplib::Headers &headers);
static std::string PrintParams(const httplib::Params &params);
};

#endif