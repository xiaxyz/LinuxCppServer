#include "XBuffer.hpp"
#include "XConnection.hpp"
#include "XEventLoop.hpp"
#include "XSocket.hpp"
#include "XTCPServer.hpp"

int main(int argc, char const *argv[])
{
	auto server_ = std::make_unique<XTCPServer>();

	server_->OnConnect([](std::shared_ptr<XConnection> _connection)
	{
		std::cout << std::format("New connection fd: {}", _connection->GetSocket()->GetFd()) << std::endl;
	});

	server_->OnReceive([](std::shared_ptr<XConnection> _connection)
	{
		std::cout << std::format("Message from client {} : {}", _connection->GetSocket()->GetFd(), _connection->GetReadBuffer()->Data()) << std::endl;
		_connection->Send(_connection->GetReadBuffer()->CharData());
	});

	server_->Start();

	return 0;
}
