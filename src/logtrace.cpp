#include "logtrace.h"

#include "includes/glog/logging.h"

void InitLogTrace(const char *argv0, const char *logDir, int logServerity)
{
    google::InitGoogleLogging(argv0);
    int setLogServerity=google::INFO;
    if(logServerity>=google::INFO&&logServerity<=google::FATAL){
       setLogServerity=logServerity;
    }
    google::SetLogDestination(setLogServerity,logDir);
}

void LogTrace(int logServerity,const char *format,...){
    char buffer[1024];

    va_list varArgs;
    va_start( varArgs, format );
    vsnprintf( buffer, sizeof(buffer), format, varArgs);
    va_end( varArgs );
    switch (logServerity) {
    case (int)LogTrace_INFO:
        LOG(INFO)<<buffer;
        break;
    case (int)LogTrace_WARNING:
        LOG(WARNING)<<buffer;
        break;
    case (int)LogTrace_ERROR:
        LOG(ERROR)<<buffer;
        break;
    default:
        LOG(FATAL)<<buffer;
        break;
    }
}

#ifdef __WIN32__
void LogTraceD(const char *format,...)
{
     char buffer[1024];

     va_list varArgs;
     va_start( varArgs, format );
     vsnprintf( buffer, sizeof(buffer), format, varArgs);
     va_end( varArgs );

     DLOG(INFO)<<buffer;
}

void LogTraceI(const char *format,...)
{
    char buffer[1024];

    va_list varArgs;
    va_start( varArgs, format );
    vsnprintf( buffer, sizeof(buffer), format, varArgs);
    va_end( varArgs );

    LOG(INFO)<<buffer;
}

void LogTraceW(const char *format,...)
{
    char buffer[1024];

    va_list varArgs;
    va_start( varArgs, format );
    vsnprintf( buffer, sizeof(buffer), format, varArgs);
    va_end( varArgs );

    LOG(WARNING)<<buffer;
}

void LogTraceE(const char *format,...)
{
    char buffer[1024];

    va_list varArgs;
    va_start( varArgs, format );
    vsnprintf( buffer, sizeof(buffer), format, varArgs);
    va_end( varArgs );
    LOG(ERROR)<<buffer;
}

#endif



