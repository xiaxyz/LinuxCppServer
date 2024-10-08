#ifndef XEventLoop_hpp
#define XEventLoop_hpp

#include <functional>
#include <memory>
#include <vector>

class XPoller;
class XChannel;

class XEventLoop
{
private:
	std::unique_ptr<XPoller> poller;
	bool quit;

public:
	XEventLoop();
	~XEventLoop();

	void Loop();
	void UpdateChannel(std::shared_ptr<XChannel> _channel);
};

#endif // XEventLoop_hpp
