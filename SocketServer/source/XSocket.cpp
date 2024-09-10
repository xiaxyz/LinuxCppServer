#include "XSocket.hpp"
#include "XInternetAddress.hpp"

std::unordered_set<int> XSocket::flag = std::unordered_set<int>();
int XSocket::count = 0;

XSocket::XSocket() : fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    flag.insert(fd);
    ++count;
}

XSocket::XSocket(int _fd) : fd(_fd)
{
    flag.insert(fd);
    ++count;
}

XSocket::XSocket(const XSocket &_other) : fd(_other.fd)
{
    ++count;
}

XSocket::~XSocket()
{
    --count;
    flag.erase(fd);
    if (count <= 0)
    {
        for (auto i_flag : flag)
        {
            close(i_flag);
        }
    }
}

int XSocket::Bind(XInternetAddress &_address)
{
    return bind(fd, (sockaddr *)&_address.GetAddress(), _address.GetSize());
}

int XSocket::Listen(int _length)
{
    return listen(fd, _length);
}

void XSocket::SetNonBlocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

XSocket XSocket::Accept(XInternetAddress &_address)
{
    return XSocket(accept(fd, (sockaddr *)&_address.GetAddress(), &_address.GetSize()));
}

void XSocket::Close()
{
    close(fd);
    flag.erase(fd);
}

int XSocket::GetFd()
{
    return fd;
}

void XSocket::SetFd(int _fd)
{
    fd = _fd;
    flag.insert(fd);
}
