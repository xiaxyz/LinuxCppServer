#include "XServer.hpp"
#include "XAcceptor.hpp"
#include "XChannel.hpp"
#include "XConnection.hpp"
#include "XEpoll.hpp"
#include "XEventLoop.hpp"
#include "XSocket.hpp"
#include "XThreadPool.hpp"

XServer::XServer(std::shared_ptr<XEventLoop> _event_loop)
	: main_reactor(_event_loop)
{
	acceptor = std::make_unique<XAcceptor>(main_reactor);
	auto callback = std::function<void(std::shared_ptr<XSocket>)>(std::bind(&XServer::NewConnection, this, std::placeholders::_1));
	acceptor->SetNewConnectionCallback(callback);

	auto size_ = std::thread::hardware_concurrency();
	thread_pool = std::make_unique<XThreadPool>(size_);
	for(auto i = 0; i < size_; ++i)
	{
		sub_reactors.push_back(std::make_shared<XEventLoop>());
	}
	for(const auto &i_sub_reactors : sub_reactors)
	{
		auto sub_loop = std::function<void()>(std::bind(&XEventLoop::Loop, i_sub_reactors));
		thread_pool->Add(sub_loop);
	}
}

XServer::~XServer()
{
}

void XServer::NewConnection(std::shared_ptr<XSocket> _socket)
{
	if(_socket->GetFd() != -1)
	{
		auto mark_ = _socket->GetFd() % sub_reactors.size();
		auto connection_ = std::make_shared<XConnection>(sub_reactors[mark_], _socket);
		auto callback_ = std::function<void(std::shared_ptr<XSocket>)>(std::bind(&XServer::DeleteConnection, this, std::placeholders::_1));
		connection_->SetDeleteConnectionCallback(callback_);
		connections[_socket->GetFd()] = connection_;
	}
}

void XServer::DeleteConnection(std::shared_ptr<XSocket> _socket)
{
	connections.erase(_socket->GetFd());
}
