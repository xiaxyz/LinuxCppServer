#include "XSocket.hpp"
#include "XUtility.hpp"

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

void XSocket::Bind(XInternetAddress *_address)
{
    ErrorIfFile(bind(fd, (sockaddr *)&_address->SocketAddress(), _address->SocketLength()) == -1, "socket bind error");
}

void XSocket::Listen(int _length)
{
    ErrorIfFile(listen(fd, _length), "socket listen error");
}


void XSocket::SetNonBlocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

XSocket XSocket::Accept(XInternetAddress *_address)
{
    auto client_ = XSocket(accept(fd, (sockaddr *)&_address->SocketAddress(), &_address->SocketLength()));
    ErrorIfFile(client_.GetFd() == -1, "socket accept error");
    return client_;
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
