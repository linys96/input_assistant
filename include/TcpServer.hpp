#ifndef __LYS_TCPSERVER_HPP
#define __LYS_TCPSERVER_HPP

#include "EventLoop.hpp"
#include "Acceptor.hpp"
#include "TcpConnection.hpp"
#include <string>

namespace lys
{
using TcpConnectionCallback= std::function<void(const TcpConnectionPtr&)>;
class TcpServer
{
public:
    TcpServer(std::string ip, unsigned short port)
    :_accepor(ip, port)
    ,_loop(_accepor)
    {
    }
    ~TcpServer() {}
    
    void setConnectionCallback(TcpConnectionCallback && cb);
    void setMessageCallback(TcpConnectionCallback && cb);
    void setCloseCallback(TcpConnectionCallback && cb);
    void start();
private:
    Acceptor _accepor;
    EventLoop _loop;
};

}//end of namespace lys

#endif
