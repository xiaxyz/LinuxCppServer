#ifndef XUtility_hpp
#define XUtility_hpp

#include <iostream>

#define ErrorIfFile(_condition, _error_message) ErrorIf(_condition, _error_message, __FILE__, __LINE__)

bool ErrorIf(bool _condition, const char *_error_message, const char *_file, int _line);

#endif // XUtility_hpp
