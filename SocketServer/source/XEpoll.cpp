#include "XEpoll.hpp"
#include "XChannel.hpp"
#include "XSocket.hpp"

XEpoll::XEpoll() : epoll_fd(-1), events(nullptr)
{
    epoll_fd = epoll_create1(0);
    events = new epoll_event[MAX_EVENTS];
    memset(events, 0, sizeof(events));
}

XEpoll::~XEpoll()
{
    if (epoll_fd != -1)
    {
        close(epoll_fd);
        epoll_fd = -1;
    }
    delete[] events;
}

int XEpoll::AddFd(int _fd, uint32_t _events)
{
    auto event = epoll_event();
    memset(&event, 0, sizeof(event));
    event.data.fd = _fd;
    event.events = _events;
    return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _fd, &event);
}

std::vector<XChannel *> XEpoll::TriggeredEvents(int _timeout)
{
    auto triggered_events = std::vector<XChannel *>();
    auto num_fd = epoll_wait(epoll_fd, events, MAX_EVENTS, _timeout);
    for (auto i = 0; i < num_fd; ++i)
    {
        auto channel = (XChannel *)events[i].data.ptr;
        channel->SetRevents(events[i].events);
        triggered_events.push_back(channel);
    }
    return std::move(triggered_events);
}

int XEpoll::UpdateChannel(XChannel *_channel)
{
    auto event = epoll_event();
    memset(&event, 0, sizeof(event));
    event.data.ptr = _channel;
    event.events = _channel->GetEvents();
    if (_channel->GetInEpoll())
    {
        return epoll_ctl(epoll_fd, EPOLL_CTL_MOD, _channel->GetXSocket()->GetFd(), &event);
    }
    else
    {
        _channel->SetInEpoll(true);
        return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _channel->GetXSocket()->GetFd(), &event);
    }
}
