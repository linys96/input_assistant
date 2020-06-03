#ifndef __LYS_ACCEPTOR_HPP__
#define __LYS_ACCEPTOR_HPP__

#include "InetAddress.hpp"
#include "Socket.hpp"
#include <string>

namespace lys
{

class Acceptor
{
public:
    explicit
    Acceptor(unsigned short port);
    Acceptor(const std::string &ip, unsigned short port);
    void ready();
    int accept();
    int fd() const { return _listenSock.fd();  }
private:
    void setReuseAddr(bool);
    void setReusePort(bool);
    void bind();
    void listen();
private:
    InetAddress _addr;
    Socket _listenSock;
};


}//end of namespace lys

#endif
