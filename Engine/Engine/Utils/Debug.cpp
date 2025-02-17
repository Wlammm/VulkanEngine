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

// TODO: Fix rotation for this. Not working correctly.
void Debug::DrawCapsule(const glm::vec3& inPosition, const glm::quat& inRotation, const float inRadius,
	const float inHeight, const Color& inColor)
{
	glm::quat usedRotation = inRotation;

	const glm::vec3 start = inPosition - glm::vec3(1,0,0) * usedRotation * inHeight * 0.5f;
	const glm::vec3 end = inPosition + glm::vec3(1,0,0) * usedRotation * inHeight * 0.5f;

	glm::vec3 axis = glm::vec3(0, 0, 1) * usedRotation;
	Color color = inColor;

	for (size_t i = 0; i < 4; i++)
	{
		DrawLine(start + axis * inRadius, end + axis * inRadius, color);

		switch (i)
		{
		case 0:
			axis = glm::vec3(0, 1, 0) * usedRotation;
			break;
		case 1:
			axis = glm::vec3(0, 0, -1) * usedRotation;
			break;
		case 2:
			axis = glm::vec3(0, -1, 0) * usedRotation;
			break;
		default:
			break;
		}
	}

	constexpr int TotalPoints = 16;

	glm::vec3 firstPos;
	glm::vec3 prevPos;

	glm::vec3 firstPosFirstHalf;
	glm::vec3 prevPosFirstHalf;

	glm::vec3 firstPosSecondHalf;
	glm::vec3 prevPosSecondHalf;

	for (size_t circles = 0; circles < 2; circles++)
	{
		for (int i = 0; i < TotalPoints; ++i)
		{
			const float percentOfCircle = static_cast<float>(i) / static_cast<float>(TotalPoints);

			const float radiansOfCircle = glm::pi<float>() * 0.5f + glm::pi<float>() * 2.0f * percentOfCircle;
			const float radiansOfHalfCircle = glm::pi<float>() * 0.5f + glm::pi<float>() * 2.0f * percentOfCircle * 0.5f;

			glm::vec3 posInCircle;
			glm::vec3 posInFirstHalfCircle;
			glm::vec3 posInSecondHalfCircle;

			posInCircle = glm::vec3(
				0.0f,
				cos(radiansOfCircle) * inRadius,
				sin(radiansOfCircle) * inRadius);

			posInFirstHalfCircle = glm::vec3(
				cos(radiansOfHalfCircle) * inRadius,
				0.0f,
				sin(radiansOfHalfCircle) * inRadius);

			posInSecondHalfCircle = glm::vec3(
				cos(radiansOfHalfCircle) * inRadius,
				sin(radiansOfHalfCircle) * inRadius,
				0.0f);

			posInCircle = posInCircle * usedRotation;
			posInFirstHalfCircle = posInFirstHalfCircle * usedRotation;
			posInSecondHalfCircle = posInSecondHalfCircle * usedRotation;

			if (i == 0)
			{
				if (circles == 0)
				{
					firstPos = start + posInCircle;
					prevPos = start + posInCircle;

					firstPosFirstHalf = start + posInFirstHalfCircle;
					prevPosFirstHalf = start + posInFirstHalfCircle;

					firstPosSecondHalf = start + posInSecondHalfCircle;
					prevPosSecondHalf = start + posInSecondHalfCircle;
					continue;
				}
				else
				{
					firstPos = end + posInCircle;
					prevPos = end + posInCircle;

					firstPosFirstHalf = end + posInFirstHalfCircle;
					prevPosFirstHalf = end + posInFirstHalfCircle;

					firstPosSecondHalf = end + posInSecondHalfCircle;
					prevPosSecondHalf = end + posInSecondHalfCircle;
					continue;
				}
			}
			
			if (circles == 0)
			{
				glm::vec3 toPos = start + posInCircle;
				DrawLine(prevPos, toPos, inColor);
				prevPos = toPos;

				toPos = start + posInFirstHalfCircle;
				DrawLine(prevPosFirstHalf, toPos, inColor);
				prevPosFirstHalf = toPos;

				toPos = start + posInSecondHalfCircle;
				DrawLine(prevPosSecondHalf, toPos, inColor);
				prevPosSecondHalf = toPos;
			}
			else
			{
				glm::vec3 toPos = end + posInCircle;
				DrawLine(prevPos, toPos, inColor);
				prevPos = toPos;

				toPos = end - posInFirstHalfCircle;
				DrawLine(prevPosFirstHalf, toPos, inColor);
				prevPosFirstHalf = toPos;

				toPos = end - posInSecondHalfCircle;
				DrawLine(prevPosSecondHalf, toPos, inColor);
				prevPosSecondHalf = toPos;
			}
		}
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