#ifndef XServer_hpp
#define XServer_hpp

#include <cstring>
#include <unistd.h>
#include <iostream>
#include <format>
#include <vector>
#include <map>
#include <memory>

class XEventLoop;
class XAcceptor;
class XSocket;
class XConnection;

class XServer
{
private:
    std::shared_ptr<XEventLoop> event_loop;
    std::unique_ptr<XAcceptor> acceptor;
    std::map<int, std::unique_ptr<XConnection>> connections;
public:
    XServer(std::shared_ptr<XEventLoop> _event_loop);
    ~XServer();

    void NewConnection(std::shared_ptr<XSocket> _socket);
    void DeleteConnection(std::shared_ptr<XSocket> _socket);
    // void HandleReadEvent(XSocket *_socket);
};



#endif // XServer_hpp
