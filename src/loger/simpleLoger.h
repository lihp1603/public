/*
*本文件主要是用于打印loger日志
*author:lihaiping1603@aliyun.com
*date:2017-06-21
*/

#ifndef __SIMPLE_LOGER_H_
#define __SIMPLE_LOGER_H_

#pragma once
#include <stdio.h>
#include <stdlib.h>


#ifdef __WIN32__
#define LogPrint(fmt, ...)  printf("\n %s:"#fmt,__FUNCTION__,__VA_ARGS__)
#define DLogPrint(fmt, ...)  printf("\n %s:"#fmt,__FUNCTION__,__VA_ARGS__)
#define ELogPrint(fmt, ...)  printf("\n %s:"#fmt,__FUNCTION__,__VA_ARGS__)


#else//linux

//使用非日志文件方式
//
#define LogPrint(fmt, args...)   printf("\n %s:%s:%d:"#fmt,__FILE__,__FUNCTION__,__LINE__, ##args)
#define DLogPrint(fmt, args...)  printf("\n %s:%s:%d:debug:"#fmt,__FILE__,__FUNCTION__,__LINE__, ##args)
#define WLogPrint(fmt, args...)  printf("\n %s:%s:%d:warning:"#fmt,__FILE__,__FUNCTION__,__LINE__, ##args)
#define ELogPrint(fmt, args...)  printf("\n %s:%s:%d:error:"#fmt,__FILE__,__FUNCTION__,__LINE__, ##args)

#endif





#endif


