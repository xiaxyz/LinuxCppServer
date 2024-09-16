#include "XServer.hpp"
#include "XAcceptor.hpp"
#include "XChannel.hpp"
#include "XConnection.hpp"
#include "XEpoll.hpp"
#include "XSocket.hpp"

XServer::XServer(std::shared_ptr<XEventLoop> _event_loop)
	: event_loop(_event_loop)
{
	acceptor = std::make_unique<XAcceptor>(event_loop);
	auto callback = std::bind(&XServer::NewConnection, this, std::placeholders::_1);
	acceptor->SetNewConnectionCallback(callback);
}

XServer::~XServer()
{
}

void XServer::NewConnection(std::shared_ptr<XSocket> _socket)
{
	auto connection_ = std::make_unique<XConnection>(event_loop, _socket);
	auto callback_ = std::bind(&XServer::DeleteConnection, this, std::placeholders::_1);
	connection_->SetDeleteConnectionCallback(callback_);
	connections[_socket->GetFd()] = std::move(connection_);
}

void XServer::DeleteConnection(std::shared_ptr<XSocket> _socket)
{
	connections.erase(_socket->GetFd());
}
