#if __cplusplus
#pragma once
using vec4 = glm::vec4;
using mat4 = glm::mat4;
#endif

struct RenderItem
{
    vec4 myBoundingSphereModelSpace; // X, Y, Z are center position in model space. W is radius
    uint myVertexOffset;
    uint myIndexOffset;
    uint myIndexCount;
    uint padding;
};

struct RenderObject
{
    mat4 myToWorld;
    uint myRenderItemIndex;
};