#ifndef __LYS_SOCKETIO_HPP__
#define __LYS_SOCKETIO_HPP__

#include <stdint.h>
namespace lys
{

class SocketIO
{
public:
    explicit
    SocketIO(int fd):_fd(fd) {}
    int readn(char * buff, int len);
    int writen(const char *buff, int len);
    int readLine(char *buff, int maxlen);
    int readInt32(int32_t &num);
    int writeInt32(const int32_t num);

private:
    int recvPeek(char *buff, int len);
    int _fd;
};

}//end of namespace lys

#endif
