#include "../include/InetAddress.hpp"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

namespace lys
{

InetAddress::InetAddress(unsigned short port)
{
    memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);//主机字节序转网络字节序
    _addr.sin_addr.s_addr=INADDR_ANY;//本机IP地址
}

InetAddress::InetAddress(const std::string &ip, unsigned short port)
{
    memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);//主机字节序转网络字节序
    _addr.sin_addr.s_addr=inet_addr(ip.c_str());
}

InetAddress::InetAddress(const struct sockaddr_in &addr)
:_addr(addr)
{
}

std::string InetAddress::ip() const
{
    return string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const
{
    return ntohs(_addr.sin_port);
}

struct sockaddr_in* InetAddress::getInetAddressPtr()
{
    return &_addr;
}

}//end of namespace lys
