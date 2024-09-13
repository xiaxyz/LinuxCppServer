#ifndef XEventLoop_hpp
#define XEventLoop_hpp

#include <vector>
#include <memory>

class XEpoll;
class XChannel;

class XEventLoop
{
private:
    std::unique_ptr<XEpoll> epoll;
    std::vector<std::shared_ptr<XChannel>> channels;
    bool quit;

public:
    XEventLoop();
    ~XEventLoop();

    void Loop();
    void UpdateChannel(std::shared_ptr<XChannel> _channel);

};


#endif // XEventLoop_hpp
