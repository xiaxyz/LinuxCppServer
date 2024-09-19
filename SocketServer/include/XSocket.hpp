#ifndef XSocket_hpp
#define XSocket_hpp

#include <compare>
#include <fcntl.h>
#include <memory>
#include <sys/socket.h>
#include <unistd.h>

#include "XInternetAddress.hpp"

class XInternetAddress;

class XSocket
{
private:
	int fd;

public:
	XSocket();
	XSocket(int _fd);
	~XSocket();

	void Bind(std::shared_ptr<XInternetAddress> _address);
	void Listen(int _length = SOMAXCONN);

	void SetNonBlocking();

	std::shared_ptr<XSocket> Accept(std::shared_ptr<XInternetAddress> _address);

	void Close();

	void Connect(std::shared_ptr<XInternetAddress> _address);

	int GetFd();
	void SetFd(int _fd);
};

#endif // XSocket_hpp
