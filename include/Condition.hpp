#ifndef __LYS_CONDITION_H__
#define __LYS_CONDITION_H__ 

#include "Noncopyable.hpp"
 
#include <pthread.h>	

#include <iostream>		

using std::cout;
using std::endl;

namespace lys
{
class MutexLock;
class Condition
:public Noncopyable
{
public:
	explicit
	Condition(MutexLock & mutex);
	~Condition();

	void wait();
	void notify();
	void notifyAll();
		
private:
	pthread_cond_t _cond;
	MutexLock & _mutex;
};

}//end of namespace lys
 
#endif
