#ifndef XAcceptor_hpp
#define XAcceptor_hpp

#include <format>
#include <functional>
#include <iostream>
#include <memory>

class XEventLoop;
class XSocket;
class XInternetAddress;
class XChannel;

class XAcceptor
{
private:
	std::shared_ptr<XEventLoop> event_loop;
	std::shared_ptr<XInternetAddress> internet_address;
	std::shared_ptr<XSocket> socket;
	std::shared_ptr<XChannel> accept_channel;
	std::function<void(std::shared_ptr<XSocket>)> new_connection_callback;

public:
	XAcceptor(std::shared_ptr<XEventLoop> _event_loop);
	~XAcceptor();

	void AcceptConnection();
	void SetNewConnectionCallback(std::function<void(std::shared_ptr<XSocket>)> _new_connection_callback);
};

#endif // XAcceptor_hpp
