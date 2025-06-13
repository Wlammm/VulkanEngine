#pragma once

//#define TRACY_FIBERS

#include "Engine/Utils/StdIncludes.hpp"

// #if !IGNORED_BY_REFLECTION
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>
// #endif

#define USED_VULKAN_VERSION VK_API_VERSION_1_3

#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Math/GlmUtils.hpp"

#if !IGNORED_BY_REFLECTION
#include <entt/entt.hpp>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_vulkan.h>
#endif

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

static constexpr float FLOAT_MAX = 3.402823466e+38f;