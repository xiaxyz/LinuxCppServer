#include "XChannel.hpp"

#include "XEventLoop.hpp"

const short XChannel::READ_EVENT = 0b1;
const short XChannel::WRITE_EVENT = 0b10;
const short XChannel::ET = 0b100;

XChannel::XChannel(std::shared_ptr<XSocket> _socket, std::shared_ptr<XEventLoop> _event_loop)
	: socket(_socket),
	  event_loop(_event_loop),
	  listen_events(0),
	  ready_events(0)

{
}

XChannel::~XChannel()
{
}

void XChannel::HandleEvent() const
{
	if(ready_events & READ_EVENT)
	{
		read_callback();
	}
	if(ready_events & WRITE_EVENT)
	{
		write_callback();
	}
}

void XChannel::EnableRead()
{
	listen_events |= READ_EVENT;
	event_loop->UpdateChannel(shared_from_this());
}

void XChannel::EnableWrite()
{
	listen_events |= WRITE_EVENT;
	event_loop->UpdateChannel(shared_from_this());
}

void XChannel::EnableET()
{
	listen_events |= ET;
	event_loop->UpdateChannel(shared_from_this());
}

std::shared_ptr<XSocket> XChannel::GetSocket() const
{
	return socket;
}

short XChannel::GetListenEvents() const
{
	return listen_events;
}

short XChannel::GetReadyEvents() const
{
	return ready_events;
}

bool XChannel::GetExist() const
{
	return exist;
}

void XChannel::SetReadyEvents(short _ready_events)
{
	ready_events = _ready_events;
}

void XChannel::SetReadCallback(const std::function<void()> &_read_callback)
{
	read_callback = _read_callback;
}

void XChannel::SetWriteCallback(const std::function<void()> &_write_callback)
{
	write_callback = _write_callback;
}

void XChannel::SetExist(bool _exist)
{
	exist = _exist;
}
