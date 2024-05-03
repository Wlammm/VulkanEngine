#pragma once
#include "IndexBufferSystem.h"
#include "VertexBufferSystem.h"

struct Mesh
{
	uint NumVertices = 0;
	uint NumIndices = 0;

	VertexBufferHandle VertexBuffer;
	IndexBufferHandle IndexBuffer;

	glm::vec4 mySphereCenterBounds;
	
	class Material* myMaterial = nullptr;
};
