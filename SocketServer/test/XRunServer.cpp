#include "XEventLoop.hpp"
#include "XServer.hpp"
#include "XSocket.hpp"
#include "XConnection.hpp"
#include "XBuffer.hpp"

int main(int argc, char const *argv[])
{
	auto loop_ = std::make_shared<XEventLoop>();
	auto server_ = std::make_shared<XServer>(loop_);
	server_->OnConnect([](std::shared_ptr<XConnection> _connection)
	{
		_connection->Read();
		if(_connection->GetState() == XConnection::State::Closed)
		{
			_connection->Close();
			return;
		}
		std::cout << std::format("Message from client {} : {}", _connection->GetSocket()->GetFd(), _connection->GetReadBuffer()->Data()) << std::endl;
		_connection->SetSendBuffer(_connection->GetReadBuffer()->CharData());
		_connection->Write();
	});
	loop_->Loop();
	return 0;
}
