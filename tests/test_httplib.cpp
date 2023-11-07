#include <CppUTest/CommandLineTestRunner.h>
#include "httplib.h"
#include "http_client_api.h"
#include <algorithm>

//#define PRINT_DEBUG

TEST_GROUP(httplib){
    //
    void setup(){} void teardown(){}

    //
    void Start(httplib::HttpSvr & http_svr, int port)
    {
        httplib::Server &svr = http_svr.GetSvr();
        svr.Get("/test_get", ClientTestSvrHandler);
        svr.Post("/test_post", ClientTestSvrHandler);

        http_svr.Listen("0.0.0.0", port);
        // client_test_log::WriteLog("===The server started at port:%d ok...===", port_);
        printf("===The server started at port:%d ok...===\n", port);
    }

    void Stop(httplib::HttpSvr &http_svr)
    {
        http_svr.Stop();
    }

    static void ClientTestSvrHandler(const httplib::Request &req, httplib::Response &res)
    {
        /*client_test_log::WriteLog("get req,path:%s,req.header:%s,req.body size:[%u],method:%s",
    req.path.c_str(),HttpClientApi::PrintHeaders(req.headers).c_str(), req.body.size(),req.method.c_str());*/
        printf("get req,req{{method:%s,path:%s,param:%s},{req.header:%s},req.body size:[%u]}\n",
           req.method.c_str(),req.path.c_str(), HttpClientApi::PrintParams(req.params).c_str(),HttpClientApi::PrintHeaders(req.headers).c_str(), req.body.size());

        int ret = 0;
        char res_buffer[128] = {0};
        size_t res_buffer_len = sizeof(res_buffer);
        if (req.path == "/test_post")
        {
            ret = 0;
            res.set_content(ResBuffer(ret, res_buffer, res_buffer_len), "application/json");
        }
        else if (req.path == "/test_get")
        {
            ret = 0;
            res.set_content(ResBuffer(ret, res_buffer, res_buffer_len), "application/json");
        }
        else
        {
            ret = -1;
            res.set_content(ResBuffer(ret, res_buffer, res_buffer_len), "application/json");
        }
        
       printf("set rsp,rsp{status:%d,reason:%s,headers:%s,body:%u,location:%s},"\
       "req{{method:%s,path:%s,param:%s},{req.header:%s},req.body size:[%u]}\n",
           res.status,res.reason.c_str(),HttpClientApi::PrintHeaders(res.headers).c_str(),res.body.size(),res.location.c_str(),
           req.method.c_str(),req.path.c_str(), HttpClientApi::PrintParams(req.params).c_str(),HttpClientApi::PrintHeaders(req.headers).c_str(), req.body.size());
        // TODO
    }

    static inline const char *ResBuffer(int ret, char *res_buffer, size_t res_buffer_len)
    {
        if (ret == 0)
        {
            snprintf(res_buffer, res_buffer_len, "{\"retcode\": \"%d\",\"msg\": \"%s\"}", ret, "Success");
        }
        else
        {
            snprintf(res_buffer, res_buffer_len, "{\"retcode\": \"%d\",\"msg\": \"%s\"}", ret, "fail");
        }

        return res_buffer;
    }
};

TEST(httplib,test_post_ok)
{
    httplib::HttpSvr http_svr;
    int port = 18081;
    Start(http_svr, port);

    int ret = HttpClientApi::TestPost("127.0.0.1", port, "10.100.70.55", 12345);
    CHECK(ret == 0);
}

TEST(httplib,test_get_ok)
{
    httplib::HttpSvr http_svr;
    int port = 18081;
    Start(http_svr, port);

    int ret = HttpClientApi::TestGet("127.0.0.1", port);
    CHECK(ret == 0);
    
}

TEST(httplib,test_post_connect_fail)
{
    int port = 18081;
    int ret = HttpClientApi::TestPost("127.0.0.1", port, "10.100.70.55", 12345);

    int error = static_cast<std::underlying_type<httplib::Error>::type>(httplib::Error::Connection);
    CHECK(ret == error);
}

TEST(httplib,test_get_connect_fail)
{
    int port = 18081;
    int ret = HttpClientApi::TestGet("127.0.0.1", port);

    int error = static_cast<std::underlying_type<httplib::Error>::type>(httplib::Error::Connection);
    CHECK(ret == error);
}
