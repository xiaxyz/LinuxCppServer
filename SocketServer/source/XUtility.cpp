#include "XUtility.hpp"

bool ErrorIf(bool _condition, const char *_error_message)
{
    if (_condition)
    {
        std::cerr << _error_message << std::endl;
        return true;
    }
    return false;
}
