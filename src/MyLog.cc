#include "../include/MyLog.hpp"
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/Priority.hh>
#include <iostream>

using std::cout;
using std::endl;
using namespace log4cpp;

MyLog* MyLog::_pLog=nullptr;
//获取log指针
MyLog & MyLog::getInstance()
{
    if(nullptr==_pLog)
    {
        _pLog=new MyLog();
    }
    return *_pLog;
}
//销毁
void MyLog::destroy()
{
    if(_pLog)
    {
        _pLog->_categoryRef.info("MyLog destroy!");
        delete _pLog;
        _pLog=nullptr;
    }
}
//析构函数
MyLog::~MyLog()
{
    _pLog->_categoryRef.shutdown();
}
//构造函数
MyLog::MyLog()
:_categoryRef(Category::getRoot())
{
    //自定义输出格式
    PatternLayout *pPatternLayout1=new PatternLayout();
    pPatternLayout1->setConversionPattern("%d: [%p] %c %x: %m%n");
    PatternLayout *pPatternLayout2=new PatternLayout();
    pPatternLayout2->setConversionPattern("%d: [%p] %c %x: %m%n");
    //输出到屏幕
    OstreamAppender *pOstreamAppender=new OstreamAppender("osAppender",&cout);
    pOstreamAppender->setLayout(pPatternLayout1);
    //输出到文件，文件名：MyLog.log，回滚文件大小1M，备份文件数目为2
    RollingFileAppender *pRollingFileAppender=new RollingFileAppender("rollingFileAppender",
                                                                      "../log/MyLog.log",1<<20,2);
    pRollingFileAppender->setLayout(pPatternLayout2);

    _categoryRef.setPriority(log4cpp::Priority::DEBUG);
    _categoryRef.addAppender(pOstreamAppender);
    _categoryRef.addAppender(pRollingFileAppender);
    _categoryRef.info("MyLog create!");
}
//设置优先级
void MyLog::setPriority(::Priority priority)
{
    switch(priority)
    {
        case(ERROR):
            _categoryRef.setPriority(log4cpp::Priority::ERROR);
            break;
        case(WARN):
            _categoryRef.setPriority(log4cpp::Priority::WARN);
            break;
        case(INFO):
            _categoryRef.setPriority(log4cpp::Priority::INFO);
            break;
        case(DEBUG):
        default:
            _categoryRef.setPriority(log4cpp::Priority::DEBUG);
            break;
    }
}

void MyLog::error(const char *msg)
{
    _categoryRef.error(msg);
}
void MyLog::warn(const char *msg)
{
    _categoryRef.warn(msg);
}
void MyLog::info(const char *msg)
{
    _categoryRef.info(msg);
}
void MyLog::debug(const char *msg)
{
    _categoryRef.debug(msg);
}
