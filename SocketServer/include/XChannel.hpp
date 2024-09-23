#ifndef XChannel_hpp
#define XChannel_hpp

#include <functional>
#include <memory>

class XSocket;
class XEventLoop;

enum Result : unsigned int;

class XChannel : public std::enable_shared_from_this<XChannel>
{
private:
	std::shared_ptr<XSocket> socket;
	std::shared_ptr<XEventLoop> event_loop;
	short listen_events;
	short ready_events;
	bool exist;
	std::function<void()> read_callback;
	std::function<void()> write_callback;
public:
	static const short READ_EVENT;
	static const short WRITE_EVENT;
	static const short ET;
	XChannel(const XChannel &_other) = delete;
	XChannel &operator=(const XChannel &_other) = delete;
	XChannel(const XChannel &&_other) = delete;
	XChannel &operator=(const XChannel &&_other) = delete;
	XChannel(std::shared_ptr<XSocket> _socket, std::shared_ptr<XEventLoop> _event_loop);
	~XChannel();

	void HandleEvent() const;
	void EnableRead();
	void EnableWrite();
	void EnableET();

	std::shared_ptr<XSocket> GetSocket() const;
	short GetListenEvents() const;
	short GetReadyEvents() const;
	bool GetExist() const;

	void SetReadyEvents(short _ready_events);
	void SetReadCallback(const std::function<void()> &_read_callback);
	void SetWriteCallback(const std::function<void()> &_write_callback);
	void SetExist(bool _exist);
};


#endif // XChannel_hpp
