#pragma once

// THIS FILE IS THE ONLY PLACE WE SHOULD INCLUDE STD HEADERS. THIS WILL BE INCLUDED IN THE PCH FILE BUT IS REQUIRED TO BE HERE AS THE STD LIBRARY DOESNT ALLOW REDEFINING PRIVATE TO PUBLIC WHICH IS REQUIRED BY THE REFLECTION SYSTEM.

#include <Windows.h>
#undef max
#undef min

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <filesystem>
#include <functional>
#include <queue>
#include <future>
#include <processthreadsapi.h>
#include <source_location>