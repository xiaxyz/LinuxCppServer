#include "XInternetAddress.hpp"

XInternetAddress::XInternetAddress()
	: socket_address(std::make_shared<sockaddr_in>())
{
	socket_length = std::make_shared<socklen_t>(sizeof(*socket_address));
	memset(socket_address.get(), 0, sizeof(*socket_address));
}

XInternetAddress::XInternetAddress(const char *_ip, uint16_t _port)
{
	socket_address = std::make_shared<sockaddr_in>();
	socket_length = std::make_shared<socklen_t>(sizeof(*socket_address));
	memset(socket_address.get(), 0, sizeof(*socket_address));
	socket_address->sin_family = AF_INET;
	socket_address->sin_addr.s_addr = inet_addr(_ip);
	socket_address->sin_port = htons(_port);
}

XInternetAddress::~XInternetAddress()
{
}

std::shared_ptr<sockaddr_in> XInternetAddress::GetSocketAddress()
{
	return socket_address;
}

std::shared_ptr<socklen_t> XInternetAddress::GetSocketLength()
{
	*socket_length = sizeof(*socket_address);
	return socket_length;
}
