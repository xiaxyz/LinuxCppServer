#include "XEventLoop.hpp"
#include "XServer.hpp"

int main(int argc, char const *argv[])
{
    XEventLoop loop;
    XServer server(&loop);
    loop.Loop();
    return 0;
}
