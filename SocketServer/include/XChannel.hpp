#ifndef XChannel_hpp
#define XChannel_hpp

#include <cstdint>
#include <sys/epoll.h>
#include <functional>

class XEventLoop;
class XSocket;

class XChannel
{
private:
    XEventLoop *event_loop;
    XSocket *socket;
    uint32_t events;
    uint32_t revents;
    bool in_epoll;

    std::function<void()> callback;

public:
    XChannel(XEventLoop *_event_loop, XSocket *_socket);
    ~XChannel();

    int EnableReading();

    XSocket *GetXSocket();
    uint32_t GetEvents();
    uint32_t GetRevents();
    void SetRevents(uint32_t _revents);
    bool GetInEpoll();
    void SetInEpoll(bool _in_epoll);
    void SetCallback(std::function<void()> _callback);

    void HandleEvent();
};

#endif // XChannel_hpp
