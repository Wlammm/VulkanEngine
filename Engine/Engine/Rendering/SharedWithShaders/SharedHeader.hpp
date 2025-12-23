#if __cplusplus
// ---- Source ----
#pragma once
using vec4 = glm::vec4;
using mat4 = glm::mat4;
using vec3 = glm::vec3;
using mat4 = glm::mat4;

#define ALIGNAS(x) alignas(x)
#define DEFAULT_TO(x) = x

#define BUFFER(BufferTypeName, shaderBufferName, setIndex, bindingIndex, bufferType, body)\
struct BufferTypeName\
{\
    body\
};

#define DYNAMIC_BUFFER(BufferStructName, BufferType, bufferName, setIndex, bindingIndex, bufferGlslType, ...)\
struct BufferStructName\
{\
    GPUList<BufferType> bufferName{__VA_ARGS__};\
};


#else
#extension GL_EXT_scalar_block_layout : enable

// ---- Shaders ----
#define BUFFER(BufferTypeName, shaderBufferName, setIndex, bindingIndex, bufferType, body)\
layout(set = setIndex, binding = bindingIndex, scalar) bufferType BufferTypeName\
{\
    body\
} shaderBufferName;

#define DYNAMIC_BUFFER(BufferStructName, BufferType, bufferName, setIndex, bindingIndex, bufferGlslType, ...)
layout(set = setIndex, binding = bindingIndex, scalar) bufferGlslType BufferStructName\
{\
    uint inNumElements;
    BufferType inData[];
} bufferName;

#define DISABLE_ARRAY_WARNING
#define ENABLE_ARRAY_WARNING

#define ALIGNAS(x)
#define DEFAULT_TO(x) 

#define ENGINE_DESCRIPTOR_SET_INDEX 0
#endif

// ---- Shared ----
#define ENGINE_DESCRIPTOR_SET_INDEX 0