#ifndef __LYS_EVENTLOOP_HPP__
#define __LYS_EVENTLOOP_HPP__

#include "MutexLock.hpp"
#include <sys/epoll.h>
#include <map>
#include <vector>
#include <memory>
#include <functional>

namespace lys
{
class Acceptor;
class TcpConnection;

    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using Functor = std::function<void()>;

class EventLoop
{
public:
    using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;

    EventLoop(Acceptor &acceptor);
    ~EventLoop() {}
    
    void loop();
    void unloop();
    void setConnectionCallback(const TcpConnectionCallback &cb)
    {
        _onConnection = cb; 
    }
    void setMessageCallback(const TcpConnectionCallback &cb)
    {
        _onMessage = cb;
    }
    void setCloseCallback(const TcpConnectionCallback &cb)
    {
        _onClose = cb;
    }
    void runInLoop(Functor && cb);
private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    int createEpollFd();

    void addEpollFdRead(int fd);
    void delEpollFdRead(int fd);
    bool isConnectionClosed(int fd);

    int createEventfd();
    void handleRead();
    void wakeup();
    void doPendingFunctors();

private:
    int _efd;
    int _eventfd;
    Acceptor &_acceptor;
    std::vector<struct epoll_event> _events;
    std::map<int,TcpConnectionPtr> _conns;
    bool _isLooping;

    std::vector<Functor> _pendingFunctors;
    MutexLock _mutex;

    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
};

}//end of namespace lys

#endif
