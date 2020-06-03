#include "../include/Timer.hpp"
#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <sys/timerfd.h>

namespace lys
{

Timer::Timer(TimerCallback &&cb,int initialTime,int periodicTime)
:_tfd(createTimerfd())
,_cb(std::move(cb))
,_initialTime(initialTime)
,_periodicTime(periodicTime)
,_isStarted(false)
{}

Timer::~Timer()
{
    if(_isStarted){
        stop();
    }
}

/* 开启定时器 */
void Timer::start()
{
    if(!_isStarted) {
        _isStarted=true;
    }

    struct pollfd pfd;
    pfd.fd=_tfd;
    pfd.events=POLLIN;//监听可读，因为超时就会产生可读事件
    setTimerfd(_initialTime,_periodicTime);//设置timerfd

    while(_isStarted) {
        int nready;
        do{
            nready = ::poll(&pfd, 1, 5000);
        }while(nready == -1 && errno == EINTR);

        if(nready==-1) {
            perror("poll");
            return;
        } else if(nready==0) {
            printf(">> poll timeout\n");
        } else {
            if(pfd.revents&POLLIN) {
                /* 处理定时器 */
                handleRead();
                if(_cb) {
                    _cb();
                }
            }
        }
    }
}

/* 停止定时器 */
void Timer::stop()
{
    setTimerfd(0,0);//停了
    if(_isStarted==true) {
        _isStarted=false;
    }
}
int Timer::createTimerfd()//返回timerfd
{
    int fd= ::timerfd_create(CLOCK_REALTIME,0);
    if(fd<0) {
        perror("timerfd_create");
    }
    return fd;
}
void Timer::setTimerfd(int initialTime,int periodicTime)
{
    struct itimerspec value;
    value.it_value.tv_sec=initialTime;
    value.it_value.tv_nsec=0;
    value.it_interval.tv_sec=periodicTime;
    value.it_interval.tv_nsec=0;

    int ret=::timerfd_settime(_tfd, 0, &value, nullptr);
    if(ret<0) {
        perror("timerfd_settime");
    }
}

void Timer::handleRead()//处理定时器操作
{
    uint64_t howmany;//超时次数
    int ret=::read(_tfd,&howmany,sizeof(howmany));
    if(ret!=sizeof(howmany)) {
        perror("read");
    }
}

}//end of namespace lys
