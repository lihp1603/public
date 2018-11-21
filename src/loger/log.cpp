
#include "log.h"
#include <string.h> 

#ifdef __WIN32__
#include <windows.h>
#else
#include <pthread.h>
#endif

using namespace std;


Log& Log::create(const std::string& name, const std::string& path, bool mult_thread, int level)
{
	static Log log(name, path, mult_thread, level);
	return log;
}

Log::Log(const std::string& name, const std::string& path, bool mult_thread, int level): 
		m_strName(name), 
		m_strPath(path), 
		m_nLevel(level), 
		m_pFile(NULL), 
		m_bUsingMultThread(mult_thread),
		m_llLineCount(0)
{
	m_llNextTime = time(NULL);	
	m_llNextTime = (m_llNextTime / DAY_SECONDS + 1) * DAY_SECONDS;

	if(m_bUsingMultThread)
	{
#ifdef __WIN32__
		InitializeCriticalSection(&m_pCrit);
#else
        pthread_mutex_init(&m_pMutex, NULL);
#endif
	}
	//open_new_file();
}

bool Log::need_open_new_file()
{
	time_t now_time = time(NULL);
	
	return now_time >= m_llNextTime || m_llLineCount >= MAX_LINE_COUNT;
}

int Log::open_new_file()
{
	std::string full_path = m_strPath + "/" + m_strName;
	std::string day;
	std::string seconds;
	get_year_month_day(day);   
	get_hour_min_sec_ex(seconds);

//    full_path += "_" + day + seconds + ".log";
    full_path += "_" + day + "_" + seconds + ".log";
	m_pFile = fopen(full_path.c_str(), "a+");				
	if(m_pFile == NULL)
	{
		return -1;
	}
	
	return 0;
}

void Log::release_file()
{
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		m_llLineCount=0;
	}
}

Log::~Log()
{
	release_file();
	if(m_bUsingMultThread)
	{		
#ifdef __WIN32__
		DeleteCriticalSection(&m_pCrit);
#else
        pthread_mutex_destroy(&m_pMutex);
#endif
		m_bUsingMultThread = false;
	}
}

void Log::update_next_time()
{
	m_llNextTime += DAY_SECONDS;
}

int Log::print(int level, const char* file_name, int line, 
		const char* func, const char* fmt, ...)
{
	if(m_nLevel != Log::LOG_ALL_E && m_nLevel < level)
	{
		return 1;
	}

	lock();

	if(need_open_new_file())
	{
		release_file();
		if(open_new_file())
		{
			unlock();
			return -1;
		}
		update_next_time();
	}

	//lazy open
	if(m_pFile == NULL)
	{
		if(open_new_file())
		{
			unlock();
			return -2;
		}
	}

	std::string msg;
	switch(level)
	{
	case LOG_DEBUG_E:
		msg = "debug";
		break;

	case LOG_INFO_E:
		msg = "info";
		break;

	case LOG_ERROR_E:
		msg = "error";
		break;

	default:
		unlock();
		return -3;
	}
		
	fprintf(m_pFile, "[%s]", msg.c_str());

	std::string day, hour, tm;
	get_year_month_day(day);
	get_hour_min_sec(hour);
	tm = day + " " + hour; 
	fprintf(m_pFile, "[%s]", tm.c_str());
	fprintf(m_pFile, "[%s:%d]", file_name, line);
	fprintf(m_pFile, "[%s]: ", func);

	va_list ap;
	va_start(ap, fmt);
	vfprintf(m_pFile, fmt, ap);
	va_end(ap);

	fprintf(m_pFile, "\n");
	fflush(m_pFile);
	//记录打印的行数
	m_llLineCount++;
	unlock();
	return 0;
}

void Log::lock()
{
	if(m_bUsingMultThread)
	{
#ifdef __WIN32__
		EnterCriticalSection(&m_pCrit);
#else
		pthread_mutex_lock(&m_pMutex);
#endif
	}
}

void Log::unlock()
{
	if(m_bUsingMultThread)
	{
#ifdef __WIN32__
		LeaveCriticalSection(&m_pCrit);
#else
		pthread_mutex_unlock(&m_pMutex);
#endif
	}
}

void Log::get_year_month_day(std::string& res)
{
	time_t cur_time = time(NULL);
	struct tm * timeinfo = localtime(&cur_time);

	char buffer[] = {"yyyy-mm-dd"};
	strftime(buffer, sizeof(buffer), "%F", timeinfo);
	res = std::string(buffer);
}

void Log::get_hour_min_sec(std::string& res)
{
	time_t cur_time = time(NULL);
	struct tm * timeinfo = localtime(&cur_time);

	char buffer[] = {"hh:mm:ss"};
	strftime(buffer, sizeof(buffer), "%T", timeinfo);
	res = std::string(buffer);
}

void Log::get_hour_min_sec_ex(std::string& res)
{
	time_t cur_time = time(NULL);
	struct tm * timeinfo = localtime(&cur_time);

	char buffer[] = {"hh-mm-ss"};
	strftime(buffer, sizeof(buffer), "%T", timeinfo);
	res = std::string(buffer);
}


Log* LogContainer::log = NULL;

Log* LogContainer::get()
{
	return log;
}

Log* LogContainer::create(const char* name, const char* conf)
{
	string log_path = ".";
	int m_bUsingMultThread = 1; 
	int level = Log::LOG_ALL_E;
	
	FILE* pConfFile = fopen(conf, "r");
	if(pConfFile)
	{
		char path[256] = {0};
		fscanf(pConfFile, "log_path=%s\n", path); 
		if(strlen(path))
		{
			log_path = path;
		}
		fscanf(pConfFile, "m_bUsingMultThread=%d\n", &m_bUsingMultThread); 
		fscanf(pConfFile, "level=%d\n", &level);
		fclose(pConfFile); 
	}

	return log = &Log::create(name, log_path, m_bUsingMultThread, level); 
}
