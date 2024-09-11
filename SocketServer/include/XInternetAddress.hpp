#ifndef XInternetAddress_hpp
#define XInternetAddress_hpp

#include <arpa/inet.h>
#include <cstring>

class XInternetAddress
{
private:
    sockaddr_in socket_address;
    socklen_t socket_length;
public:
    XInternetAddress();
    XInternetAddress(const char *_ip, uint16_t _port);
    ~XInternetAddress();

    sockaddr_in &SocketAddress();
    socklen_t &SocketLength();
};

#endif // XInternetAddress_hpp
