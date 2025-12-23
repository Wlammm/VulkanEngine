#include "EnginePch.h"
#include "MeshUtils.h"

glm::vec4 MeshUtils::CalculateSphereBounds(const List<Vertex>& inVertices)
{
    ZoneScoped;
    glm::vec3 centerPos = glm::vec3();

    for(const Vertex& vertex : inVertices)
    {
        centerPos += glm::vec3(vertex.myPosition);
    }
	
    centerPos /= static_cast<float>(inVertices.size());

    float maxDistanceFromCenter = 0;
    for(const Vertex& vertex : inVertices)
    {
        maxDistanceFromCenter = std::max(maxDistanceFromCenter, glm::distance(centerPos, glm::vec3(vertex.myPosition)));
    }
    return glm::vec4(centerPos, maxDistanceFromCenter);
}
