#include "XEventLoop.hpp"
#include "XServer.hpp"

int main(int argc, char const *argv[])
{
    std::shared_ptr<XEventLoop> loop = std::make_shared<XEventLoop>();
    XServer server(loop);
    loop->Loop();
    return 0;
}
