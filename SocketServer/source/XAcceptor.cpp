#include "XAcceptor.hpp"
#include "XChannel.hpp"
#include "XInternetAddress.hpp"
#include "XSocket.hpp"

XAcceptor::XAcceptor(std::shared_ptr<XEventLoop> _event_loop)
	: event_loop(_event_loop)
{
	socket = std::make_shared<XSocket>();
	internet_address = std::make_shared<XInternetAddress>("127.0.0.1", 6666);
	socket->Bind(internet_address);
	socket->Listen();
	socket->SetNonBlocking();
	accept_channel = std::make_shared<XChannel>(event_loop, socket);
	auto callback_ = std::bind(&XAcceptor::AcceptConnection, this);
	accept_channel->SetCallback(callback_);
	accept_channel->EnableReading();
}

XAcceptor::~XAcceptor()
{
}

void XAcceptor::AcceptConnection()
{
	auto client_address_ = std::make_shared<XInternetAddress>();
	auto client_socket_ = socket->Accept(client_address_);
	if(client_socket_->GetFd() != -1)
	{
		std::cout << std::format("new connect IP: {}, port: {}", inet_ntoa(client_address_->GetSocketAddress()->sin_addr), ntohs(client_address_->GetSocketAddress()->sin_port)) << std::endl;
	}
	client_socket_->SetNonBlocking();
	new_connection_callback(client_socket_);
}

void XAcceptor::SetNewConnectionCallback(std::function<void(std::shared_ptr<XSocket>)> _new_connection_callback)
{
	new_connection_callback = _new_connection_callback;
}
