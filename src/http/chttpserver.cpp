/*
 * function:对http-server的封装,目前使用crow进行
 * author:lihaiping1603@aliyun.com
 * date:2017-10-13
*/
#include "src/crow/crow.h"
#include "chttpserver.h"
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

using namespace std;

static std::string toString( std::ostream& str )
{
    std::ostringstream ss;
    ss << str.rdbuf();
    return ss.str();
}

//CHttpServer *CHttpServer::GetHttpServerInstance()
//{
//    if(m_selfServerInstance){
//        return m_selfServerInstance;
//    }
//    m_selfServerInstance=new CHttpServer();
//    assert(m_selfServerInstance);
//    return m_selfServerInstance;
//}

CHttpServer::CHttpServer()
{

}

CHttpServer::~CHttpServer()
{

}

#ifdef _USE_LIBEVHTP_
void CHttpServer::SetServerCb(string path, httpserver_callback_cb cb, void *arg, string param_key)
{
    ServerCbInfo_S tempCbInfo;
    tempCbInfo.fCb=cb;
    tempCbInfo.pArg=arg;
    tempCbInfo.strPath=path;
    tempCbInfo.strParamKey=param_key;
    m_vectServerCb.push_back(tempCbInfo);
}
//启动http-server
void CHttpServer::StartServer(uint32_t server_port)
{
     return;
}
#else  //use crow
//启动http-server
void CHttpServer::StartServer(uint32_t server_port){
     crow::SimpleApp objServerApp;
    // just set the router info here
    // do not add any extea etc.
    // http_router's argument obj should be a class name
    // which inherited `Handler`.

    HttpRouter("/jobs/get", jobs_get_cb,"jobs/get",std::string("request"));

    //启动http-server
    objServerApp.port(server_port).multithreaded().run();
}
#endif
