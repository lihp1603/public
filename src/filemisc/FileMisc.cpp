/************************************************************************/
/* 这里的实现功能主要为检查本地路径下的文件是否存在,不存在的时候，需要去ucloud获取,以及上传用户文件
*author:lihaiping1603@aliyun.com
*date:2017-08-31
*/
/************************************************************************/



#include "FileMisc.h"
#include "../loger/simpleLoger.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <fstream>
#include <streambuf>

#include<sys/stat.h>
#include<sys/types.h>

using namespace std;



namespace PublicTool {

    bool FileMisc::IsFileExist(string strFilePath)
    {
        if(access(strFilePath.c_str(),0)==0){//说明文件存在
            return true;
        }
        return false;
    }

    bool FileMisc::EasyFileDownload(string strFileUrl)
    {
        if(!IsFileExist(strFileUrl)){
            return false;
        }
        return true;

        bool ret=true;
        string strResponse;
        if(/*!IsFileExist(strFileUrl)&&*/FileDownload(strFileUrl,strResponse,true)!=0){
            LogPrint("error:file download %s,response:%s",strFileUrl.c_str(),strResponse.c_str());
            return false;
        }
        else{
            //对文件进行判断,判断他是否已经存在
            return IsFileExist(strFileUrl);
        }
        return ret;
    }

    bool FileMisc::EasyFileUpload(string strFileUrl)
    {
        if(!IsFileExist(strFileUrl)){
            return false;
        }
        return true;

        bool ret=true;
        string strResponse;
        if(!IsFileExist(strFileUrl)||FileUpload(strFileUrl,strResponse,true)!=0){
            LogPrint("error:file upload %s,response:%s",strFileUrl.c_str(),strResponse.c_str());
            return false;
        }
        return ret;
    }

    /*
     *
     * 调用方式：
     *  http:/host:port/prefix/filename?action=download
     *
     *  调用说明：
     *  1、表示下载filename文件
     *  2、下载不需要参数params
     *  3、下载成功，返回HTTP状态码为200，无响应内容；同时在/data1/storage_proxy/prefix目录下会有filename文件
     *  4、下载失败，返回HTTP状态码为非200
     *
     *  调用示例：
     *  http://127.0.0.1:1180/bucket_public_global/smart/annex/c.png?action=download
     *
     *  1、返回调用成功后，应存在文件：/data1/storage_proxy/bucket_public_global/smart/annex/c.png
     *  2、bucket_public_global目录下一级通常为项目名，如smart为一键创意项目名
     */
    int FileMisc::FileDownload(string strFileUrl, string &strRespson, bool bBlocking)
    {
        string strLoadPrefix="http://127.0.0.1:1180/";//前缀
        string strDownActionSuffix="?action=download";//下载后缀
        string strFixedDir="/data1/storage_proxy/";//固定目录
        size_t pos=strFileUrl.find(strFixedDir);

        string strDownloadUrl=strFileUrl.substr(pos!=string::npos?pos+strFixedDir.length():0);
        if(strDownloadUrl.empty()){
             LogPrint("error:download url is empty.");
            return -1;
        }
        CurlHttpClient HttpClient;
        string strError;
        int nError;
        string strLoadUrl=strLoadPrefix+strDownloadUrl+strDownActionSuffix;
        strRespson.clear();
        DLogPrint("download:strLoadUrl:%s",strLoadUrl.c_str());
        bool bRet=HttpClient.CurlHttpGet(strLoadUrl,strRespson,strError,nError);
        if(!bRet){//发送failed
            LogPrint("download file curl get:%s,error:%s,%d",strLoadUrl.c_str(),strError.c_str(),nError);
            return -1;
        }
//        if(bBlocking){//如果使用阻塞方式下载
//            while(strRespson.empty()){
//                usleep(50*1000);//50ms
//            }
//            return 0;
//        }
//        else{//如果使用非阻塞方式下载
//            return 0;
//        }
        return 0;
    }

   /*
    *   调用方式：
    *   http:/host:port/prefix/filename?action=upload&params=xx
    *
    *   调用说明：
    *       1、表示上传filename文件，该文件应该已存在
    *       2、上传成功，返回HTTP状态码为200，响应内容根据params而定
    *       3、上传失败，返回HTTP状态码为非200
    *
    *   调用示例：
    *       http://127.0.0.1:1180/bucket_public_global/smart/annex/c.png?action=upload&params={"return_external_url":1 ,"expires":100,"protocol":"https"}
    *
    *       1、表示上传/data1/storage_proxy/bucket_public_global/smart/annex/c.png
    *       2、return_external_url为1表示需要返回外部访问链接（0表示不需要）
    *       3、expires为100，表示100秒返回的链接将过期（0表示返回的链接永不过期）
    *       4、protocol为https，表示返回返回链接使用的协议为https（还可取值为http，目前不支持https，所以该参数将被忽略，返回的均为http）
    *       5、响应体示例（格式为json）：
    *           {
    *               "code":0,
    *               "url":"http://kz-pri-gz-1.kuaizitech.net/annex/c.png?UCloudPublicKey=Y4l+khTQUNxKIkmdvuL8zkk1g3lLCi4cqqnT/iaoxgivI//e&Signature=UOFTTzFK8yL9vq9iqyEX4eBNXho%3D"
    *           }
    */
    int FileMisc::FileUpload(string strFileUrl, string &strRespson, bool bBlocking,bool bNeedExternUrl)
    {
        string strLoadPrefix="http://127.0.0.1:1180/";//前缀
        string strUpActionSuffix="?action=upload";//上传后缀
        //生成外部链接上传的后缀
        string strUpActionSuffixEx="?action=upload&params={\"return_external_url\":1 ,\"expires\":100,\"protocol\":\"https\"}";
        string strFixedDir="/data1/storage_proxy/";//固定目录
        size_t pos=strFileUrl.find(strFixedDir);

        string strUploadUrl=strFileUrl.substr(pos!=string::npos?pos+strFixedDir.length():0);
        if(strUploadUrl.empty()){
            LogPrint("error:upload url is empty.");
            return -1;
        }
        CurlHttpClient HttpClient;
        string strError;
        int nError;
        string strLoadUrl;
        if(!bNeedExternUrl)//不需要外部链接
           strLoadUrl=strLoadPrefix+strUploadUrl+strUpActionSuffix;
        else//需要生成外部链接
           strLoadUrl=strLoadPrefix+strUploadUrl+strUpActionSuffixEx;

        strRespson.clear();
        //bool bRet=HttpClient.CurlHttpGet(strLoadUrl,strRespson,strError,nError);
        bool bRet=HttpClient.CurlHttpGetEx(strLoadUrl,strRespson,strError,nError);
        if(!bRet){//发送failed
            LogPrint("upload file failed,curl get:%s,error:%s,%d",strLoadUrl.c_str(),strError.c_str(),nError);
            return -1;
        }

        return 0;
    }

    bool FileMisc::ReadFileContent(string strFileName,string &strFileContent){

        std::ifstream tfile(strFileName.c_str());
        if (!tfile)//ifstream重载了！操作符
        {//打开文件失败
            return false;
        }

        std::string strTmpContent((std::istreambuf_iterator<char>(tfile)), std::istreambuf_iterator<char>());
        strFileContent=strTmpContent;
        tfile.close();

        return true;
    }

    bool FileMisc::WriteFileContent(string strFileName,string strFileContent){

        std::ofstream tfile(strFileName.c_str());
        if (!tfile)//ofstream重载了！操作符
        {//打开文件失败
            return false;
        }

        tfile<<strFileContent<<endl;
        tfile.close();

        return true;
    }

    /*
    *string 并没有提供这样的函数，所以我们自己来实现。
    *由于给定字符串可能出现多次，所以需要用到 find() 成员函数的第二个参数，
    *每次查找之后，从找到位置往后继续搜索。直接看代码（这个函数返回替换的次数，如果返回值是 0 说明没有替换）
    */
    int FileMisc::StrReplace(string &str, const string &src, const string &dest)
    {
        int counter = 0;
        string::size_type pos = 0;
        while ((pos = str.find(src, pos)) != string::npos) {
            str.replace(pos, src.size(), dest);
            ++counter;
            pos += dest.size();
        }
        return counter;
    }

    void FileMisc::MKDirs(const char *muldir)
    {
        int i,len;
        char str[512];
        strncpy(str, muldir, 512);
        len=strlen(str);
        for( i=0; i<len; i++ )
        {
            if( str[i]=='/' )
            {
                str[i] = '\0';
                if( access(str,0)!=0 )
                {
                    mkdir( str, 0777 );
                }
                str[i]='/';
            }
        }
        if( len>0 && access(str,0)!=0 )
        {
            mkdir( str, 0777 );
        }
        return;
    }

    bool FileMisc::CopyFile(string strSrcFilePath, string strDestFilePath)
    {
        //先创建一个目录，防止拷贝的时候，目录不存在拷贝失败
        //FileMisc::MKDirs(strDestFilePath.c_str());
        //文件拷贝
        std::ifstream in;
        in.open(strSrcFilePath);
        if(!in)
        {
            LogPrint("open src file : %s failed",strSrcFilePath.c_str());
            return false;
        }
        std::ofstream out;
        out.open(strDestFilePath);
        if(!out)
        {
//            std::cout << "create new file : " << strDestFilePath << " failed" << std::endl;
            LogPrint("create new file :%s failed",strDestFilePath.c_str());
            in.close();
            return false;
        }

        out << in.rdbuf();

        out.close();
        in.close();
        return true;
    }

}



