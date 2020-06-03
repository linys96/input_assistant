#ifndef __TIMERTHREAD_HPP__
#define __TIMERTHREAD_HPP__

#include "Timer.hpp"
#include "Thread.hpp"

namespace lys
{

class TimerThread
{
    using TimerCallback=std::function<void()>;
public:
    TimerThread(TimerCallback &&cb, int initalTime, int intervalTime)
    :_timer(std::move(cb), initalTime, intervalTime)
    ,_thread(std::bind(&Timer::start, &_timer))
    {}

    ~TimerThread() {}

    void start()
    {
        _thread.start();
    }

    void stop()
    {
        _timer.stop();
        _thread.join();
    }

private:
    Timer _timer;
    Thread _thread;

};

}//end of namespace lys

#endif
