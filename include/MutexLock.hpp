#ifndef __LYS_MUTEXLOCK_H__
#define __LYS_MUTEXLOCK_H__
#include "Noncopyable.hpp"
#include <pthread.h>

#include <iostream>
using std::cout;
using std::endl;


namespace lys
{

class MutexLock
:public Noncopyable
{
public:
	MutexLock()
	: _isLocking(false)
	{
		if(0 != pthread_mutex_init(&_mutex, NULL)) {
			perror("pthread_mutex_init");
		}
	}

	~MutexLock()
	{
		if(0 != pthread_mutex_destroy(&_mutex)) {
			perror("pthread_mutex_init");
		}
	}

	void lock()
	{
		if(0 != pthread_mutex_lock(&_mutex)) {
			perror("pthread_mutex_lock");
			return ;
		}
		_isLocking = true;
	}

	void unlock()
	{
		if(0 != pthread_mutex_unlock(&_mutex)) {
			perror("pthread_mutex_unlock");
			return;
		}
		_isLocking = false;
	}

	pthread_mutex_t * getMutexLockPtr()
	{	return &_mutex;	}

	bool isLocking() const 
	{	return _isLocking;	}

private:
	pthread_mutex_t _mutex;
	bool _isLocking;
};

class MutexLockGuard
{
public:
	explicit
	MutexLockGuard(MutexLock & mutex)
	: _mutex(mutex)
	{	_mutex.lock();	}

	~MutexLockGuard()
	{	_mutex.unlock();	}
private:
	MutexLock & _mutex;
};

}//end of namespace lys
 
#endif
