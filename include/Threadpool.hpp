#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__

#include "Thread.hpp"
#include "TaskQueue.hpp"
#include "TimerThread.hpp"
#include "CacheManger.hpp"

#include <vector>
#include <memory>

namespace lys
{

using Task= std::function<void()>;
class Threadpool
{
public:
    Threadpool(size_t threadNum, size_t queSize);
    ~Threadpool();

    void addTask(Task &&);
    void start();
    void stop();
private:
    Task getTask();
    void threadFunc();

private:
    size_t _threadNum;
    size_t _queSize;
    std::vector<std::unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    bool _isExit;
};

}//end of namespace lys

#endif
