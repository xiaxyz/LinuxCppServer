#include "XServer.hpp"
#include "XAcceptor.hpp"
#include "XSocket.hpp"
#include "XEpoll.hpp"
#include "XChannel.hpp"

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

void XServer::NewConnection(XSocket *_server_socket)
{
    auto client_address_ = XInternetAddress();
    auto client_socket_ = new XSocket(_server_socket->Accept(&client_address_));
    socket.push_back(client_socket_);
    if (client_socket_->GetFd() != -1)
    {
        std::cout << std::format("new connect IP: {}, port: {}", inet_ntoa(client_address_.SocketAddress().sin_addr), ntohs(client_address_.SocketAddress().sin_port)) << std::endl;
    }
    client_socket_->SetNonBlocking();
    auto client_channel_ = new XChannel(event_loop, client_socket_);
    channels.push_back(client_channel_);
    auto callback_ = std::bind(&XServer::HandleReadEvent, this, client_socket_);
    client_channel_->SetCallback(callback_);
    client_channel_->EnableReading();
}

void XServer::HandleReadEvent(XSocket *_socket)
{
    char buffer_[MAX_BUFFER];
    while (true)
    {
        memset(buffer_, 0, sizeof(buffer_));
        ssize_t byte_read = read(_socket->GetFd(), buffer_, sizeof(buffer_));
        if (byte_read > 0)
        {
            std::cout << std::format("message from client fd {}: {}", _socket->GetFd(), buffer_) << std::endl;
            write(_socket->GetFd(), buffer_, sizeof(buffer_));
        }
        else if (byte_read == -1 && errno == EINTR)
        {
            std::cout << std::format("continue reading") << std::endl;
            continue;
        }
        else if (byte_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        {
            std::cout << std::format("finish reading once, errno: {}", errno) << std::endl;
            break;
        }
        else if (byte_read == 0)
        {
            std::cout << std::format("EOF, client fd {} disconnected", _socket->GetFd()) << std::endl;
            close(_socket->GetFd());
            break;
        }
    }
}
