#ifndef __LYS_THREAD_H__
#define __LYS_THREAD_H__

#include "Noncopyable.hpp"
#include <pthread.h>

#include <iostream>
#include <functional>
#include <string>

namespace lys
{

namespace current_thread
{
    //线程名字
    extern __thread const char * threadName;
}

    using ThreadCallback = std::function<void()>;

class Thread
:Noncopyable
{
public:
	Thread( ThreadCallback && cb, const std::string &name = std::string() )
	: _pthid(0)
    , _name(name)
	, _isRunning(false)
    , _cb(cb)
	{	
        /* std::cout << "Thread()" << std::endl; */	
    }
	virtual ~Thread()
    {
        /* std::cout << "~Thread()" << std::endl; */
        if(_isRunning) {
            pthread_detach(_pthid);
        }
    }


	void start(); //开启线程 ==> pthread_create
	void join();  //回收线程

private:
	static void * threadFunc(void * arg);//线程函数执行体

private:
	pthread_t _pthid;// 线程id
    std::string _name;// 线程名字 
	bool _isRunning;
    ThreadCallback _cb;
};

}//end of namespace lys
#endif
