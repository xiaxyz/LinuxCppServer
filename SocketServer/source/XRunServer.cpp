#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <format>

#include "XSocket.hpp"
#include "XInternetAddress.hpp"
#include "XEpoll.hpp"
#include "XChannel.hpp"
#include "XUtility.hpp"

#define MAX_EVENTS 1024
#define MAX_BUFFER 1024

void HandleReadEvent(int _fd);

int main(int argc, char const *argv[])
{
    auto socket_server = XSocket();
    auto internet_address = XInternetAddress("127.0.0.1", 6666);
    ErrorIf(socket_server.Bind(internet_address) == -1, "socket bind error");
    ErrorIf(socket_server.Listen() == -1, "socket listen error");
    auto epoll = XEpoll();
    auto channel = XChannel(&epoll, &socket_server);
    ErrorIf(channel.EnableReading() == -1, "epoll set event error");
    while (true)
    {
        auto trigger_events = epoll.TriggeredEvents();
        for (auto i_event : trigger_events)
        {
            if (i_event->GetXSocket()->GetFd() == socket_server.GetFd())
            {
                auto client_address = XInternetAddress();
                auto client_socket = socket_server.Accept(client_address);
                if (!ErrorIf(client_socket.GetFd() == -1, "socket accept error"))
                {
                    std::cout << std::format("new connect IP: {}, port: {}", inet_ntoa(client_address.GetAddress().sin_addr), ntohs(client_address.GetAddress().sin_port)) << std::endl;
                }
                client_socket.SetNonBlocking();
                auto client_channel = XChannel(&epoll, &client_socket);
                ErrorIf(client_channel.EnableReading() == -1, "epoll set event error");
            }
            else if (i_event->GetRevents() & EPOLLIN)
            {
                HandleReadEvent(i_event->GetXSocket()->GetFd());
            }
            else
            {
                std::cout << std::format("event {} happened", i_event->GetRevents()) << std::endl;
            }
        }
    }

    return 0;
}

void HandleReadEvent(int _fd)
{
    char buffer[MAX_BUFFER];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        ssize_t byte_read = read(_fd, buffer, sizeof(buffer));
        if (byte_read > 0)
        {
            std::cout << std::format("message from client fd {}: {}", _fd, buffer) << std::endl;
            write(_fd, buffer, sizeof(buffer));
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
            std::cout << std::format("EOF, client fd {} disconnected", _fd) << std::endl;
            close(_fd);
            break;
        }
    }
}
