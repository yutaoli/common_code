#include "httplib.h"
#include "http_client_api.h"

int HttpClientApi::TestPost(const std::string &server_ip, int server_port, 
const std::string &register_ip, int register_port) {
    char scheme_host_port[64] = {0};
    snprintf(scheme_host_port,sizeof(scheme_host_port),"http://%s:%d",server_ip.c_str(),server_port);

  httplib::Headers headers;
  headers.insert({"User-Agent", "Mozilla/5.0"});

  httplib::Client cli_post(scheme_host_port);
  unsigned int connect_timeout_ms = 300;
  cli_post.set_connection_timeout(0, connect_timeout_ms * 1000);

  char body[128] = {0};
  snprintf(body, sizeof(body), "{\"instance\": \"%s:%d\"}", register_ip.c_str(),register_port);
  //std::string body = "{\"instance\": \"10.203.0.118:9122\"}";
  auto res = cli_post.Post("/test_post", headers, body, strlen(body),
                           "application/json");
  if (res == nullptr) {
    int ret = static_cast<std::underlying_type<httplib::Error>::type>(res.error());
    /*client_test_log::WriteLog("/client/register error:%d,errmsg:%s,scheme_host_port:%s",
           ret, to_string(res.error()).c_str(),scheme_host_port);*/
           return ret;
  } else {
    if (res->status == 200) {
      /*client_test_log::WriteLog("/client/register,status:%d,headers:[%s],body:[%s],scheme_host_port:%s", res->status,
             PrintHeaders(res->headers).c_str(), res->body.c_str(),scheme_host_port);*/
    } else {
      //client_test_log::WriteLog("/client/register error,status:%d,scheme_host_port:%s", res->status,scheme_host_port);
      return res->status;
    }
  }
  return 0;
}

int HttpClientApi::TestGet(const std::string &server_ip, int server_port) {
  char scheme_host_port[64] = {0};
  snprintf(scheme_host_port, sizeof(scheme_host_port), "http://%s:%d",
           server_ip.c_str(), server_port);

  httplib::Headers headers;
  headers.insert({"User-Agent", "Mozilla/5.0"});

  httplib::Client cli_get(scheme_host_port);
  httplib::Params params;
  params.insert(std::make_pair("ip", "127.0.0.1"));
  //std::string body = "{\"instance\": \"10.203.0.118:9122\",\"status\": \"1\"}";
  char body[128] = {0};

  std::string path = "/test_get";
  auto res = cli_get.Get(path, params, headers);
  if (res == nullptr) {
    int ret = static_cast<std::underlying_type<httplib::Error>::type>(res.error());

           printf("%s/%s,req:{param:%s,header:%s,body:%s},"\
           "error:%d,errmsg:%s\n", 
             scheme_host_port,path.c_str(),PrintParams(params).c_str(), PrintHeaders(headers).c_str(),body,
             static_cast<std::underlying_type<httplib::Error>::type>(res.error()),to_string(res.error()).c_str());
             return ret;
  } else {
    if (res->status == 200) {
      printf("%s/%s,req:{param:%s,header:%s,body:%s},"\
           "rsp{status:%d,headers:[%s],body:[%s]}\n", 
             scheme_host_port,path.c_str(),PrintParams(params).c_str(), PrintHeaders(headers).c_str(),body,
             res->status,PrintHeaders(res->headers).c_str(), res->body.c_str());
    } else {
             printf("%s/%s,req:{param:%s,header:%s,body:%s},"\
           "rsp{status:%d,headers:[%s],body:[%s]}\n", 
             scheme_host_port,path.c_str(),PrintParams(params).c_str(), PrintHeaders(headers).c_str(),body,
             res->status,PrintHeaders(res->headers).c_str(), res->body.c_str());
             return res->status;
    }
  }
  return 0;
}

std::string HttpClientApi::PrintHeaders(const httplib::Headers &headers) {
  httplib::Headers::const_iterator m_it;
  std::string out;
  char buf[128] = {0};
  for (m_it = headers.begin(); m_it != headers.end(); m_it++) {
    snprintf(buf, sizeof(buf), "[k:%s,v:%s]", m_it->first.c_str(),
             m_it->second.c_str());
    out += buf;
  }
  return out;
}

std::string HttpClientApi::PrintParams(const httplib::Params &params) {
  httplib::Params::const_iterator m_it;
  std::string out;
  char buf[128] = {0};
  for (m_it = params.begin(); m_it != params.end(); m_it++) {
    snprintf(buf, sizeof(buf), "[k:%s,v:%s]", m_it->first.c_str(),
             m_it->second.c_str());
    out += buf;
  }
  return out;
}