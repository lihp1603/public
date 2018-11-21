/************************************************************************/
/*本文的主要目的是作为client发送http到server         
*author:lihaiping1603@aliyun.com
*date:2017-06-27
*/
/************************************************************************/

#ifndef __CURL_HTTP_REQUEST_H__
#define __CURL_HTTP_REQUEST_H__
#include <string>

using namespace std;

namespace PublicTool{

	//借助curl实现
	class CurlHttpClient{
	public:
      CurlHttpClient();
       ~CurlHttpClient();

       //发送post请求
       bool CurlHttpPost(const string & strUrl, const string & strPostRequest,string & strResponse, string &strError, int &iError);

       //发送get请求
       bool CurlHttpGet(const string & strUrl, string & strResponse,string &strError, int &iError);

       //get
       bool CurlHttpGetEx(const string & strUrl, string & strResponse,string &strError, int &iError);
    };

}




#endif

