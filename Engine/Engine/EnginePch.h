#pragma once

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

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>

#include <entt/entt.hpp>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_vulkan.h>

#include "Engine/Core/EngineDefines.hpp"
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

#include "Engine/Core/Console.h"

#include "Engine/Containers/List.hpp"