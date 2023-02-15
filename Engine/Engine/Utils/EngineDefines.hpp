#pragma once

#define check(x) if(!(x)) __debugbreak()
#define checkif(x) check(!(x))

#ifdef DEBUG
#define THROW(x) { __debugbreak(); throw std::runtime_error(x); }
#elif
#define THROW(x) throw std::runtime_error(x);
#endif

#define THROW_IF(x, message) if(x) THROW(message)

#define del(x) { delete x; x = nullptr; }