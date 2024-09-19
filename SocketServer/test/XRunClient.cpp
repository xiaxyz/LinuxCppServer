#include <arpa/inet.h>
#include <cstring>
#include <format>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

bool ErrorIfFile(bool _condition, const char *_error_message);

int main(int argc, char const *argv[])
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in serve_address;
	memset(&serve_address, 0, sizeof(serve_address));

	serve_address.sin_family = AF_INET;
	serve_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	serve_address.sin_port = htons(6666);

	if(!ErrorIfFile(connect(socket_fd, (sockaddr *)&serve_address, sizeof(serve_address)) == -1, "socket connect error"))
	{
		std::cout << std::format("success connect IP: {}, port: {}", inet_ntoa(serve_address.sin_addr), ntohs(serve_address.sin_port)) << std::endl;
	}

	while(true)
	{
		char buf[1024]; // 定义缓冲区
		bzero(&buf, sizeof(buf)); // 清空缓冲区
		scanf("%s", buf); // 从键盘输入要传到服务器的数据
		ssize_t write_bytes = write(socket_fd, buf, sizeof(buf)); // 发送缓冲区中的数据到服务器socket，返回已发送数据大小
		if(write_bytes == -1) // write返回-1，表示发生错误
		{
			printf("socket already disconnected, can't write any more!\n");
			break;
		}
		bzero(&buf, sizeof(buf)); // 清空缓冲区
		ssize_t read_bytes = read(socket_fd, buf, sizeof(buf)); // 从服务器socket读到缓冲区，返回已读数据大小
		if(read_bytes > 0)
		{
			printf("message from server: %s\n", buf);
		}
		else if(read_bytes == 0) // read返回0，表示EOF，通常是服务器断开链接，等会儿进行测试
		{
			printf("server socket disconnected!\n");
			break;
		}
		else if(read_bytes == -1) // read返回-1，表示发生错误，按照上文方法进行错误处理
		{
			close(socket_fd);
			ErrorIfFile(true, "socket read error");
		}
	}

	return 0;
}

bool ErrorIfFile(bool _condition, const char *_error_message)
{
	if(_condition)
	{
		std::cerr << _error_message << std::endl;
		return true;
	}
	return false;
}
