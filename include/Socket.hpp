#ifndef __LYS_SOCKET_HPP__
#define __LYS_SOCKET_HPP__

namespace lys
{

class Socket
{
public:
    Socket(); 
    Socket(int);
    ~Socket(); 
    void shutdownWrite();
    int fd() const;

private:
    int _fd;
};

}//end of namespace lys

#endif
