#pragma once

namespace glm
{
	inline glm::vec3 right()
	{
		return glm::vec3(1, 0, 0);
	}

	inline glm::vec3 up()
	{
		return glm::vec3(0, 1, 0);
	}

	inline glm::vec3 forward()
	{
		return glm::vec3(0, 0, 1);
	}
	
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
		return glm::normalize(glm::vec3(inMatrix[2][0], inMatrix[2][1], inMatrix[2][2]));
	}
}