#include "XBuffer.hpp"

XBuffer::XBuffer()
{
}

XBuffer::~XBuffer()
{
}

std::string &XBuffer::Append(const char *_data, std::size_t _size)
{
	return data.append(_data, _size);
}

std::size_t XBuffer::Size()
{
	return data.size();
}

const char *XBuffer::Data()
{
	return data.data();
}

void XBuffer::Clear()
{
	data.clear();
}
