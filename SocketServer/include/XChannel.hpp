#ifndef XChannel_hpp
#define XChannel_hpp

#include <cstdint>
#include <functional>
#include <memory>
#include <sys/epoll.h>

class XEventLoop;
class XSocket;

class XChannel : public std::enable_shared_from_this<XChannel>
{
private:
	std::shared_ptr<XEventLoop> event_loop;
	std::shared_ptr<XSocket> socket;
	uint32_t events;
	uint32_t revents;
	bool in_epoll;

	std::function<void()> read_callback;
	std::function<void()> write_callback;

public:
	XChannel(std::shared_ptr<XEventLoop> _event_loop, std::shared_ptr<XSocket> _socket);
	~XChannel();

	void EnableRead();
	void UseET();

	std::shared_ptr<XSocket> GetXSocket();
	uint32_t GetEvents();
	uint32_t GetRevents();
	void SetRevents(uint32_t _revents);
	bool GetInEpoll();
	void SetInEpoll(bool _in_epoll);
	void SetReadCallback(std::function<void()> _callback);

	void HandleEvent();
};

#endif // XChannel_hpp
