#include "../include/Socket.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>


namespace lys
{

Socket::Socket()
{
    _fd = socket(AF_INET,SOCK_STREAM,0);    
    if(-1 == _fd){
        perror("socket");
    }
}

Socket::Socket(int fd)
:_fd(fd)
{}

Socket::~Socket()
{
    ::close(_fd);
}

void Socket::shutdownWrite()
{
   ::shutdown(_fd, SHUT_WR); 
}

int Socket::fd() const
{
    return _fd;
}

}//end of namespace lys
