#pragma once

// THIS FILE IS THE ONLY PLACE WE SHOULD INCLUDE STD HEADERS. THIS WILL BE INCLUDED IN THE PCH FILE BUT IS REQUIRED TO BE HERE AS THE STD LIBRARY DOESNT ALLOW REDEFINING PRIVATE TO PUBLIC WHICH IS REQUIRED BY THE REFLECTION SYSTEM.

#include <Windows.h>
#undef max
#undef min

//#if !IGNORED_BY_REFLECTION
#include <vector>
#include <fstream>
#include <filesystem>
#include <queue>
#include <future>
#include <processthreadsapi.h>
#include <source_location>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <functional>
#include <map>
// #endif
#include <string>
