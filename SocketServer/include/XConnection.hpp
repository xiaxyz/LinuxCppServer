#ifndef XConnection_hpp
#define XConnection_hpp

#include <cassert>
#include <cstring>

#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <unistd.h>

#define READ_BUFFER 1024

class XEventLoop;
class XSocket;
class XChannel;
class XBuffer;

class XConnection : public std::enable_shared_from_this<XConnection>
{
public:
	enum State : unsigned int
	{
		Invalid = 1,
		Connected,
		Closed,
	};

private:
	std::shared_ptr<XSocket> socket;
	std::shared_ptr<XEventLoop> event_loop;
	std::shared_ptr<XChannel> channel;
	State state;
	std::shared_ptr<XBuffer> read_buffer;
	std::shared_ptr<XBuffer> send_buffer;
	std::function<void(std::shared_ptr<XSocket>)> delete_connection;
	std::function<void(std::shared_ptr<XConnection>)> on_receive;

public:
	XConnection(const XConnection &) = delete;
	XConnection &operator=(const XConnection &) = delete;
	XConnection(const XConnection &&) = delete;
	XConnection &operator=(const XConnection &&) = delete;
	XConnection(std::shared_ptr<XSocket> _socket, std::shared_ptr<XEventLoop> _event_loop);
	~XConnection();

	void Read();
	void ReadBlocking();
	void ReadNonblocking();

	void Write();
	void WriteBlocking();
	void WriteNonblocking();

	void Send(const char *_message);
	void Business();

	void Close();

	State GetState();

	void SetDeleteConnection(const std::function<void(std::shared_ptr<XSocket>)> &_delete_connection);
	void SetOnReceive(const std::function<void(std::shared_ptr<XConnection>)> &_on_receive);
	std::shared_ptr<XSocket> GetSocket();
	void SetReadBuffer(const char *_read_buffer);
	std::shared_ptr<XBuffer> GetReadBuffer();
	void SetSendBuffer(const char *_send_buffer);
	std::shared_ptr<XBuffer> GetSendBuffer();
};

#endif // XConnection_hpp
