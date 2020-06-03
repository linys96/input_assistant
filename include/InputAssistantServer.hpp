#ifndef __INPUTASSISTANTSERVER_HPP__
#define __INPUTASSISTANTSERVER_HPP__

#include "TcpServer.hpp"
#include "Threadpool.hpp"
#include "TimerThread.hpp"

#include <memory>
#include <string>

namespace lys
{
class Myconf;
class TcpConnection;

/* using TcpConnectionPtr = std::shared_ptr<const TcpConnection&>; */

class InputAssistantServer
{

public:
    /* InputAssistantServer(const std::string &confFileName); */
    InputAssistantServer(Myconf &conf);
    ~InputAssistantServer();
    
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &conn);
    void onClose(const TcpConnectionPtr &conn);
    void start();

private:
    Myconf &_conf;
    TcpServer _tcpServer; //IO线程
    Threadpool _threadpool; //计算线程
    TimerThread _timer; 
};

}//end of namespace lys

#endif
