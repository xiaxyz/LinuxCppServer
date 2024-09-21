#include "XBuffer.hpp"
#include "XConnection.hpp"
#include "XEventLoop.hpp"
#include "XServer.hpp"
#include "XSocket.hpp"

int main(int argc, char const *argv[])
{
	auto loop_ = std::make_shared<XEventLoop>();
	auto server_ = std::make_shared<XServer>(loop_);

	server_->NewConnect([](std::shared_ptr<XConnection> _connection)
	{
		std::cout << std::format("New connection fd: {}", _connection->GetSocket()->GetFd()) << std::endl;
	});

	server_->OnMessage([](std::shared_ptr<XConnection> _connection)
	{
		std::cout << std::format("Message from client {} : {}", _connection->GetSocket()->GetFd(), _connection->GetReadBuffer()->Data()) << std::endl;
		if(_connection->GetState() == XConnection::State::Connected)
		{
			_connection->Send(_connection->GetReadBuffer()->CharData());
		}
	});

	loop_->Loop();

	return 0;
}
