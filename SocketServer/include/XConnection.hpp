#ifndef XConnection_hpp
#define XConnection_hpp

#include <functional>
#include <format>
#include <iostream>
#include <cstring>
#include <unistd.h>

#define READ_BUFFER 1024

class XEventLoop;
class XSocket;
class XChannel;

class XConnection
{
private:
    XEventLoop *event_loop;
    XSocket *socket;
    XChannel *channel;
    std::function<void(XSocket *)> delete_connection_callback;
public:
    XConnection(XEventLoop *_event_loop, XSocket *_socket);
    ~XConnection();

    void Echo();
    void SetDeleteConnectionCallback(std::function<void(XSocket *)> _delete_connection_callback);
};

#endif // XConnection_hpp
