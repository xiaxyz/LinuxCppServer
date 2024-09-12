#include "XAcceptor.hpp"
#include "XSocket.hpp"
#include "XInternetAddress.hpp"
#include "XChannel.hpp"

XAcceptor::XAcceptor(XEventLoop *_event_loop) : event_loop(_event_loop)
{
    socket = new XSocket();
    internet_address = new XInternetAddress("127.0.0.1", 6666);
    socket->Bind(internet_address);
    socket->Listen();
    socket->SetNonBlocking();
    accept_channel = new XChannel(event_loop, socket);
    auto callback_ = std::bind(&XAcceptor::AcceptConnection, this);
    accept_channel->SetCallback(callback_);
    accept_channel->EnableReading();
}

XAcceptor::~XAcceptor()
{
    delete socket;
    delete internet_address;
    delete accept_channel;
}

void XAcceptor::AcceptConnection()
{
    auto client_address_ = XInternetAddress();
    auto client_socket_ = new XSocket(socket->Accept(&client_address_));
    if (client_socket_->GetFd() != -1)
    {
        std::cout << std::format("new connect IP: {}, port: {}", inet_ntoa(client_address_.SocketAddress().sin_addr), ntohs(client_address_.SocketAddress().sin_port)) << std::endl;
    }
    client_socket_->SetNonBlocking();
    new_connection_callback(client_socket_);
}

void XAcceptor::SetNewConnectionCallback(std::function<void(XSocket *)> _new_connection_callback)
{
    new_connection_callback = _new_connection_callback;
}
