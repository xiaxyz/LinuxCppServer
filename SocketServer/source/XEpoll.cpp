#include "XEpoll.hpp"

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
    auto temp_event = epoll_event();
    memset(&temp_event, 0, sizeof(temp_event));
    temp_event.data.fd = _fd;
    temp_event.events = _events;
    return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _fd, &temp_event);
}

std::vector<epoll_event> XEpoll::TriggeredEvents(int _timeout)
{
    auto triggered_events = std::vector<epoll_event>();
    auto num_fd = epoll_wait(epoll_fd, events, MAX_EVENTS, _timeout);
    for (auto i = 0; i < num_fd; ++i)
    {
        triggered_events.push_back(events[i]);
    }
    return std::move(triggered_events);
}
