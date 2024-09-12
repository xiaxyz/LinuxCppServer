#include "XUtility.hpp"

bool ErrorIf(bool _condition, const char *_error_message, const char * _file, int _line)
{
    if (_condition)
    {
        std::cerr << _file << ":" << _line << ": " << "errno: " << errno << ", " << "message: " << _error_message << std::endl;
        return true;
    }
    return false;
}
