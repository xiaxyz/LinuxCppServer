#ifndef XEventLoop_hpp
#define XEventLoop_hpp

#include <vector>

class XEpoll;
class XChannel;

class XEventLoop
{
private:
    XEpoll *epoll;
    bool quit;

public:
    XEventLoop();
    ~XEventLoop();

    void Loop();
    int UpdateChannel(XChannel *_channel);

};


#endif // XEventLoop_hpp
