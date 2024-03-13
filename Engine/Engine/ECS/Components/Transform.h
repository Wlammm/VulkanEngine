#pragma once

class Transform 
{
public:
	Transform();
	~Transform();

	void SetParent(Transform* inParent);
	void RemoveParent();

	void AddChild(Transform* inChild);
	void RemoveChild(Transform* inChild);

	void SetPositionLocal(const glm::vec3& inPosition);
	void SetRotationLocal(const glm::quat& inQuaternion);
	void SetScaleLocal(const glm::vec3& inScale);
	void SetScaleLocal(const float inScale);

	void SetPosition(const glm::vec3& inPosition);
	void SetPosition(const float inX, const float inY, const float inZ);

	void SetPositionX(const float inX);
	void SetPositionY(const float inY);
	void SetPositionZ(const float inZ);

	void SetScale(const glm::vec3& inScale);
	void SetScale(const float inX, const float inY, const float inZ);
	void SetScale(const float inScalar);

	void SetRotation(const glm::quat& inQuat);
	void SetRotationRad(const glm::vec3& inRotation);
	void SetRotationDeg(const glm::vec3& inRotation);
	void SetRotationDeg(const float inX, const float inY, const float inZ);

	const glm::vec3& GetPositionLocal() const;
	const glm::quat& GetRotationLocal() const;
	const glm::vec3& GetScaleLocal() const;

	const glm::vec3	LocalForward() const;
	const glm::vec3	LocalRight() const;
	const glm::vec3	LocalUp() const;

	glm::mat4 GetMatrix() const;
	glm::mat4 GetMatrixLocal() const;

	glm::vec3 GetPosition() const;

	glm::quat GetRotation() const;
	glm::vec3 GetRotationRad() const;
	glm::vec3 GetRotationDeg() const;

	glm::vec3 GetScale() const;

	Transform* GetParent() const;
	bool HasParent() const;

	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;

	void Move(const glm::vec3& inDisplacement);

private:
	glm::vec3 myPosition { 0, 0, 0 };
	glm::quat myRotation {};
	glm::vec3 myScale { 1, 1, 1 };

	Transform* myParent = nullptr;
	List<Transform*> myChildren;
};