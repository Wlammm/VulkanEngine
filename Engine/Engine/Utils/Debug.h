#pragma once

class Debug
{
public:
	struct DrawLineInfos
	{
		glm::vec3 myStart;
		glm::vec3 myEnd;
		Color myColor;
	};

	static void DrawLine(const glm::vec3& inStart, const glm::vec3& inEnd, const Color& inColor = Color::White());
	static void DrawArrow(const glm::vec3& inStart, const glm::vec3& inEnd, const Color& inColor = Color::White());

	static void DrawSphere(const glm::vec3& inPosition, const float inRadius, const Color& inColor = Color::White(), const int inSphereSubdivisions = 25);

	static void DrawCapsule(const glm::vec3& inPosition, const glm::quat& inRotation, const float inRadius, const float inHeight, const Color& inColor = Color::White());

	static const List<DrawLineInfos>& GetDrawInfos();
	static void ClearDrawInfos();
private:
	inline static List<DrawLineInfos> myLineInfos;
};
