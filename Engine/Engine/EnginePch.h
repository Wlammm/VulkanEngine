#pragma once

#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <filesystem>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#include "Engine/Utils/EngineDefines.hpp"
#include "Engine/Math/Defines.hpp"

#include "Engine/Math/Constants.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Matrix2x2.hpp"
#include "Engine/Math/Matrix3x3.hpp"
#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/Quaternion.hpp"
#include "Engine/Math/io.hpp"
#include "Engine/Math/Maths.hpp"

#include "Utils/Console.h"