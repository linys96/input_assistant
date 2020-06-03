#include "../include/TcpServer.hpp"

namespace lys
{

void TcpServer::setConnectionCallback(TcpConnectionCallback && cb)
{
    _loop.setConnectionCallback(std::move(cb));
}

void TcpServer::setMessageCallback(TcpConnectionCallback && cb)
{
    _loop.setMessageCallback(std::move(cb));
}

void TcpServer::setCloseCallback(TcpConnectionCallback && cb)
{
    _loop.setCloseCallback(cb);
}

void TcpServer::start()
{
    _accepor.ready();
    _loop.loop();
}

}//end of namespace lys
