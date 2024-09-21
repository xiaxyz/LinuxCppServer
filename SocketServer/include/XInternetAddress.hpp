#ifndef XInternetAddress_hpp
#define XInternetAddress_hpp

#include <cstring>

#include <memory>

#include <arpa/inet.h>

class XInternetAddress
{
private:
	std::shared_ptr<sockaddr_in> socket_address;
	std::shared_ptr<socklen_t> socket_length;

public:
	XInternetAddress();
	XInternetAddress(const char *_ip, uint16_t _port);
	~XInternetAddress();

	std::shared_ptr<sockaddr_in> GetSocketAddress();
	std::shared_ptr<socklen_t> GetSocketLength();
};

#endif // XInternetAddress_hpp
