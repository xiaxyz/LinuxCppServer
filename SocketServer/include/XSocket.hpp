#ifndef XSocket_hpp
#define XSocket_hpp

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <utility>
#include <unordered_set>

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

    int Bind(XInternetAddress &_address);
    int Listen(int _length = SOMAXCONN);

    void SetNonBlocking();

    XSocket Accept(XInternetAddress &_address);

    void Close();

    int GetFd();
    void SetFd(int _fd);
};

#endif // XSocket_hpp
