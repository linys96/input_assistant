#ifndef __MYLOG_H__
#define __MYLOG_H__
#include <log4cpp/Category.hh>
#include <string>
#include <iostream>

//日志优先级
enum Priority{
    ERROR,
    WARN,
    INFO,
    DEBUG
};
//单例模式封装log4cpp
class MyLog
{
public:
    static MyLog& getInstance();
    static void destroy();
    
    void setPriority(Priority priority);
    void warn(const char * msg);
    void error(const char * msg);
    void debug(const char * msg);
    void info(const char * msg);
private:
    MyLog() ;
    ~MyLog();

private:
    static MyLog *_pLog;
    log4cpp::Category &_categoryRef;
};
//在日志后加上文件名、函数名、行号
#define suffix(msg) std::string(msg).append(" ##").append(__FILE__).append(":")\
        .append(__func__).append(":").append(std::to_string(__LINE__))\
        .append("##").c_str()
//在主函数开头加上#define _LOG4CPP_就可以不用每次使用时写getInstance就可以使用
#ifdef _LOG4CPP_ 
MyLog &log = MyLog::getInstance();
#else
extern MyLog &log;
#endif

//用宏缩短并简化函数调用形式
#define logError(msg) log.error(suffix(msg))
#define logWarn(msg) log.warn(suffix(msg))
#define logInfo(msg) log.info(suffix(msg))
#define logDebug(msg) log.debug(suffix(msg))

//大写的Log可变参数
template <class... Args>
void LogInfo(const char * str, Args... args)
{
    char buf[1024]={0}; 
    sprintf(buf,str,args...);
    logInfo(buf);
}
template <class... Args>
void LogDebug(const char * str, Args... args)
{
    char buf[1024]={0}; 
    sprintf(buf,str,args...);
    logDebug(buf);
}
template <class... Args>
void LogError(const char * str, Args... args)
{
    char buf[1024]={0}; 
    sprintf(buf,str,args...);
    logError(buf);
}
template <class... Args>
void LogWarn(const char * str, Args... args)
{
    char buf[1024]={0}; 
    sprintf(buf,str,args...);
    logWarn(buf);
}
#endif
