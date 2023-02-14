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

	void SetPositionLocal(const Vec3f& inPosition);
	void SetRotationLocal(const Quatf& inQuaternion);
	void SetScaleLocal(const Vec3f& inScale);
	void SetScaleLocal(const float inScale);

	const Vec3f& GetPositionLocal() const;
	const Quatf& GetRotationLocal() const;
	const Vec3f& GetScaleLocal() const;

	const Vec3f	LocalForward() const;
	const Vec3f	LocalRight() const;
	const Vec3f	LocalUp() const;

	Mat4f GetMatrix() const;

	Vec3f GetPosition() const;
	Quatf GetRotation() const;
	Vec3f GetScale() const;

private:
	Vec3f myPosition;
	Quatf myRotation;
	Vec3f myScale = { 1, 1, 1 };

	Transform* myParent = nullptr;
	std::vector<Transform*> myChildren;
};