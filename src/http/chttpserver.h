/*
 * function:对http-server的封装,目前使用crow进行
 * author:lihaiping1603@aliyun.com
 * date:2017-10-13
*/

#ifndef __CHTTPSERVER_H_
#define __CHTTPSERVER_H_
//#include "src/crow/crow.h"
#include <string>
#include <vector>

using namespace std;

#define _USE_LIBEVHTP_

//数据回调的函数格式
//req_read:client请求的数据
//resp_write:回复给client的数据
typedef void (* httpserver_callback_cb)(string req_read, string& resp_write, void * arg);

class CHttpServer
{
public:
#ifdef _USE_LIBEVHTP_
    //设置回调
    static void SetServerCb(string path,httpserver_callback_cb cb, void* arg,string param_key="");
    //启动server
    static void StartServer(uint32_t server_port);
#else //use crow
#define HttpRouter(url, fCb,pArg,strParamKey)\
    do\
    {\
        CROW_ROUTE(objServerApp, url)\
        ([fCb,pArg,strParamKey](const crow::request& req, crow::response& res) {\
             try{\
                 string dataString;\
                 string strRespon;\
                 if(!strParamKey.empty()){\
                     dataString = req.url_params.get(strParamKey);\
                 }else{\
                     ostream outParam;\
                     outParam<<req.url_params;\
                     dataString=toString(outParam);\
                 }\
                 if(fCb){/*回调*/ \
                     fCb(dataString,strRespon,pArg);\
                 }\
                 if(!strRespon.empty()){\
                     res.write(strRespon);\
                     res.end();\
                 }\
             }\
             catch (...){\
                 res.write("404:error!");\
                 res.end();\
             }\
         });     \
    }while(0)

    //启动server
    static void StartServer(uint32_t server_port);
#endif

private:
    CHttpServer();
    ~CHttpServer();
#ifdef _USE_LIBEVHTP_
    typedef struct __tagServerCbInfo_S{
        string strPath;
        httpserver_callback_cb fCb;
        void* pArg;
        string strParamKey;
    }ServerCbInfo_S;
    static vector<ServerCbInfo_S> m_vectServerCb;
#endif
//    CHttpServer* m_selfServerInstance;
//    CHttpServer* GetHttpServerInstance();

};





#endif // __CHTTPSERVER_H_
