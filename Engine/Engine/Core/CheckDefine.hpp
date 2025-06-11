
#ifdef DEBUG
#include <windows.h>
#undef min
#undef max

#define check(x)\
if(!(x))\
{\
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
