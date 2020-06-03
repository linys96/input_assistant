#ifndef __LYS_TCPCONNECTION_HPP__
#define __LYS_TCPCONNECTION_HPP__

#include "Socket.hpp"
#include "InetAddress.hpp"
#include "SocketIO.hpp"
#include "Noncopyable.hpp"

#include <string>
#include <memory>
#include <functional>

namespace lys
{
class EventLoop;
class TcpConnection;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

class TcpConnection
:Noncopyable
,public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
public:
    TcpConnection(int, EventLoop*);
    ~TcpConnection();

    std::string receive();
    void send(const std::string &msg);
    void sendInLoop(const std::string &msg);
    int32_t recvInt32();
    void sendInt32(int32_t num);
    void receiveN(char *buff, int len);
    void sendN(const char* buff, int len);

    std::string toString() const;
    void shutdown();

    void setConnectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback &cb);
    void setCloseCallback(const TcpConnectionCallback &cb);
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
private:
    InetAddress getLocalAddr(int fd);
    InetAddress getPeerAddr(int fd);

private:
    EventLoop *_loop;
    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    SocketIO _socketIO;
    bool _isShutdownWrite;

    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
};

}//end of namespace lys

#endif
