#include "EnginePch.h"
#include "Debug.h"

void Debug::DrawLine(const glm::vec3& inStart, const glm::vec3& inEnd)
{
	myLineInfos.Add({ inStart, inEnd });
}

const List<Debug::DrawLineInfos>& Debug::GetDrawInfos()
{
	return myLineInfos;
}

void Debug::ClearDrawInfos()
{
	myLineInfos.Clear();
}