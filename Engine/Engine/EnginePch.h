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
#include <source_location>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>

#define USED_VULKAN_VERSION VK_API_VERSION_1_3

#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Math/GlmUtils.hpp"

#include <entt/entt.hpp>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_vulkan.h>

#include "Engine/Core/EngineDefines.hpp"
#include "Engine/Utils/HashUtils.hpp"
#include "Engine/Math/Color.h"
#include "Engine/Math/LinearColor.h"

#include "Engine/Core/Console.h"

#include "Engine/Containers/List.hpp"
#include "Engine/Containers/MutexList.hpp"

#include "Engine/Utils/OnScopeExit.h"
#include "Engine/Utils/CommonMacros.h"

using uint = unsigned int;