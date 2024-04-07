#pragma once

class Debug
{
public:
	struct DrawLineInfos
	{
		glm::vec3 myStart;
		glm::vec3 myEnd;
	};

	static void DrawLine(const glm::vec3& inStart, const glm::vec3& inEnd);

	static const List<DrawLineInfos>& GetDrawInfos();
	static void ClearDrawInfos();
private:
	inline static List<DrawLineInfos> myLineInfos;
};
