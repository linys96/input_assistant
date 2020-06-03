#include "../include/TcpConnection.hpp"
#include "../include/EventLoop.hpp"
#include <sstream>
namespace lys
{

TcpConnection::TcpConnection(int fd, EventLoop *loop)
:_loop(loop)
,_sock(fd)
,_localAddr(getLocalAddr(fd))
,_peerAddr(getPeerAddr(fd))
,_socketIO(fd)
,_isShutdownWrite(false)
{
}

TcpConnection::~TcpConnection()
{
    if(!_isShutdownWrite){
        shutdown();
    }
}

std::string TcpConnection::receive()
{
    char buff[1024]={0};
    /* printf("readLine\n"); */
    _socketIO.readLine(buff, sizeof(buff));
    /* printf("after readLine\n"); */

    return  std::string(buff);
}

void TcpConnection::send(const std::string &msg)
{
    _socketIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::sendInLoop(const std::string &msg)
{
    _loop->runInLoop(std::bind(&TcpConnection::send,this,msg));
}

int32_t TcpConnection::recvInt32()
{
    int32_t num;
    _socketIO.readInt32(num); 
    return num;
}

void TcpConnection::sendInt32(int32_t num)
{
    _socketIO.writeInt32(num);
}

void TcpConnection::receiveN(char *buff, int len)
{
    _socketIO.readn(buff,len);
}

void TcpConnection::sendN(const char* buff, int len)
{
    _socketIO.writen(buff,len);
}

std::string TcpConnection::toString() const
{
    std::ostringstream oss;
    oss << _localAddr.ip() << ":" << _localAddr.port() 
        << " ----> " << _peerAddr.ip() << ":" << _peerAddr.port();
    return oss.str();
}
void TcpConnection::shutdown()
{
    if(!_isShutdownWrite) {
        _isShutdownWrite = true;
        _sock.shutdownWrite();
    }
}

InetAddress TcpConnection::getLocalAddr(int fd )
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(getsockname(_sock.fd(),(struct sockaddr*)&addr,&len)==-1){
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr(int fd )
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if(getpeername(_sock.fd(),(struct sockaddr*)&addr,&len)==-1){
        perror("getpeername");
    }
    return InetAddress(addr);
}
void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb)
{
    _onConnection=cb;    
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb)
{
    _onMessage=cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb)
{
    _onClose=cb;
}

void TcpConnection::handleConnectionCallback()
{
    if(_onConnection) {
        /* printf(">> handleConnectionCallback\n"); */
        _onConnection(shared_from_this());
    }
    /* printf(">> exit handleConnectionCallback\n"); */
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessage) {
        /* printf(">> handleMessageCallback\n"); */ 
        _onMessage(shared_from_this());
    }
    /* printf(">> exit handleMessageCallback\n"); */ 
}

void TcpConnection::handleCloseCallback()
{
    if(_onClose) {
        /* printf(">> handleCloseCallback\n"); */
        _onClose(shared_from_this());
    }
    /* printf(">> exit handleCloseCallback\n"); */
}

}//end of namespace lys
