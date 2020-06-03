#include "../include/EventLoop.hpp"
#include "../include/TcpConnection.hpp"
#include "../include/Acceptor.hpp"
#include <unistd.h>
#include <sys/eventfd.h>
#include <iostream>

using std::cout;
using std::endl;

namespace lys
{

EventLoop::EventLoop(Acceptor &acceptor)
:_efd(createEpollFd())
,_eventfd(createEventfd())
,_acceptor(acceptor)
,_events(1024)
,_isLooping(false)
{
    addEpollFdRead(_acceptor.fd());
    addEpollFdRead(_eventfd);
}

void EventLoop::loop()
{
    if(!_isLooping) {
        _isLooping = true;
    }
    while(_isLooping) {
       waitEpollFd(); 
    }
}

void EventLoop::unloop()
{
    if(_isLooping){
        _isLooping = false;
    }
}

void EventLoop::runInLoop(Functor && cb)
{
    {
        MutexLockGuard protec(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    wakeup();
}

void EventLoop::waitEpollFd()
{
    int nready;
    do{
        nready = epoll_wait(_efd, &_events[0], _events.size(), 5000);
    }while(-1 == nready && errno == EINTR); 
    if(-1 == nready) {
        perror("epoll_wait");
        return ;
    }else if(0 == nready){
        cout << ">> epoll_wait timeout!" << endl;
    }else{
        if(nready == _events.size()) {
            _events.resize(2 * nready);
        }
        
        for(int idx = 0; idx != nready; ++idx) {
            int fd = _events[idx].data.fd;
            if(fd ==  _acceptor.fd()) {
                //新连接
                if(_events[idx].events & EPOLLIN){
                    handleNewConnection();
                }
            }else if(fd == _eventfd){
                if(_events[idx].events & EPOLLIN){
                    handleRead();
                    doPendingFunctors();
                }
            }else{
                if(_events[idx].events & EPOLLIN){
                    handleMessage(fd);
                }
            }
        }
    } 
} 
void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    addEpollFdRead(peerfd);
    TcpConnectionPtr conn(new TcpConnection(peerfd,this));
    conn->setConnectionCallback(_onConnection);
    conn->setMessageCallback(_onMessage);
    conn->setCloseCallback(_onClose);

    _conns.insert(std::make_pair(peerfd,conn));

    conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    auto iter = _conns.find(fd);
    if(iter != _conns.end()){
        bool isClosed = isConnectionClosed(fd);
        if(!isClosed){
            iter->second->handleMessageCallback();
            /* printf("---exit handleMessage---\n"); */
        }else{
            delEpollFdRead(fd);
            iter->second->handleCloseCallback();
            _conns.erase(iter);
        }
    }
}

int EventLoop::createEpollFd()
{
   int ret = ::epoll_create1(0); 
   if(-1 == ret) {
       perror("epoll_create1");
   }
   return ret;
}


void EventLoop::addEpollFdRead(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &evt);
    if(-1 == ret) {
        perror("epoll_ctl_add");
    }
}

void EventLoop::delEpollFdRead(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    int ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &evt);
    if(-1 == ret) {
        perror("epoll_ctl_del");
    }
}

bool EventLoop::isConnectionClosed(int fd)
{
    int ret;

    do{
        char buff[64];
        ret = recv(fd, buff, sizeof(buff), MSG_PEEK);
    }while(-1 == ret && errno == EINTR);

    return  (ret==0);
}

int EventLoop::createEventfd()
{
    int fd = ::eventfd(1,0);
    if (-1 == fd){
        perror("eventfd");
    }
    return fd;
}

void EventLoop::handleRead()
{
    uint64_t howmany;
    int ret = ::read(_eventfd, &howmany, sizeof(howmany));
    if(-1 == ret){
        perror("read");
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = ::write(_eventfd, &one, sizeof(one));
    if(-1 == ret){
        perror("write");
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> tmp;

    {
        MutexLockGuard protec(_mutex);
        tmp.swap(_pendingFunctors);
    }

    for(auto &f: tmp) {
        f();
    }
}

}//end of namespace lys
