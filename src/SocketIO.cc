#include "../include/SocketIO.hpp"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace lys
{

int SocketIO::readn(char * buff, int len)
{
    int left = len;
    char *p = buff;
    int ret;
    while(left > 0) {
        ret = ::read(_fd, p, left);
        if(ret == -1 && errno == EINTR) {
            continue;
        } else if(ret == -1) {
            perror("read");
            return len - left;
        } else {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

int SocketIO::writen(const char *buff, int len)
{
    int left = len;
    const char *p = buff;
    int ret;
    while(left > 0) {
        ret = ::write(_fd, p, left);
        if(ret == -1 && errno == EINTR) {
            continue;
        } else if(ret == -1) {
            perror("read");
            return len - left;
        } else {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

int SocketIO::readLine(char *buff, int maxlen)
{
    int left = maxlen - 1;
    /* int left = maxlen; */
    char *p = buff;
    int ret;
    int total = 0;
    while(left > 0) {
        /* printf("before rcvpeek\n"); */
        ret = recvPeek(p, left);
        /* printf("after rcvpeek\n"); */
        if(ret != -1)
        {
            for(int idx = 0; idx != ret; ++idx) {
                if(p[idx] == '\n') {
                    int sz = idx + 1; 
                    readn(p, sz);
                    total += sz;
                    p += sz;
                    *p = '\0';
                    return total;
                }  
            }
            readn(p, ret);
            left -= ret;
            p += ret;
            total += ret;
        }
    }
    *p = '\0';
    return total;
}

int SocketIO::recvPeek(char *buff, int len)
{
    int ret;
    do {
        ret = ::recv(_fd, buff, len, MSG_PEEK);
    }while(ret == -1 && errno == EINTR);
    if(ret == -1) {
        perror("recv");
    }
    return ret;
}

int SocketIO::readInt32(int32_t &num)
{
   int ret;
   ret = readn((char*)&num,sizeof(int32_t));
   return ret;
}

int SocketIO::writeInt32(const int32_t num)
{
   int ret;
   ret = writen((char*)&num,sizeof(int32_t));
   return ret;
}


}//end of namespace lys
