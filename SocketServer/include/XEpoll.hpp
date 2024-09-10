#ifndef XEpoll_hpp
#define XEpoll_hpp

#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <vector>

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
    std::vector<epoll_event> TriggeredEvents(int _timeout = -1);
};

#endif // XEpoll_hpp
