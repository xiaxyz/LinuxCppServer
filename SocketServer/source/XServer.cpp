#include "XServer.hpp"
#include "XAcceptor.hpp"
#include "XSocket.hpp"
#include "XEpoll.hpp"
#include "XChannel.hpp"
#include "XConnection.hpp"

XServer::XServer(XEventLoop *_event_loop) : event_loop(_event_loop)
{
    acceptor = new XAcceptor(event_loop);
    auto callback = std::bind(&XServer::NewConnection, this, std::placeholders::_1);
    acceptor->SetNewConnectionCallback(callback);
}

XServer::~XServer()
{
    delete acceptor;
    for (auto i_channel : channels)
    {
        delete i_channel;
    }
}

void XServer::NewConnection(XSocket *_socket)
{
    auto connection_ = new XConnection(event_loop, _socket);
    auto callback_ = std::bind(&XServer::DeleteConnection, this, std::placeholders::_1);
    connection_->SetDeleteConnectionCallback(callback_);
    connections[_socket] = connection_;
}

void XServer::DeleteConnection(XSocket *_socket)
{
    auto connection_ = connections[_socket];
    connections.erase(_socket);
    delete connection_;
    delete _socket;
}
