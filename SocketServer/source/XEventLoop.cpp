#include "XEventLoop.hpp"
#include "XChannel.hpp"
#include "XPoller.hpp"
#include "XThreadPool.hpp"

XEventLoop::XEventLoop()
	: quit(false)
{
	poller = std::make_unique<XPoller>();
}

XEventLoop::~XEventLoop()
{
}

void XEventLoop::Loop()
{
	while(!quit)
	{
		auto channels = poller->TriggeredEvents();
		for(const auto &i_channel : channels)
		{
			i_channel->HandleEvent();
		}
	}
}

void XEventLoop::UpdateChannel(std::shared_ptr<XChannel> _channel)
{
	poller->UpdateChannel(_channel);
}
