#ifndef CAMERABUFFER_HPP
#define CAMERABUFFER_HPP

#include "../SharedHeader.hpp"
#include "../MeshStructs.hpp"

BUFFER(CameraBuffer, inCameraBuffer, ENGINE_DESCRIPTOR_SET_INDEX, 0, uniform, 
    mat4 myToView;
    mat4 myProjection;
    vec3 myCameraPosition;
)

#endif // CAMERABUFFER_HPP
