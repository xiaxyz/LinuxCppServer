#include "XConnection.hpp"

#include "XBuffer.hpp"
#include "XChannel.hpp"
#include "XSocket.hpp"

#include "XUtility.hpp"

XConnection::XConnection(std::shared_ptr<XSocket> _socket, std::shared_ptr<XEventLoop> _event_loop)
	: socket(_socket),
	  event_loop(_event_loop),
	  state(State::Connected)
{
	if(_event_loop != nullptr)
	{
		channel = std::make_shared<XChannel>(socket, event_loop);
		channel->EnableRead();
		channel->EnableET();
	}
	read_buffer = std::make_shared<XBuffer>();
	send_buffer = std::make_shared<XBuffer>();
}

XConnection::~XConnection()
{
}

void XConnection::Read()
{
	assert(((void)"connection state is disconnected!", state == State::Connected));
	read_buffer->Clear();
	if(socket->IsNonblocking())
	{
		ReadNonblocking();
	} else
	{
		ReadBlocking();
	}
}

void XConnection::ReadBlocking()
{
	auto receive_size_ = (unsigned int)(0);
	auto socket_len_ = socklen_t(sizeof(receive_size_));
	getsockopt(socket->GetFd(), SOL_SOCKET, SO_RCVBUF, &receive_size_, &socket_len_);
	char buffer_[receive_size_];
	auto bytes_read_ = read(socket->GetFd(), buffer_, sizeof(buffer_));
	if(bytes_read_ > 0)
	{
		read_buffer->Append(buffer_, bytes_read_);
	} else if(bytes_read_ == 0)
	{
		std::cout << std::format("read EOF, blocking client fd {} disconnected", socket->GetFd()) << std::endl;
		state = State::Closed;
	} else if(bytes_read_ == -1)
	{
		std::cout << std::format("Other error on blocking client fd {}", socket->GetFd()) << std::endl;
		state = State::Closed;
	}
}

void XConnection::ReadNonblocking()
{
	char buffer_[READ_BUFFER];
	while(true)
	{
		memset(buffer_, 0, sizeof(buffer_));
		auto bytes_read_ = read(socket->GetFd(), buffer_, sizeof(buffer_));
		if(bytes_read_ > 0)
		{
			read_buffer->Append(buffer_, bytes_read_);
		} else if(bytes_read_ == -1 && errno == EINTR)
		{
			std::cout << std::format("continue reading") << std::endl;
			continue;
		} else if(bytes_read_ == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
		{
			break;
		} else if(bytes_read_ == 0)
		{
			std::cout << std::format("EOF, client fd {} disconnected", socket->GetFd()) << std::endl;
			delete_connection(socket);
			break;
		} else
		{
			std::cout << std::format("other error on client fd {}", socket->GetFd()) << std::endl;
			state = State::Closed;
			break;
		}
	}
}

void XConnection::Write()
{
	assert(((void)"connection state is disconnected!", state == State::Connected));
	if(socket->IsNonblocking())
	{
		WriteNonblocking();
	} else
	{
		WriteBlocking();
	}
}

void XConnection::WriteBlocking()
{
	auto send_size_ = send_buffer->Size();
	while(send_size_ > 0)
	{
		auto bytes_write_ = write(socket->GetFd(), send_buffer->CharData(), send_size_);
		if(bytes_write_ == -1 && errno == EINTR)
		{
			std::cout << std::format("continue writing") << std::endl;
		} else if(bytes_write_ == -1 && errno == EAGAIN)
		{
			break;
		} else if(bytes_write_ == -1)
		{
			std::cout << std::format("other error on client fd {}", socket->GetFd()) << std::endl;
			state = State::Closed;
			break;
		}
		send_size_ -= bytes_write_;
	}
}

void XConnection::WriteNonblocking()
{
	auto send_size_ = send_buffer->Size();
	while(send_size_ > 0)
	{
		auto bytes_write_ = write(socket->GetFd(), send_buffer->CharData(), send_size_);
		if(bytes_write_ == -1 && errno == EINTR)
		{
			std::cout << std::format("continue writing") << std::endl;
		} else if(bytes_write_ == -1 && errno == EAGAIN)
		{
			break;
		} else if(bytes_write_ == -1)
		{
			std::cout << std::format("other error on client fd {}", socket->GetFd()) << std::endl;
			state = State::Closed;
			break;
		}
		send_size_ -= bytes_write_;
	}
}

void XConnection::Send(const char *_message)
{
	SetSendBuffer(_message);
	Write();
}

void XConnection::Business()
{
	Read();
	on_receive(shared_from_this());
}

void XConnection::Close()
{
	delete_connection(socket);
}

XConnection::State XConnection::GetState()
{
	return state;
}

void XConnection::SetDeleteConnection(const std::function<void(std::shared_ptr<XSocket>)> &_delete_connection)
{
	delete_connection = _delete_connection;
}

void XConnection::SetOnReceive(const std::function<void(std::shared_ptr<XConnection>)> &_on_receive)
{
	on_receive = _on_receive;
	auto business = std::function<void()>(std::bind(&XConnection::Business, this));
	channel->SetReadCallback(business);
}

std::shared_ptr<XSocket> XConnection::GetSocket()
{
	return socket;
}

void XConnection::SetReadBuffer(const char *_read_buffer)
{
	read_buffer->SetData(_read_buffer);
}

std::shared_ptr<XBuffer> XConnection::GetReadBuffer()
{
	return read_buffer;
}

void XConnection::SetSendBuffer(const char *_send_buffer)
{
	send_buffer->SetData(_send_buffer);
}

std::shared_ptr<XBuffer> XConnection::GetSendBuffer()
{
	return send_buffer;
}
