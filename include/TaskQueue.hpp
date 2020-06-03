#ifndef __LYS_TASKQUEUE_H__
#define __LYS_TASKQUEUE_H__

#include "MutexLock.hpp"
#include "Condition.hpp"

#include <iostream>
#include <queue>
#include <functional>

using std::cout;
using std::endl;
using std::queue;

namespace lys
{

using ElemType=std::function<void()>;
class TaskQueue
{
public:
	TaskQueue(size_t size = 10)
	: _queSize(size)
	, _mutex()
	, _cond(_mutex)
    , _flag(true)
	{}

	bool empty();
	bool full();
	void push(ElemType data);
	ElemType pop();
    void wakeup();

private:
	queue<ElemType> _que;
	size_t _queSize;
	MutexLock _mutex;
	Condition _cond;
    bool _flag;
};

}//end of namespace lys

#endif
