#ifndef GLOBALLIGHTBUFFER_HPP
#define GLOBALLIGHTBUFFER_HPP

#include "../SharedHeader.hpp"
#include "../MeshStructs.hpp"

BUFFER(GlobalLightBuffer, inGlobalLightBuffer, ENGINE_DESCRIPTOR_SET_INDEX, 2, uniform, 
    vec4 myDirectionalLightColor;
    vec3 myDirectionalLightDirection;
    uint myCubemapIndex;

    mat4 myLightView;
    mat4 myLightProjection;
)
#endif // GLOBALLIGHTBUFFER_HPP
