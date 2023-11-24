#pragma once

#include "CheckDefine.hpp"

#ifdef DEBUG
#define THROW(x) { __debugbreak(); throw std::runtime_error(x); }
#else
#define THROW(x) throw std::runtime_error(x);
#endif

#define THROW_IF(x, message) if(x) THROW(message)

#define DO_ONCE(x) { static bool firstRun = true; if(firstRun) { firstRun = false; x } }

#define del(x) { delete x; x = nullptr; }