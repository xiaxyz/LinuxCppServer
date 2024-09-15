#include "XThreadPool.hpp"

XThreadPool::XThreadPool(int _size) : stop(false)
{
    for (auto i = 0; i < _size; ++i)
    {
        threads.emplace_back(std::thread([this]()
        {
            while (true)
            {
                auto task_ = std::function<void()>();
                auto look_ = std::unique_lock<std::mutex>(tasks_mutex);
                variable.wait(look_, [this]() { return stop || !tasks.empty(); });
                if (stop && tasks.empty())
                {
                    return;
                }
                task_ = tasks.front();
                tasks.pop();
                look_.unlock();
                task_();
            }
        }));
    }
}

XThreadPool::~XThreadPool()
{
    auto look_ = std::unique_lock<std::mutex>(tasks_mutex);
    stop = true;
    look_.unlock();
    variable.notify_all();
    for (auto &i_threads : threads)
    {
        if (i_threads.joinable())
        {
            i_threads.join();
        }
    }
}

void XThreadPool::Add(std::function<void()> _function)
{
    auto look_ = std::unique_lock<std::mutex>(tasks_mutex);
    if (stop)
    {
        throw std::runtime_error("ThreadPool already stop, can't add task any more");
    }
    tasks.emplace(_function);
    look_.unlock();
    variable.notify_one();
}
