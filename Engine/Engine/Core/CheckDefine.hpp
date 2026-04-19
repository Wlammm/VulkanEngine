
#ifdef DEBUG
#include <windows.h>
#undef min
#undef max

#include "Console.h"

#define check(x)\
if(!(x))\
{\
    LOG_ERROR("Check failed in file: %s on line: %i: %s", __FILE__, __LINE__, #x);\
    if(IsDebuggerPresent())\
    {\
        __debugbreak();\
    }\
    else\
    {\
        throw std::runtime_error(#x);\
    }\
}

#define checkif(x) check(!(x))
#else
#define check(x)
#define checkif(x)
#endif
