#ifndef XServer_hpp
#define XServer_hpp

#include <cstring>
#include <unistd.h>
#include <iostream>
#include <format>
#include <vector>

#define MAX_EVENTS 1024
#define MAX_BUFFER 1024

class XEventLoop;
class XChannel;
class XSocket;

class XServer
{
private:
    XEventLoop *event_loop;
    std::vector<XSocket *> socket;
    std::vector<XChannel *> channels;
public:
    XServer(XEventLoop *_event_loop);
    ~XServer();

    void NewConnection(XSocket *_server_socket);
    void HandleReadEvent(XSocket *_socket);
};



#endif // XServer_hpp
