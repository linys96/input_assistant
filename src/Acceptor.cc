#include "../include/Acceptor.hpp"

#include <sys/types.h>
#include <sys/socket.h>

namespace lys
{

Acceptor::Acceptor(unsigned short port)
:_addr(port)
,_listenSock()
{
}
Acceptor::Acceptor(const std::string &ip, unsigned short port)
:_addr(ip,port)
,_listenSock()
{
}

void Acceptor::ready()
{
     setReuseAddr(true);
     setReusePort(true);
     bind();
     listen();
}

int Acceptor::accept()
{
    int peerfd = ::accept(_listenSock.fd(), NULL, NULL);
    if(-1 == peerfd) {
        perror("accept");
    }
    return peerfd;
}

void Acceptor::setReuseAddr(bool flag)
{
    int reuse= flag;
    int ret = setsockopt(_listenSock.fd(),
                         SOL_SOCKET,
                         SO_REUSEADDR,
                         &reuse,
                         sizeof(int));
    if(-1 == ret) {
        perror("setsockopt");
    }
}

void Acceptor::setReusePort(bool flag)
{
    int reuse= flag;
    int ret = setsockopt(_listenSock.fd(),
                         SOL_SOCKET,
                         SO_REUSEPORT,
                         &reuse,
                         sizeof(int));
    if(-1 == ret) {
        perror("setsockopt");
    }

}

void Acceptor::bind()
{
    int ret = ::bind(_listenSock.fd(),
            (struct sockaddr *)_addr.getInetAddressPtr(),
            sizeof(struct sockaddr));
    if(-1 == ret) {
        perror("bind");
    }
}

void Acceptor::listen()
{
    int ret = ::listen(_listenSock.fd(), 10);
    if(-1 == ret) {
        perror("listen");
    }
}

}//end of namespace lys
