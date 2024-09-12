#ifndef XEpoll_hpp
#define XEpoll_hpp

#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <vector>

class XChannel;

#define MAX_EVENTS 1024

class XEpoll
{
private:
    int epoll_fd;
    epoll_event *events;
public:
    XEpoll();
    ~XEpoll();

    int AddFd(int _fd, uint32_t _events);
    std::vector<XChannel *> TriggeredEvents(int _timeout = -1);

    void UpdateChannel(XChannel *_channel);
};

#endif // XEpoll_hpp
