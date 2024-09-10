#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <format>

#include "XSocket.hpp"
#include "XInternetAddress.hpp"
#include "XEpoll.hpp"
#include "XUtility.hpp"

#define MAX_EVENTS 1024
#define MAX_BUFFER 1024


int main(int argc, char const *argv[])
{
    auto socket_server = XSocket();
    auto internet_address = XInternetAddress("127.0.0.1", 6666);
    ErrorIf(socket_server.Bind(internet_address) == -1, "socket bind error");
    ErrorIf(socket_server.Listen() == -1, "socket listen error");
    auto epoll = XEpoll();
    ErrorIf(epoll.AddFd(socket_server.GetFd(), EPOLLIN | EPOLLET) == -1, "epoll add event error");
    while (true)
    {
        auto trigger_events = epoll.TriggeredEvents();

        for (auto i_event : trigger_events)
        {
            if (i_event.data.fd == socket_server.GetFd())
            {
                auto client_address = XInternetAddress();
                auto client_socket = socket_server.Accept(client_address);
                if (!ErrorIf(client_socket.GetFd() == -1, "socket accept error"))
                {
                    std::cout << std::format("new connect IP: {}, port: {}", inet_ntoa(client_address.GetAddress().sin_addr), ntohs(client_address.GetAddress().sin_port)) << std::endl;
                }
                client_socket.SetNonBlocking();
                ErrorIf(epoll.AddFd(client_socket.GetFd(), EPOLLIN | EPOLLET) == -1, "epoll add event error");
            }
            else if (i_event.events & EPOLLIN)
            {
                char buffer[MAX_BUFFER];
                while (true)
                {
                    memset(buffer, 0, sizeof(buffer));
                    ssize_t byte_read = read(i_event.data.fd, buffer, sizeof(buffer));
                    if (byte_read > 0)
                    {
                        std::cout << std::format("message from client fd {}: {}", (int)i_event.data.fd, buffer) << std::endl;
                        write(i_event.data.fd, buffer, sizeof(buffer));
                    }
                    else if (byte_read == -1 && errno == EINTR)
                    {
                        std::cout << std::format("continue reading") << std::endl;
                        continue;
                    }
                    else if (byte_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
                    {
                        std::cout << std::format("finish reading once, errno: {}", errno) << std::endl;
                        break;
                    }
                    else if (byte_read == 0)
                    {
                        std::cout << std::format("EOF, client fd {} disconnected", (int)i_event.data.fd) << std::endl;
                        close(i_event.data.fd);
                        break;
                    }
                }
            }
            else
            {
                std::cout << std::format("event {} happened", (uint32_t)i_event.events) << std::endl;
            }
        }
    }

    return 0;
}
