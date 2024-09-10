#ifndef XInternetAddress_hpp
#define XInternetAddress_hpp

#include <arpa/inet.h>
#include <cstring>

class XInternetAddress
{
private:
    sockaddr_in address;
    socklen_t length;
public:
    XInternetAddress();
    XInternetAddress(const char *_ip, uint16_t _port);
    ~XInternetAddress();

    sockaddr_in &GetAddress();
    socklen_t &GetSize();
};

#endif // XInternetAddress_hpp
