#pragma once

#if REFLECTION_GENERATION == 1
#define META(...) __attribute__((annotate(#__VA_ARGS__)))
#define AllowPrivateAccess
#define ExposeToEditor
#else
#define EXPAND(x) x
#define CAT(a, b) a##b

#define COUNT_ARGS_IMPL( \
_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
count, ...) count

#define COUNT_ARGS(...) \
EXPAND(COUNT_ARGS_IMPL(__VA_ARGS__, \
20, 19, 18, 17, 16, 15, 14, 13, 12, 11, \
10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define META_CHOOSER2(count) META##count
#define META_CHOOSER1(count) META_CHOOSER2(count)
#define META_CHOOSER(...) META_CHOOSER1(COUNT_ARGS(__VA_ARGS__))

#define META(...) EXPAND(META_CHOOSER(__VA_ARGS__)(__VA_ARGS__))

#define META0()
#define META1(a) a
#define META2(a, b) a b
#define META3(a, b, c) a b c
#define META4(a, b, c, d) a b c d
#define META5(a, b, c, d) a b c d
#define META6(a, b, c, d, e) a b c d e 
#define META7(a, b, c, d, e, f) a b c d e f
#define META8(a, b, c, d, e, f, g) a b c d e f g
#define META9(a, b, c, d, e, f, g, h) a b c d e f h

// Disable warning about empty macro arguments. 
#pragma warning(disable : 4003)

// add up to META10 or more as needed


#define AllowPrivateAccess friend class ReflectionPrivateMethodAccesser;

#define ExposeToEditor
#endif

// Called whenever the field was modified in the inspector
#define OnInspectorChangedEvent(methodName)

//#define TRACY_FIBERS

#include "Engine/Utils/StdIncludes.hpp"

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

static constexpr float FLOAT_MAX = 3.402823466e+38f;