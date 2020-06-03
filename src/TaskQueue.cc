 
#include "../include/TaskQueue.hpp"

#include <iostream>
using std::cout;
using std::endl;

namespace lys
{
bool TaskQueue::empty()
{
	return _que.empty();
}

bool TaskQueue::full() 
{
	return _que.size() == _queSize;
}

//生产者线程会调用的方法
void TaskQueue::push(ElemType data)
{
	//队列之中是否还有位置存放数据
	MutexLockGuard protec(_mutex);
	while(full()) {
		_cond.wait();//阻塞
	}

	//队列之中还有位置
	_que.push(data);

	//通知消费者线程取数据
	_cond.notify();
}

//消费者线程需要做的事情
ElemType TaskQueue::pop()
{
	//队列之中是否有数据
	MutexLockGuard protec(_mutex);
	while(_flag&&empty()) {
		_cond.wait();//阻塞
	}
    if(_flag){
        ElemType data = _que.front();
        _que.pop();

        //通知生产者线程放入数据
        _cond.notify();
        return data;
    }else {
        return nullptr;
    }
}

void TaskQueue::wakeup()
{
    if(_flag){
        _flag=false;
    }
    _cond.notifyAll();
}
}//end of namespace lys
