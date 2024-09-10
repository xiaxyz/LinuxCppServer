#include "XChannel.hpp"
#include "XEpoll.hpp"

XChannel::XChannel(XEpoll *_epoll, XSocket *_socket) : epoll(_epoll), socket(_socket), in_epoll(false)
{
}

XChannel::~XChannel()
{
}

int XChannel::EnableReading()
{
    events = EPOLLIN | EPOLLET;
    return epoll->UpdateChannel(this);
}

XSocket *XChannel::GetXSocket()
{
    return socket;
}

uint32_t XChannel::GetEvents()
{
    return events;
}

uint32_t XChannel::GetRevents()
{
    return revents;
}

void XChannel::SetRevents(uint32_t _revents)
{
    revents = _revents;
}

bool XChannel::GetInEpoll()
{
    return in_epoll;
}

void XChannel::SetInEpoll(bool _in_epoll)
{
    in_epoll = _in_epoll;
}
