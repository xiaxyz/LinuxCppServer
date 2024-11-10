#include "XAcceptor.hpp"

#include "XChannel.hpp"
#include "XInternetAddress.hpp"
#include "XSocket.hpp"

#include "XUtility.hpp"

XAcceptor::XAcceptor(std::shared_ptr<XEventLoop> _event_loop, std::shared_ptr<XInternetAddress> _internet_address)
	: event_loop(_event_loop),
	  socket(std::make_shared<XSocket>())
{
	ErrorIfFile(_internet_address == nullptr, "internet address is null");
	internet_address = _internet_address;
	socket->Bind(internet_address);
	socket->Listen();
	accept_channel = std::make_shared<XChannel>(socket, event_loop);
	auto callback_ = std::function<void()>(std::bind(&XAcceptor::AcceptConnection, this));
	accept_channel->SetReadCallback(callback_);
	accept_channel->EnableRead();
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
	client_socket_->SetNonblocking();
	new_connection_callback(client_socket_);
}

void XAcceptor::SetNewConnectionCallback(std::function<void(std::shared_ptr<XSocket>)> _new_connection_callback)
{
	new_connection_callback = _new_connection_callback;
}
