#ifndef XEpoll_hpp
#define XEpoll_hpp

#include <cstring>
#include <memory>
#include <sys/epoll.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

class XChannel;

#define MAX_EVENTS 1024

class XEpoll
{
private:
	int epoll_fd;
	std::shared_ptr<epoll_event[]> events;
	std::unordered_map<XChannel *, std::shared_ptr<XChannel>> ptr_channel;

public:
	XEpoll();
	~XEpoll();

	int AddFd(int _fd, uint32_t _events);
	std::vector<std::shared_ptr<XChannel>> TriggeredEvents(int _timeout = -1);

	void UpdateChannel(std::shared_ptr<XChannel> _channel);
};

#endif // XEpoll_hpp
