#include "XInternetAddress.hpp"

XInternetAddress::XInternetAddress() : address(), length(sizeof(address))
{
    memset(&address, 0, sizeof(address));
}

XInternetAddress::XInternetAddress(const char *_ip, uint16_t _port) : address(), length(sizeof(address))
{
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(_ip);
    address.sin_port = htons(_port);
}

XInternetAddress::~XInternetAddress()
{
}

sockaddr_in &XInternetAddress::GetAddress()
{
    return address;
}

socklen_t &XInternetAddress::GetSize()
{
    length = sizeof(address);
    return length;
}
