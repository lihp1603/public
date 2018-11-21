/************************************************************************/
/*本文的主要目的是作为client发送http到server         
*author:lihaiping1603@aliyun.com
*date:2017-06-27
*/
/************************************************************************/



#include "CurlHttpClient.h"
#include "../curl/curl.h"
#include "../curl/easy.h"
#include <string>
#include <stddef.h>

using namespace std;


#ifdef __WIN32__
#pragma comment(lib,"lib/curl/libcurl.lib")
#endif

namespace PublicTool{

    static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
    {
        std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
        if( NULL == str || NULL == buffer )
        {
            return -1;
        }
        char* pData = (char*)buffer;
        str->append(pData, size * nmemb);
        return nmemb;
    }


    static void get_error_info(int resId, std::string & strError)
    {
        switch (resId)
        {
        case CURLE_COULDNT_CONNECT:
            strError = "couldn't connect";
            break;
        case CURLE_LOGIN_DENIED:
            strError =
                "user, password or similar was not accepted and we failed to login.";
            break;
        case CURLE_REMOTE_FILE_NOT_FOUND:
            strError = "remote file not found";
            break;
        case CURLE_COULDNT_RESOLVE_HOST:
            strError = "couldn't resolve host";
            break;
        default:
            strError = resId;
            break;
        }
    }


    CurlHttpClient::CurlHttpClient()
    {

    }

    CurlHttpClient::~CurlHttpClient()
    {

    }

    bool CurlHttpClient::CurlHttpPost( const string & strUrl, const string & strPostRequest, string & strResponse, string &strError, int &iError )
    {
        bool bError = true;
        CURLcode res;
        CURL *curl=NULL;
        struct curl_slist *headers = NULL; // init to NULL is important
        headers = curl_slist_append(headers, "Accept: application/json");
#ifdef _CB_PHP_POST_
        headers = curl_slist_append(headers, "Content-Type:application/x-www-data-urlencoded");
#else
        headers = curl_slist_append(headers, "Content-Type: application/json");
#endif
        headers = curl_slist_append(headers, "charsets: utf-8");
        curl = curl_easy_init();
        if (NULL == curl)
        {
            bError=false;
            iError = CURLE_FAILED_INIT;
            return bError;
        }
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPostRequest.c_str());
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void * )&strResponse);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
        //这个接口是同步阻塞的，会直到server端的数据返回,才会返回
        res = curl_easy_perform(curl);
        if (CURLE_OK != res)
        {
            bError = false;
            get_error_info(res, strError);
            iError = res;
        }
         curl_easy_cleanup(curl);
        return bError;

    }

    bool CurlHttpClient::CurlHttpGet(const string & strUrl, string & strResponse,string &strError, int &iError)
    {
        bool bError = true;
        CURLcode res;
        CURL *curl;
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        curl = curl_easy_init();
        if (NULL == curl)
        {
            bError=false;
            iError = CURLE_FAILED_INIT;
            return bError;
        }
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void * )&strResponse);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
        //这个接口是同步阻塞的，会直到server端的数据返回,才会返回
        res = curl_easy_perform(curl);
        if (CURLE_OK != res)
        {
            bError = false;
            get_error_info(res, strError);
            iError = res;
        }
        curl_easy_cleanup(curl);
        return bError;
    }

    bool CurlHttpClient::CurlHttpGetEx(const string &strUrl, string &strResponse, string &strError, int &iError)
    {
        bool bError = true;
        CURLcode res;
        CURL *curl;
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        //headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        curl = curl_easy_init();
        if (NULL == curl)
        {
            bError=false;
            iError = CURLE_FAILED_INIT;
            return bError;
        }
        curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void * )&strResponse);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
        res = curl_easy_perform(curl);
        if (CURLE_OK != res)
        {
            bError = false;
            get_error_info(res, strError);
            iError = res;
        }
        else
        {
            long retcode = 0;
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode);
            if((CURLE_OK != res) || (200 != retcode))
            {
                bError = false;
                iError = retcode;
            }
        }
        curl_easy_cleanup(curl);
        return bError;

    }

}

