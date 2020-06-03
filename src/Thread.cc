 
#include "../include/Thread.hpp"
 
using std::string;

namespace lys
{
namespace  current_thread
{
    __thread const char* threadName = "thread";
}

class ThreadData
{
public:
    string _name;
    ThreadCallback _cb;
    
    ThreadData(const string &name, const ThreadCallback &cb)
    : _name(name)
    , _cb(cb)
    {}
    
    void runInThread()
    {
        current_thread::threadName = (_name == string() ? "thread": _name.c_str() );
        if(_cb) {
            _cb();
        }
    }
};
	
void Thread::start() //开启线程 ==> pthread_create
{
    ThreadData *pData = new ThreadData(_name, _cb);
	pthread_create(&_pthid, nullptr, threadFunc, pData);
	_isRunning = true;
}

void Thread::join()  //回收线程
{
	if(_isRunning) {
		pthread_join(_pthid, nullptr);
		_isRunning = false;
	}
}

void * Thread::threadFunc(void * arg)
{
	ThreadData * pData= static_cast<ThreadData*>(arg);

	if(pData) {
        pData->runInThread();
	}
    pthread_exit(nullptr);
}

}//end of namespace lys
