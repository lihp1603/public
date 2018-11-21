/************************************************************************/
/*
function:   封装了对redis的使用，使用了xRedis这个client的api
author:     lihaiping1603@aliyun.com
date:		2017-09-21
from:		https://github.com/0xsky/xredis
*/
/************************************************************************/


#include "CRedisClientWrap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include "../include/xredis/xRedisClient.h"
#include <xredis/xRedisClient.h>


using namespace std;

namespace PublicTool {
    //定义本地默认的redis
    RedisNode LoaclDefaultRedisList[1] = {
        {0, "127.0.0.1", 6379, "", 2, 5}
    };

    RedisNode RedisList1[1] = {
           {0, "dev-redis", 40000, "", 2, 5}
       };

    // AP Hash Function
    unsigned int APHash(const char *str) {
        unsigned int hash = 0;
        int i;
        for (i=0; *str; i++) {
            if ((i&  1) == 0) {
                hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
            } else {
                hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
            }
        }
        return (hash&  0x7FFFFFFF);
    }

    //连接redis
    bool RedisConnnect(xRedisClient& xClient,const char *pszHost,int nPort,const char *pszPwd,bool bNeedInit){
        if(bNeedInit){//针对重连操作,第一次连接等操作
            xClient.release();//先要释放连接池
            //再初始化
            if(false==xClient.Init(3)){
                LogPrint("redis client reinit err");
                return false;
            }
        }

        RedisNode tempRedisNodeList[1]={
            {0,pszHost,nPort,pszPwd,2,5}
        };
        return xClient.ConnectRedisCache(tempRedisNodeList,1,CACHE_TYPE_1);
    }
    bool IsRedisDisconnect(string strErr)
    {
        if(strErr=="redis connection be closed"){
            return true;
        }
       return false;
    }
    //string 操作
    /*inline*/ bool RedisSet(xRedisClient& xClient,const string& strKey, const string& strValue,int cache_type)
    {
      RedisDBIdx dbi(&xClient);
      bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
      if (bRet) {
          if (xClient.set(dbi, strKey, strValue)) {
              DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
              return true;
          } else {
              ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
          }
      }
      return false;
    }
    /*inline*/ bool RedisGet(xRedisClient& xClient,const string& strKey, string& strValue,int cache_type)
      {
          RedisDBIdx dbi(&xClient);
          bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
          if (bRet) {
              if (xClient.get(dbi, strKey, strValue)) {
                  DLogPrint("%s success data:%s \r\n", __PRETTY_FUNCTION__, strValue.c_str());
                  return true;
              } else {
                  ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
              }
          }
          return false;
      }

    /*inline*/ bool RedisExists(xRedisClient& xClient,const string& strKey,int cache_type)
    {
      RedisDBIdx dbi(&xClient);
      bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
      if (bRet) {
          if (xClient.exists(dbi, strKey)) {//存在
              DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
              return true;
          } else {//不存在
              ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
          }
      }
      return false;
    }

    /*inline*/ bool RedisDel(xRedisClient& xClient,const string& strKey,int cache_type)
    {
      RedisDBIdx dbi(&xClient);
      bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
      if (bRet) {
          if (xClient.del(dbi, strKey)) {
              DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
              return true;
          } else {
              ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
          }
      }
      return false;
    }



    //list操作
    bool RedisLIndex( xRedisClient& xClient, const string &strKey, const int64_t index, VALUE &value,int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.lindex(dbi, strKey,index,value)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

    bool RedisLInsert( xRedisClient& xClient, const string &strKey, const LMODEL mod, const string &pivot, const string &value, int64_t &retval,int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.linsert(dbi, strKey,mod,pivot,value,retval)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

    bool RedisLLen( xRedisClient& xClient, const string &strKey, int64_t &len, int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.llen(dbi, strKey,len)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

    bool RedisLPop( xRedisClient& xClient, const string &strKey, string &value, int cache_type,string& strErr)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.lpop(dbi, strKey,value)) {
//                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                if(NULL!=dbi.GetErrInfo()){
                    ELogPrint("redis lpop error [%s] \r\n", dbi.GetErrInfo());
                    strErr.assign((const char*)dbi.GetErrInfo());
                }
            }
        }
        return false;
    }

    bool RedisLPush( xRedisClient& xClient, const string &strKey, const VALUES &vValue, int64_t &length, int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.lpush(dbi, strKey,vValue,length)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

//    bool RedisLPushx( xRedisClient& xClient, const string &strKey, const string &value, int64_t &length, int cache_type)
//    {
//        RedisDBIdx dbi(&xClient);
//        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
//        if (bRet) {
//            if (xClient.lpushx(dbi, strKey,value,length)) {
//                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
//                return true;
//            } else {
//                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
//            }
//        }
//        return false;
//    }

    bool RedisLRange( xRedisClient& xClient, const string &strKey, const int64_t start, const int64_t end, ArrayReply &array, int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.lrange(dbi, strKey,start,end,array)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

    bool RedisLRem( xRedisClient& xClient, const string &strKey, const int count, const string &value, int64_t num, int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.lrem(dbi, strKey,count,value,num)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

    bool RedisLSet( xRedisClient& xClient, const string &strKey, const int index, const string &value, int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.lset(dbi, strKey,index,value)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

    bool RedisLTrim( xRedisClient& xClient, const string &strKey, const int start, const int end, int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.ltrim(dbi, strKey,start,end)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

    bool RedisRPop( xRedisClient& xClient, const string &strKey, string &value, int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.rpop(dbi,strKey,value)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

    bool RedisRPopLPush( xRedisClient& xClient, const string &strKey_src, const string &strKey_dest, string &value, int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey_src.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.rpoplpush(dbi, strKey_src,strKey_dest,value)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

    bool RedisRPush( xRedisClient& xClient, const string &strKey, const VALUES &vValue, int64_t &length, int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.rpush(dbi, strKey,vValue,length)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }

    bool RedisRPushx( xRedisClient& xClient, const string &strKey, const string &value, int64_t &length, int cache_type)
    {
        RedisDBIdx dbi(&xClient);
        bool bRet = dbi.CreateDBIndex(strKey.c_str(), APHash, cache_type);
        if (bRet) {
            if (xClient.rpushx(dbi, strKey,value,length)) {
                DLogPrint("%s success \r\n", __PRETTY_FUNCTION__);
                return true;
            } else {
                ELogPrint("%s error [%s] \r\n", __PRETTY_FUNCTION__, dbi.GetErrInfo());
            }
        }
        return false;
    }




}
