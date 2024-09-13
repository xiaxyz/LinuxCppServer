#ifndef XConnection_hpp
#define XConnection_hpp

#include <functional>
#include <format>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <memory>

#define READ_BUFFER 1024

class XEventLoop;
class XSocket;
class XChannel;

class XConnection
{
private:
    std::shared_ptr<XEventLoop> event_loop;
    std::shared_ptr<XSocket> socket;
    std::shared_ptr<XChannel> channel;
    std::function<void(std::shared_ptr<XSocket>)> delete_connection_callback;
public:
    XConnection(std::shared_ptr<XEventLoop> _event_loop, std::shared_ptr<XSocket> _socket);
    ~XConnection();

    void Echo();
    void SetDeleteConnectionCallback(std::function<void(std::shared_ptr<XSocket>)> _delete_connection_callback);
};

#endif // XConnection_hpp
