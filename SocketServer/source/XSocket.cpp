#include "XSocket.hpp"
#include "XUtility.hpp"

XSocket::XSocket()
	: fd(-1)
{
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

XSocket::XSocket(int _fd)
	: fd(_fd)
{
}

XSocket::~XSocket()
{
	if(fd != -1)
	{
		close(fd);
	}
}

void XSocket::Bind(std::shared_ptr<XInternetAddress> _address)
{
	ErrorIfFile(bind(fd, (sockaddr *)_address->GetSocketAddress().get(), *_address->GetSocketLength()) == -1, "socket bind error");
}

void XSocket::Listen(int _length)
{
	ErrorIfFile(listen(fd, _length), "socket listen error");
}

void XSocket::SetNonBlocking()
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

std::shared_ptr<XSocket> XSocket::Accept(std::shared_ptr<XInternetAddress> _address)
{
	auto client_ = accept(fd, (sockaddr *)_address->GetSocketAddress().get(), _address->GetSocketLength().get());
	ErrorIfFile(client_ == -1, "socket accept error");
	return std::make_shared<XSocket>(client_);
}

void XSocket::Close()
{
	close(fd);
}

void XSocket::Connect(std::shared_ptr<XInternetAddress> _address)
{
	if(fcntl(fd, F_GETFL) & O_NONBLOCK)
	{
		while(true)
		{
			auto result = connect(fd, (sockaddr *)_address->GetSocketAddress().get(), *_address->GetSocketLength());
			if(result == 0)
			{
				return;
			} else if(result == -1 && (errno == EINPROGRESS))
			{
				continue;
			} else if (result == -1)
			{
				ErrorIfFile(true, "socket connect error");
			}
		}
	} else
	{
		ErrorIfFile(connect(fd, (sockaddr *)_address->GetSocketAddress().get(), *_address->GetSocketLength()) == -1, "socket connect error");
	}
}

int XSocket::GetFd()
{
	return fd;
}

void XSocket::SetFd(int _fd)
{
	fd = _fd;
}
