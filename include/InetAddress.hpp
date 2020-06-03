#ifndef __LYS_INETADDRESS_HPP__
#define __LYS_INETADDRESS_HPP__

#include <arpa/inet.h>
#include <string>

namespace lys
{

class InetAddress
{
public:
    explicit
    InetAddress(unsigned short port);
    InetAddress(const std::string &ip, unsigned short port);
    InetAddress(const struct sockaddr_in &);

    std::string ip() const;
    unsigned short port() const;
    struct sockaddr_in* getInetAddressPtr();
private:
    struct sockaddr_in _addr;
};

}//end of namespace lys

#endif
