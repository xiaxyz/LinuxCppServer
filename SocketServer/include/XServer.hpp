#ifndef XServer_hpp
#define XServer_hpp

#include <cstring>
#include <unistd.h>
#include <iostream>
#include <format>
#include <vector>
#include <map>

// #define MAX_EVENTS 1024
// #define MAX_BUFFER 1024

class XEventLoop;
class XAcceptor;
class XChannel;
class XSocket;
class XConnection;

class XServer
{
private:
    XEventLoop *event_loop;
    XAcceptor *acceptor;
    std::map<XSocket *, XConnection *> connections;
    std::vector<XSocket *> socket;
    std::vector<XChannel *> channels;
public:
    XServer(XEventLoop *_event_loop);
    ~XServer();

    void NewConnection(XSocket *_socket);
    void DeleteConnection(XSocket *_socket);
    // void HandleReadEvent(XSocket *_socket);
};



#endif // XServer_hpp
