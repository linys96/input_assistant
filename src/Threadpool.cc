#include "../include/Threadpool.hpp"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;
namespace lys
{

Threadpool::Threadpool(size_t threadNum, size_t queSize)
:_threadNum(threadNum)
,_queSize(queSize)
,_taskQue(queSize)
,_isExit(false)
{
    _threads.reserve(threadNum);
}

Threadpool::~Threadpool()
{
    if(!_isExit){
        stop();
    }
}

void Threadpool::addTask(Task &&task)
{
    _taskQue.push(task);    
}

void Threadpool::start()
{
    for(size_t idx=0; idx != _threadNum; ++idx){
        std::unique_ptr<Thread> upThread(
        new Thread(std::bind(&Threadpool::threadFunc,this),std::to_string(idx)));

        _threads.push_back(std::move(upThread)); 
    }
    for (auto &pthread : _threads){
        pthread->start();
    }

}

void Threadpool::stop()
{
    if(!_isExit){
        while(!_taskQue.empty()){
            sleep(1);
        }
    }
    _isExit=true;
    _taskQue.wakeup();

    for(auto &pthread: _threads){
        pthread->join();
    }

}

Task Threadpool::getTask()
{
    return _taskQue.pop();
}

void Threadpool::threadFunc()
{
    while(!_isExit){
        Task task = getTask();
        if(task){
            task();
        }
    }
}

}//end of namespace lys
