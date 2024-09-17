#include "XChannel.hpp"
#include "XEventLoop.hpp"

XChannel::XChannel(std::shared_ptr<XEventLoop> _event_loop, std::shared_ptr<XSocket> _socket)
	: event_loop(_event_loop),
	  socket(_socket),
	  events(0),
	  revents(0),
	  in_epoll(false)
{
}

XChannel::~XChannel()
{
}

void XChannel::EnableRead()
{
	events = EPOLLIN | EPOLLET;
	event_loop->UpdateChannel(shared_from_this());
}

void XChannel::UseET()
{
	events |= EPOLLET;
	event_loop->UpdateChannel(shared_from_this());
}

std::shared_ptr<XSocket> XChannel::GetXSocket()
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

void XChannel::SetReadCallback(std::function<void()> _read_callback)
{
	read_callback = _read_callback;
}

void XChannel::HandleEvent()
{
	if(revents & (EPOLLIN | EPOLLPRI))
	{
		read_callback();
	}
	else if(revents & EPOLLOUT)
	{
		write_callback();
	}
}
