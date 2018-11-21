//一个简单的日志库
//支持输出时间、行号、文件名、函数名并支持多线程
//	支持按天自动分割日志


#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <stdarg.h>

#ifdef __WIN32__
#include <windows.h>
#else
#include <pthread.h>
#endif

//定义日志备份时间
const long long DAY_SECONDS = 24 * 60 * 60;
//定义日志内打印的最大函数就需要进行备份
const int MAX_LINE_COUNT=20000;//2w

class Log
{
public:
	typedef enum __tagLogLevel_E
	{
		LOG_ALL_E = 0,
		LOG_ERROR_E,
		LOG_INFO_E,
		LOG_DEBUG_E,	
	}LogLevel_E;

private:

	long long m_llLineCount;

	std::string m_strName;//日志名字
	std::string m_strPath;//日志路径 
	int m_nLevel;//打印日志的级别
	
	time_t m_llNextTime;  //下一次新建日志的时间
	FILE* m_pFile;

	bool m_bUsingMultThread;
#ifdef __WIN32__
	CRITICAL_SECTION m_pCrit;
#else
    pthread_mutex_t m_pMutex;
#endif

private:
	Log(const std::string& name, const std::string& path = ".", bool mult_thread = false, int level = 0);
	Log(const Log&);
	Log operator=(const Log&);
	~Log();
	
public:
	static Log& create(const std::string& n, const std::string& p = ".", bool mult_thread = false, int l = 0);

	bool need_open_new_file();
	int open_new_file();
	void release_file();
	void update_next_time();
	int print(int level, const char* file_name, int line, 
			const char* func, const char* fmt, ...);

	void lock();
	void unlock();
private:
	void get_year_month_day(std::string& res);
	void get_hour_min_sec(std::string& res);
	void get_hour_min_sec_ex(std::string& res);	

};

class LogContainer
{
private:	
	static Log* log;
	
public:
	static Log* get();
	static Log* create(const char* name, const char* conf = NULL);
};


#define INIT_LOG(name) \
	do \
	{ \
		if(LogContainer::get() == NULL) \
		{ \
			LogContainer::create(name, "log.conf"); \
		} \
	} while(0)


#define INIT_LOG2(name, conf) \
	do \
	{ \
		if(LogContainer::get() == NULL) \
		{ \
			LogContainer::create(name, conf); \
		} \
	} while(0)


#define DEBUG_LOG(...) \
	do \
	{ \
        LogContainer::get()->print(Log::LOG_DEBUG_E, __FILE__, __LINE__, __FUNCTION__ ,__VA_ARGS__); \
	} while(0)


#define INFO_LOG(...) \
	do \
	{ \
        LogContainer::get()->print(Log::LOG_INFO_E, __FILE__, __LINE__, __FUNCTION__ ,__VA_ARGS__); \
	} while(0)


#define ERROR_LOG(...) \
	do \
	{ \
        LogContainer::get()->print(Log::LOG_ERROR_E, __FILE__, __LINE__, __FUNCTION__ ,__VA_ARGS__); \
	} while(0)

#endif
	
