#ifdef DEBUG
#define check(x) if(!(x)) __debugbreak()
#define checkif(x) check(!(x))
#else
#define check(x)
#define checkif(x)
#endif