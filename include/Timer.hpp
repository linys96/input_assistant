#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <functional>

namespace lys
{

class Timer
{
public:

    using TimerCallback= std::function<void()>;

    Timer(TimerCallback &&,int initialTime,int periodicTime);
    ~Timer();

    void start();
    void stop();
private:
    int createTimerfd();
    void setTimerfd(int initialTime,int periodicTime);
    void handleRead();
private:
    int _tfd;
    TimerCallback _cb;
    int _initialTime;//第一次到期时间
    int _periodicTime;//周期的到期时间
    bool _isStarted;
};

}

#endif
