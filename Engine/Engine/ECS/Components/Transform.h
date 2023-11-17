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

	void SetPosition(const Vec3f& inPosition);
	void SetPosition(const float inX, const float inY, const float inZ);

	void SetScale(const Vec3f& inScale);
	void SetScale(const float inX, const float inY, const float inZ);
	void SetScale(const float inScalar);

	void SetRotation(const Quatf& inQuat);
	void SetRotationRad(const Vec3f& inRotation);
	void SetRotationDeg(const Vec3f& inRotation);
	void SetRotationDeg(const float inX, const float inY, const float inZ);

	const Vec3f& GetPositionLocal() const;
	const Quatf& GetRotationLocal() const;
	const Vec3f& GetScaleLocal() const;

	const Vec3f	LocalForward() const;
	const Vec3f	LocalRight() const;
	const Vec3f	LocalUp() const;

	Mat4f GetMatrix() const;
	Mat4f GetMatrixLocal() const;

	Vec3f GetPosition() const;

	Quatf GetRotation() const;
	Vec3f GetRotationRad() const;
	Vec3f GetRotationDeg() const;

	Vec3f GetScale() const;

	Transform* GetParent() const;
	bool HasParent() const;

	Vec3f GetForward() const;
	Vec3f GetUp() const;
	Vec3f GetRight() const;

	void Move(const Vec3f& inDisplacement);

private:
	Vec3f myPosition { 0, 0, 0 };
	Quatf myRotation {};
	Vec3f myScale { 1, 1, 1 };

	Transform* myParent = nullptr;
	List<Transform*> myChildren;
};