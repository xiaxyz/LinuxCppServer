#ifndef XPoller_hpp
#define XPoller_hpp

#include <cstring>

#include <memory>
#include <unordered_map>
#include <vector>

#ifdef OS_LINUX

#include <sys/epoll.h>
#include <unistd.h>

#endif

#ifdef OS_MACOS

#include <sys/event.h>
#include <unistd.h>

#endif

class XChannel;

#define MAX_EVENTS 1024

class XPoller
{
private:
	int poller_fd;

#ifdef OS_LINUX

	std::shared_ptr<epoll_event[]> events;

#endif

#ifdef OS_MACOS

	std::shared_ptr<kevent[]> events;

#endif

	std::unordered_map<XChannel *, std::shared_ptr<XChannel>> ptr_channel;

public:
	XPoller();
	~XPoller();

	int AddFd(int _fd, uint32_t _events);
	std::vector<std::shared_ptr<XChannel>> TriggeredEvents(int _timeout = -1);

	void UpdateChannel(std::shared_ptr<XChannel> _channel);
};

#endif // XPoller_hpp
