#ifndef XAcceptor_hpp
#define XAcceptor_hpp

#include <functional>

class XEventLoop;
class XSocket;
class XInternetAddress;
class XChannel;

class XAcceptor
{
private:
    XEventLoop *event_loop;
    XSocket *socket;
    XInternetAddress *internet_address;
    XChannel *accept_channel;
    std::function<void(XSocket *)> new_connection_callback;

public:
    XAcceptor(XEventLoop *_event_loop);
    ~XAcceptor();

    void AcceptConnection();
    void SetNewConnectionCallback(std::function<void(XSocket *)> _new_connection_callback);
};

#endif // XAcceptor_hpp
