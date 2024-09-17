#include "XThreadPool.hpp"
#include "XEventLoop.hpp"

XThreadPool::XThreadPool(int _size)
	: stop(false)
{
	for(auto i = 0; i < _size; ++i)
	{
		threads.emplace_back(std::thread([this]()
		{
			while(true)
			{
				auto task_ = std::function<void()>();
				auto lock_ = std::unique_lock<std::mutex>(tasks_mutex);
				variable.wait(lock_, [this]()
				{
					return stop || !tasks.empty();
				});
				if(stop && tasks.empty())
				{
					return;
				}
				task_ = tasks.front();
				tasks.pop();
				lock_.unlock();
				task_();
			}
		}));
	}
}

XThreadPool::~XThreadPool()
{
	auto lock_ = std::unique_lock<std::mutex>(tasks_mutex);
	stop = true;
	lock_.unlock();
	variable.notify_all();
	for(auto &i_threads : threads)
	{
		if(i_threads.joinable())
		{
			i_threads.join();
		}
	}
}

template<typename Function, typename... Args>
std::future<typename std::invoke_result<Function, Args...>::type> XThreadPool::Add(Function &&_function, Args &&..._args)
{
	using return_type = typename std::invoke_result<Function, Args...>::type;
	auto task_ = std::make_shared<std::packaged_task<return_type()>>(std::function<return_type(Args...)>(std::bind(std::forward<Function>(_function), std::forward<Args>(_args)...)));
	auto result_ = task_->get_future();
	auto lock_ = std::unique_lock<std::mutex>(tasks_mutex);
	if(stop)
	{
		throw std::runtime_error("enqueue on stopped ThreadPool");
	}
	tasks.emplace([task_]()
	{
		(*task_)();
	});
	lock_.unlock();
	variable.notify_one();
	return result_;
}
template std::future<void> XThreadPool::Add<std::function<void()> &>(std::function<void()> &);
