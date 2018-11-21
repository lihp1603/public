/************************************************************************/
/*
function:   封装了对redis的使用，使用了xRedis这个client的api
author:     lihaiping1603@aliyun.com
date:		2017-09-21
from:		https://github.com/0xsky/xredis
*/
/************************************************************************/

#ifndef __CREDIS_CLIENT_WRAP_H_
#define __CREDIS_CLIENT_WRAP_H_
#include <string>
//#include "../include/xredis/xRedisClient.h"
#include <xredis/xRedisClient.h>

#include "../loger/simpleLoger.h"

//使用xredis 做client

using namespace std;


namespace PublicTool {

    #define CACHE_TYPE_1  1
    #define CACHE_TYPE_2  2

    //连接函数
    bool RedisConnnect(xRedisClient& xClient,const char *pszHost,int nPort,const char *pszPwd,bool bNeedInit);
    //获取redis报错信息
    bool IsRedisDisconnect(string strErr);

    //string
    /*inline*/ bool RedisSet(xRedisClient &xClient,const string& strKey, const string& strValue,int cache_type);
    /*inline*/ bool RedisGet(xRedisClient &xClient,const string& strKey, string& strValue,int cache_type);
    /*inline*/ bool RedisExists(xRedisClient &xClient,const string& strKey,int cache_type);
    /*inline*/ bool RedisDel(xRedisClient &xClient,const string& strKey,int cache_type);

    //list 列表
    /* LINDEX       */ /*inline*/ bool  RedisLIndex( xRedisClient &xClient,    const string& strKey, const int64_t index, VALUE& value,int cache_type);
    /* LINSERT      */ /*inline*/ bool  RedisLInsert( xRedisClient &xClient,  const string& strKey, const LMODEL mod, const string& pivot, const string& value, int64_t& retval,int cache_type);
    /* LLEN         */ /*inline*/ bool  RedisLLen( xRedisClient &xClient,     const string& strKey, int64_t& len,int cache_type);
    /* LPOP         */ /*inline*/ bool  RedisLPop( xRedisClient &xClient,     const string& strKey, string& value,int cache_type,string& strErr);
    /* LPUSH        */ /*inline*/ bool  RedisLPush( xRedisClient &xClient,    const string& strKey, const VALUES& vValue, int64_t& length,int cache_type);
//    /* LPUSHX       */ /*inline*/ bool  RedisLPushx( xRedisClient &xClient,   const string& strKey, const string& value, int64_t& length,int cache_type);
    /* LRANGE       */ /*inline*/ bool  RedisLRange( xRedisClient &xClient,   const string& strKey, const int64_t start, const int64_t end, ArrayReply& array,int cache_type);
    /* LREM         */ /*inline*/ bool  RedisLRem( xRedisClient &xClient,     const string& strKey,  const int count, const string& value, int64_t num,int cache_type);
    /* LSET         */ /*inline*/ bool  RedisLSet( xRedisClient &xClient,     const string& strKey,  const int index, const string& value,int cache_type);
    /* LTRIM        */ /*inline*/ bool  RedisLTrim( xRedisClient &xClient,    const string& strKey,  const int start, const int end,int cache_type);
    /* RPOP         */ /*inline*/ bool  RedisRPop( xRedisClient &xClient,     const string& strKey, string& value,int cache_type);
    /* RPOPLPUSH    */ /*inline*/ bool  RedisRPopLPush( xRedisClient &xClient,const string& strKey_src, const string& strKey_dest, string& value,int cache_type);
    /* RPUSH        */ /*inline*/ bool  RedisRPush( xRedisClient &xClient,    const string& strKey, const VALUES& vValue, int64_t& length,int cache_type);
    /* RPUSHX       */ /*inline*/ bool  RedisRPushx( xRedisClient &xClient,   const string& strKey, const string& value, int64_t& length,int cache_type);



}



#endif

