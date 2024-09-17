#ifndef XServer_hpp
#define XServer_hpp

#include <cstring>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>
#include <vector>

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

public:
	XServer(std::shared_ptr<XEventLoop> _event_loop);
	~XServer();

	void NewConnection(std::shared_ptr<XSocket> _socket);
	void DeleteConnection(std::shared_ptr<XSocket> _socket);
};

#endif // XServer_hpp
