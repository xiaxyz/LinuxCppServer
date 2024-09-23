#include "XTCPServer.hpp"

#include "XAcceptor.hpp"
#include "XConnection.hpp"
#include "XEventLoop.hpp"
#include "XSocket.hpp"
#include "XThreadPool.hpp"

#include "XUtility.hpp"

XTCPServer::XTCPServer()
	: main_reactor(std::make_shared<XEventLoop>())
{
	acceptor = std::make_unique<XAcceptor>(main_reactor);
	auto callback = std::function<void(std::shared_ptr<XSocket>)>(std::bind(&XTCPServer::NewConnection, this, std::placeholders::_1));
	acceptor->SetNewConnectionCallback(callback);

	auto size_ = std::thread::hardware_concurrency();
	thread_pool = std::make_unique<XThreadPool>(size_);
	sub_reactors.reserve(size_);

	for(auto i = 0; i < size_; ++i)
	{
		auto sub_reactor_ = std::make_shared<XEventLoop>();
		sub_reactors.emplace_back(sub_reactor_);
	}
}

XTCPServer::~XTCPServer()
{
}

void XTCPServer::Start()
{
	for(const auto &i_sub_reactor : sub_reactors)
	{
		auto sub_loop = std::function<void()>(std::bind(&XEventLoop::Loop, i_sub_reactor.get()));
		thread_pool->Add(sub_loop);
	}
	main_reactor->Loop();
}

Result XTCPServer::NewConnection(std::shared_ptr<XSocket> _socket)
{
	assert(_socket->GetFd() != -1);
	auto random_ = _socket->GetFd() % sub_reactors.size();

	auto connection_ = std::make_shared<XConnection>(_socket, sub_reactors[random_]);
	auto callback_ = std::function<void(std::shared_ptr<XSocket>)>(std::bind(&XTCPServer::DeleteConnection, this, std::placeholders::_1));

	connection_->SetDeleteConnection(callback_);
	connection_->SetOnReceive(on_receive);

	connections[_socket->GetFd()] = connection_;
	if(on_connect)
	{
		on_connect(connections[_socket->GetFd()]);
	}
	return Result::success;
}

Result XTCPServer::DeleteConnection(std::shared_ptr<XSocket> _socket)
{
	auto connection_ = connections.find(_socket->GetFd());
	assert(connection_ != connections.end());
	connections.erase(connection_);
	return Result::success;
}

void XTCPServer::OnConnect(std::function<void(std::shared_ptr<XConnection>)> _on_connect)
{
	on_connect = _on_connect;
}

void XTCPServer::OnReceive(std::function<void(std::shared_ptr<XConnection>)> _on_receive)
{
	on_receive = _on_receive;
}
