#include "EnginePch.h"
#include "Debug.h"

void Debug::DrawLine(const glm::vec3& inStart, const glm::vec3& inEnd, const Color& inColor)
{
	myLineInfos.Add({ inStart, inEnd, inColor });
}

void Debug::DrawArrow(const glm::vec3& inStart, const glm::vec3& inEnd, const Color& inColor)
{
	const float length = glm::distance(inStart, inEnd);
	const glm::vec3 direction = glm::normalize(inEnd - inStart);
	
	const float arrowLength = 0.1f * length;
	const float arrowWidth = 0.01f * length;

	// Main line
	DrawLine(inStart, inEnd, inColor);

	// I know this won't be right if the direction sent in is straight up but its still a good name to use for thinking.
	glm::vec3 right;
	if(direction != glm::vec3(0, 1, 0))
	{
		right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction)) * arrowWidth;
	}
	else
	{
		right = glm::normalize(glm::cross(glm::vec3(1, 0, 0), direction)) * arrowWidth;
	}
	glm::vec3 left = -right;

	// Arrow head
	DrawLine(inEnd - direction * arrowLength + right * arrowLength, inEnd, inColor);
	DrawLine(inEnd - direction * arrowLength + left * arrowLength, inEnd, inColor);
}

void Debug::DrawSphere(const glm::vec3& inPosition, const float inRadius, const Color& inColor, const int inSphereSubdivisions)
{
	enum Axis
	{
		X = 0,
		Y,
		Z,

		Count
	};

	// For each axis
	for (int axis = X; axis < Axis::Count; axis++)
	{
		glm::vec3 firstPos;
		glm::vec3 prevPos;

		// Draw a circle
		for (int i = 0; i < inSphereSubdivisions; ++i)
		{
			const float percentOfCircle = static_cast<float>(i) / static_cast<float>(inSphereSubdivisions);

			const float radiansOfCircle = glm::pi<float>() * 2.f * percentOfCircle;

			glm::vec3 posInCircle;

			switch (axis)
			{
			case X:
				posInCircle = glm::vec3(
					cos(radiansOfCircle) * inRadius,
					sin(radiansOfCircle) * inRadius,
					0.f);
				break;
			case Y:
				posInCircle = glm::vec3(
					cos(radiansOfCircle) * inRadius,
					0.f,
					sin(radiansOfCircle) * inRadius);
				break;
			case Z:
				posInCircle = glm::vec3(
					0.f,
					cos(radiansOfCircle) * inRadius,
					sin(radiansOfCircle) * inRadius);
				break;
			default:
				assert(false && "whot");
				break;
			}

			if (i == 0)
			{
				firstPos = inPosition + posInCircle;
				prevPos = inPosition + posInCircle;
				continue;
			}

			const auto toPos = inPosition + posInCircle;

			DrawLine(prevPos, toPos, inColor);

			prevPos = toPos;
		}

		DrawLine(prevPos, firstPos, inColor);
	}
}

const List<Debug::DrawLineInfos>& Debug::GetDrawInfos()
{
	return myLineInfos;
}

void Debug::ClearDrawInfos()
{
	myLineInfos.Clear();
}