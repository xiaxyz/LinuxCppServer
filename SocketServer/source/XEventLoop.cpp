#include "XEventLoop.hpp"
#include "XChannel.hpp"
#include "XEpoll.hpp"

XEventLoop::XEventLoop() : quit(false)
{
    epoll = std::make_unique<XEpoll>();
}

XEventLoop::~XEventLoop()
{
}

void XEventLoop::Loop()
{
    while (!quit)
    {
        auto channels = epoll->TriggeredEvents();
        for (const auto &i_channel : channels)
        {
            i_channel->HandleEvent();
        }
    }

}

void XEventLoop::UpdateChannel(std::shared_ptr<XChannel> _channel)
{
    epoll->UpdateChannel(_channel);
}
