#include "EnginePch.h"
#include "Debug.h"

#include "Engine.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Transform.h"
#include "World/World.h"

void Debug::DrawLine(const glm::vec3& inStart, const glm::vec3& inEnd, const Color& inColor)
{
	myLineInfos.Add({ inStart, inEnd, inColor });
}

void Debug::DrawArrow(const glm::vec3& inStart, const glm::vec3& inEnd, const Color& inColor)
{
	check(false && "Arrow implementation not complete");
	DrawLine(inStart, inEnd, inColor);
	
	const glm::vec3 arrowDirection = glm::normalize(inEnd - inStart);
	const float distance = glm::distance(inStart, inEnd);

	auto view = Engine::GetWorld().GetRegistry().view<const Transform, const Camera>();

	glm::vec3 cameraUpVector;
	for (auto [ent, transform, camera] : view.each())
	{
		cameraUpVector = transform.GetUp();
		break;
	}
	
	DrawLine(inStart, inStart * arrowDirection * (distance * 0.8f) + cameraUpVector * (distance * 0.8f));
	DrawLine(inStart, inStart * arrowDirection * (distance * 0.8f) - cameraUpVector * (distance * 0.8f));
	
	/*
	*FVector Dir = (LineEnd-LineStart);
	Dir.Normalize();
	FVector Up(0, 0, 1);
	FVector Right = Dir ^ Up;
	if (!Right.IsNormalized())
	{
		Dir.FindBestAxisVectors(Up, Right);
	}
	const FVector Origin = FVector::ZeroVector;
	FMatrix TM;
	// get matrix with dir/right/up
	TM.SetAxes(&Dir, &Right, &Up, &Origin);

	// since dir is x direction, my arrow will be pointing +y, -x and -y, -x
	const float ArrowSqrt = FMath::Sqrt(ArrowSize);

	BatchedLines.Emplace(LineStart,LineEnd, Color, LifeTime, Thickness, DepthPriority, BatchID);
	BatchedLines.Emplace(LineEnd,LineEnd + TM.TransformPosition(FVector(-ArrowSqrt, ArrowSqrt, 0)), Color, LifeTime, Thickness, DepthPriority, BatchID);
	BatchedLines.Emplace(LineEnd,LineEnd + TM.TransformPosition(FVector(-ArrowSqrt, -ArrowSqrt, 0)), Color, LifeTime, Thickness, DepthPriority, BatchID);
	 **/
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