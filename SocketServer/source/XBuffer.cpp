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


const char *XBuffer::CharData()
{
	return data.data();
}

void XBuffer::SetData(const char *_data)
{
	data = _data;
}

void XBuffer::Clear()
{
	data.clear();
}

std::string &XBuffer::Data()
{
	return data;
}
