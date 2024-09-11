#include "XEventLoop.hpp"
#include "XChannel.hpp"
#include "XEpoll.hpp"

XEventLoop::XEventLoop() : epoll(nullptr), quit(false)
{
    epoll = new XEpoll();
}

XEventLoop::~XEventLoop()
{
    delete epoll;
}

void XEventLoop::Loop()
{
    while (!quit)
    {
        std::vector<XChannel *> channels;
        channels = epoll->TriggeredEvents();
        for (auto i_channel : channels)
        {
            i_channel->HandleEvent();
        }
    }

}

int XEventLoop::UpdateChannel(XChannel *_channel)
{
    return epoll->UpdateChannel(_channel);
}
