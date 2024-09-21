#ifndef XServer_hpp
#define XServer_hpp

#include <cstring>

#include <format>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include <unistd.h>

class XEventLoop;
class XAcceptor;
class XSocket;
class XConnection;
class XThreadPool;

class XServer
{
private:
	std::shared_ptr<XEventLoop> main_reactor;
	std::unique_ptr<XAcceptor> acceptor;
	std::map<int, std::shared_ptr<XConnection>> connections;
	std::vector<std::shared_ptr<XEventLoop>> sub_reactors;
	std::unique_ptr<XThreadPool> thread_pool;
	std::function<void(std::shared_ptr<XConnection>)> on_connect_callback;
	std::function<void(std::shared_ptr<XConnection>)> on_message_callback;
	std::function<void(std::shared_ptr<XConnection>)> new_connect_callback;

public:
	XServer(std::shared_ptr<XEventLoop> _event_loop);
	~XServer();

	void NewConnection(std::shared_ptr<XSocket> _socket);
	void DeleteConnection(std::shared_ptr<XSocket> _socket);
	void OnConnect(std::function<void(std::shared_ptr<XConnection>)> _on_connect_callback);
	void OnMessage(std::function<void(std::shared_ptr<XConnection>)> _on_message_callback);
	void NewConnect(std::function<void(std::shared_ptr<XConnection>)> _new_connect_callback);
};

#endif // XServer_hpp
