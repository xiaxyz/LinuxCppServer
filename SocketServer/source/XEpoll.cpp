#include "XEpoll.hpp"
#include "XChannel.hpp"
#include "XSocket.hpp"
#include "XUtility.hpp"

XEpoll::XEpoll() : epoll_fd(-1), events(nullptr)
{
    epoll_fd = epoll_create1(0);
    events = std::make_shared<epoll_event[]>(MAX_EVENTS);
    std::memset(events.get(), 0, sizeof(events));
}

XEpoll::~XEpoll()
{
    if (epoll_fd != -1)
    {
        close(epoll_fd);
        epoll_fd = -1;
    }
}

int XEpoll::AddFd(int _fd, uint32_t _events)
{
    auto event_ = epoll_event();
    memset(&event_, 0, sizeof(event_));
    event_.data.fd = _fd;
    event_.events = _events;
    return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _fd, &event_);
}

std::vector<std::shared_ptr<XChannel>> XEpoll::TriggeredEvents(int _timeout)
{
    auto num_fd_ = epoll_wait(epoll_fd, events.get(), MAX_EVENTS, _timeout);
    auto triggered_events_ = std::vector<std::shared_ptr<XChannel>>(num_fd_);
    for (auto i = 0; i < num_fd_; ++i)
    {
        auto channel = ptr_channel.find(static_cast<XChannel *>(events[i].data.ptr))->second;
        channel->SetRevents(events[i].events);
        triggered_events_[i] = channel;
    }
    return triggered_events_;
}

void XEpoll::UpdateChannel(std::shared_ptr<XChannel> _channel)
{
    auto event_ = epoll_event();
    memset(&event_, 0, sizeof(event_));
    ptr_channel[_channel.get()] = _channel;
    event_.data.ptr = static_cast<void *>(_channel.get());
    event_.events = _channel->GetEvents();
    if (_channel->GetInEpoll())
    {
        ErrorIfFile(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, _channel->GetXSocket()->GetFd(), &event_) == -1, "epoll modify event error");
    }
    else
    {
        ErrorIfFile(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _channel->GetXSocket()->GetFd(), &event_) == -1, "epoll add event error");
        _channel->SetInEpoll(true);
    }
}
