#pragma once

namespace glm
{
	inline glm::vec3 right(const glm::mat4& inMatrix)
	{
		return glm::normalize(glm::vec3(inMatrix[0][0], inMatrix[0][1], inMatrix[0][2]));
	}

	inline glm::vec3 up(const glm::mat4& inMatrix)
	{
		return glm::normalize(glm::vec3(inMatrix[1][0], inMatrix[1][1], inMatrix[1][2]));
	}

	inline glm::vec3 forward(const glm::mat4& inMatrix)
	{
		// Vulkan and OpenGL uses a right-handed coordinate system which means that the z axis points towards the screen. (Further away is lower number)
		// Comparing this to DirectX which uses a left-handed coordinate system which means the z axis points away from the screen. (Further away is higher number)
		// 
		// Because of this I have negated the return vector here as for gameplay reasons otherwise we'd go away from where we are looking.
		return glm::normalize(-glm::vec3(inMatrix[2][0], inMatrix[2][1], inMatrix[2][2]));
	}
}