/*
 * 本文件主要用于记录系统运行过错中，出现的一些重要的错误信息，用于程序获取
*/

#include "cerorr.h"
#include "../cosmutex.h"

namespace PublicTool{

    //在外部进行实例化
    CErorr* CErorr::m_pInstatce=new CErorr();

    CErorr::CErorr()
        :m_nLastErrCode(0)
    {

    }


    CErorr::~CErorr()
    {

    }

    CErorr *CErorr::GetInstatce()
    {
        return m_pInstatce;
    }

    void CErorr::DestroyInstatce()
    {
        if(m_pInstatce){
            delete m_pInstatce;
            m_pInstatce=NULL;
        }
    }

    int CErorr::GetLastErrMsg(string &strErrMsg)
    {
        COSMutexLocker autoLocker(&m_mutexLocker);
        strErrMsg=m_strLastErrMsg;
        return m_nLastErrCode;
    }

    void CErorr::SetLastErrMsg(int nErrCode, const char *szErrMsg)
    {
        if(szErrMsg!=NULL){
             COSMutexLocker autoLocker(&m_mutexLocker);
             m_strLastErrMsg.assign(szErrMsg);
             m_nLastErrCode=nErrCode;
        }
    }



}
