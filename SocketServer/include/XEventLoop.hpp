#ifndef XEventLoop_hpp
#define XEventLoop_hpp

#include <functional>
#include <memory>
#include <vector>

class XEpoll;
class XChannel;
class XThreadPool;

class XEventLoop
{
private:
    std::unique_ptr<XEpoll> epoll;
    std::unique_ptr<XThreadPool> thread_pool;
    bool quit;

public:
    XEventLoop();
    ~XEventLoop();

    void Loop();
    void UpdateChannel(std::shared_ptr<XChannel> _channel);

    void AddThread(std::function<void()> _function);
};

#endif // XEventLoop_hpp
