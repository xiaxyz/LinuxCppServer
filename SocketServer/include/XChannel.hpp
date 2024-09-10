#ifndef XChannel_hpp
#define XChannel_hpp

#include <cstdint>

class XEpoll;
class XSocket;

class XChannel
{
private:
    XEpoll *epoll;
    XSocket *socket;
    uint32_t events;
    uint32_t revents;
    bool in_epoll;

public:
    XChannel(XEpoll *_epoll, XSocket *_socket);
    ~XChannel();

    int EnableReading();

    XSocket *GetXSocket();
    uint32_t GetEvents();
    uint32_t GetRevents();
    void SetRevents(uint32_t _revents);
    bool GetInEpoll();
    void SetInEpoll(bool _in_epoll);
};

#endif // XChannel_hpp
