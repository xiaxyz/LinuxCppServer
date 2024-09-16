#include "XConnection.hpp"
#include "XBuffer.hpp"
#include "XChannel.hpp"
#include "XSocket.hpp"
#include "XUtility.hpp"

XConnection::XConnection(std::shared_ptr<XEventLoop> _event_loop, std::shared_ptr<XSocket> _socket)
	: event_loop(_event_loop),
	  socket(_socket)
{
	channel = std::make_shared<XChannel>(event_loop, socket);
	auto callback = std::bind(&XConnection::Echo, this);
	channel->SetCallback(callback);
	channel->EnableReading();
	read_buffer = std::make_unique<XBuffer>();
}

XConnection::~XConnection()
{
}

void XConnection::Echo()
{
	char buffer_[READ_BUFFER];
	while(true)
	{
		memset(buffer_, 0, sizeof(buffer_));
		ssize_t bytes_read = read(socket->GetFd(), buffer_, sizeof(buffer_));
		if(bytes_read > 0)
		{
			read_buffer->Append(buffer_, bytes_read);
		}
		else if(bytes_read == -1 && errno == EINTR)
		{
			std::cout << std::format("continue reading") << std::endl;
			continue;
		}
		else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
		{
			std::cout << std::format("finish reading once") << std::endl;
			std::cout << std::format("message from client fd {}: {}", socket->GetFd(), read_buffer->Data()) << std::endl;
			ErrorIfFile(write(socket->GetFd(), read_buffer->Data(), read_buffer->Size()) == -1, "socket write error");
			read_buffer->Clear();
			break;
		}
		else if(bytes_read == 0)
		{
			std::cout << std::format("EOF, client fd {} disconnected", socket->GetFd()) << std::endl;
			delete_connection_callback(socket);
			break;
		}
	}
}

void XConnection::SetDeleteConnectionCallback(std::function<void(std::shared_ptr<XSocket>)> _delete_connection_callback)
{
	delete_connection_callback = _delete_connection_callback;
}
