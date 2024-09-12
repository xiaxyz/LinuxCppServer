#ifndef XSocket_hpp
#define XSocket_hpp

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <utility>
#include <unordered_set>
#include <compare>

#include "XInternetAddress.hpp"

class XInternetAddress;

class XSocket
{
private:
    int fd;
    static std::unordered_set<int> flag;
    static int count;
public:
    XSocket();
    XSocket(int _fd);
    XSocket(const XSocket &_other);
    ~XSocket();

    std::strong_ordering operator<=>(const XSocket &_other) const;

    void Bind(XInternetAddress *_address);
    void Listen(int _length = SOMAXCONN);

    void SetNonBlocking();

    XSocket Accept(XInternetAddress *_address);

    void Close();

    int GetFd();
    void SetFd(int _fd);
};

#endif // XSocket_hpp
