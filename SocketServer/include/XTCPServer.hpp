#ifndef XTCPServer_hpp
#define XTCPServer_hpp

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

class XEventLoop;
class XAcceptor;
class XConnection;
class XThreadPool;
class XSocket;

enum Result : unsigned int;

class XTCPServer
{
private:
	std::shared_ptr<XEventLoop> main_reactor;
	std::unique_ptr<XAcceptor> acceptor;
	std::unordered_map<int, std::shared_ptr<XConnection>> connections;
	std::vector<std::shared_ptr<XEventLoop>> sub_reactors;
	std::unique_ptr<XThreadPool> thread_pool;
	std::function<void(std::shared_ptr<XConnection>)> on_connect;
	std::function<void(std::shared_ptr<XConnection>)> on_receive;

public:
	XTCPServer(const XTCPServer &_other) = delete;
	XTCPServer &operator=(const XTCPServer &_other) = delete;
	XTCPServer(const XTCPServer &&_other) = delete;
	XTCPServer &operator=(const XTCPServer &&_other) = delete;
	XTCPServer();
	~XTCPServer();

	void Start();

	Result NewConnection(std::shared_ptr<XSocket> _socket);
	Result DeleteConnection(std::shared_ptr<XSocket> _socket);

	void OnConnect(std::function<void(std::shared_ptr<XConnection>)> _on_connect);
	void OnReceive(std::function<void(std::shared_ptr<XConnection>)> _on_receive);
};

#endif // XTCPServer_hpp
