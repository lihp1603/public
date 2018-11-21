/*
 * 本文件主要用于记录系统运行过错中，出现的一些重要的错误信息，用于程序获取
*/
#ifndef __CERORR_H_
#define __CERORR_H_

#include <string>
#include "../cosmutex.h"

using namespace std;

//产生错误码
#define ERR_X(X)  -((int)X)

namespace PublicTool{
    typedef enum {
        NO_ERR=0,//无错误
        UNKOWN_ERR=1,//未知错误
        PROJECT_PARSE_ERR=100,//工程解析出错
        PROJECT_MATCH_ERR,//工程和用户素材数量不匹配
        PROJECT_REPLACE_ERR,//工程素材替换出错
        PROJECT_SAVE_ERR,//保存工程文件出错
        DOWNLOAD_ERR,//下载文件出错
        UPLOAD_ERR,//上传文件出错
        INFO_LACK_ERR,//用户缺少必须的合成信息
        ZERO_CREATIVE_ERR,//生成不了创意
        TEMPLATE_QUERY_ERR,//数据库查询出错
        NOT_LOGO_TEMPALTE_ERR,//没有找到logo模板
        NOT_TEXT_TEMPALTE_ERR,//没有找到文字模板
        NOT_AUDIO_FILE_ERR,//没有找到音乐文件
        NOT_PLATTE_FILE_ERR,//没有找到调色视频文件
        NOT_PRODUCT_TEMPLATE_ERR,//没有找到产品的模板
        NO_TEMPLATE_TAG_ERR,//找不到合适的模板系列
    }SYS_ERR_E;

    class CErorr
    {
    public:

        //获取实例
        static CErorr *GetInstatce();
        //销毁实例
        static void DestroyInstatce();
        //获取最后一次的错误信息
        int GetLastErrMsg(string &strErrMsg);
        //设置最后一次的错误信息
        void SetLastErrMsg(int nErrCode,const char* szErrMsg);

        ~CErorr();
    private:
        //
        CErorr();
        CErorr(CErorr const&);            // copy ctor hidden
        CErorr& operator=(CErorr const&); // assign op. hidde
        //用于记录最后一次的错误信息
        string  m_strLastErrMsg;
        int m_nLastErrCode;
        COSMutex m_mutexLocker;

        static CErorr* m_pInstatce;
    };

}

#endif // CERORR_H
