#include "XInternetAddress.hpp"

XInternetAddress::XInternetAddress() : socket_address(), socket_length(sizeof(socket_address))
{
    memset(&socket_address, 0, sizeof(socket_address));
}

XInternetAddress::XInternetAddress(const char *_ip, uint16_t _port) : socket_address(), socket_length(sizeof(socket_address))
{
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = inet_addr(_ip);
    socket_address.sin_port = htons(_port);
}

XInternetAddress::~XInternetAddress()
{
}

sockaddr_in &XInternetAddress::SocketAddress()
{
    return socket_address;
}

socklen_t &XInternetAddress::SocketLength()
{
    socket_length = sizeof(socket_address);
    return socket_length;
}
