#ifndef XThreadPool_hpp
#define XThreadPool_hpp

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class XThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mutex;
    std::condition_variable variable;
    bool stop;

public:
    XThreadPool(int _size = std::thread::hardware_concurrency());
    ~XThreadPool();

    void Add(std::function<void()> _function);
};

#endif // XThreadPool_hpp
