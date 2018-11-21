/************************************************************************/
/* 这里的实现功能主要为检查本地路径下的文件是否存在,不存在的时候，需要去ucloud获取,以及上传用户文件
*author:lihaiping1603@aliyun.com
*date:2017-08-31
*/
/************************************************************************/


#ifndef __FILE_MANAGE_H_
#define __FILE_MANAGE_H_

#include <string>
#include "../http/CurlHttpClient.h"

using namespace std;

namespace PublicTool {
    class FileMisc{
    public:
        //文件存在
        //param:strFilePath:文件路径
        //return:存在,true;不存在,false
       static  bool IsFileExist(string strFilePath);

        //简单下载接口
       static bool EasyFileDownload(string strFileUrl);

        //简单上传接口
        static bool EasyFileUpload(string strFileUrl);

        //下载文件
        //
        static int FileDownload(string strFileUrl,string& strRespson,bool bBlocking=true);

        //上传文件
        //
        static int FileUpload(string strFileUrl,string& strRespson,bool bBlocking=true,bool bNeedExternUrl=false);

        //读文件
        static bool ReadFileContent(string strFileName,string &strFileContent);
        //写文件
        static bool WriteFileContent(string strFileName,string strFileContent);
        //替换掉字符串中的字符
        static int StrReplace(string &str, const string &src, const string &dest);
        //创建多级目录
        static void MKDirs(const char *muldir);
        //copy 文件
        static bool CopyFile(string strSrcFilePath,string strDestFilePath);
    };
}





#endif









