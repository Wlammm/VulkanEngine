#pragma once
#include "Vertex.hpp"

class MeshUtils
{
public:
    static glm::vec4 CalculateSphereBounds(const List<Vertex>& inVertices);
};
