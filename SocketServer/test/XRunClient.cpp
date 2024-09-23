#include <iostream>
#include <memory>

#include "XBuffer.hpp"
#include "XConnection.hpp"
#include "XInternetAddress.hpp"
#include "XSocket.hpp"
#include "XUtility.hpp"

int main(int argc, char const *argv[])
{
	auto client_socket_ = std::make_shared<XSocket>();
	auto server_address_ = std::make_shared<XInternetAddress>("127.0.0.1", 6666);
	client_socket_->Connect(server_address_);

	auto connection_ = std::make_shared<XConnection>(client_socket_, nullptr);

	while(true)
	{
		std::cin >> connection_->GetSendBuffer()->Data();
		connection_->Write();
		if(connection_->GetState() == XConnection::State::Closed)
		{
			connection_->Close();
			break;
		}
		connection_->Read();
		std::cout << std::format("Message from server: {}", connection_->GetReadBuffer()->Data()) << std::endl;
	}

	return 0;
}
