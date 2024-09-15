#ifndef XBuffer_hpp
#define XBuffer_hpp

#include <string>

class XBuffer
{
private:
    std::string data;

public:
    XBuffer();
    ~XBuffer();
    std::string &Append(const char *_data, std::size_t _size);
    std::size_t Size();
    const char *Data();
    void Clear();
};

#endif // XBuffer_hpp
