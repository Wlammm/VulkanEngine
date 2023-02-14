#include "EnginePch.h"
#include "Transform.h"

Transform::Transform()
{

}

Transform::~Transform()
{

}

void Transform::SetParent(Transform* inParent)
{
	if (myParent)
		RemoveParent();

	myParent = inParent;
	myParent->AddChild(this);
}

void Transform::RemoveParent()
{
	if (!myParent)
		return;

	Vec3f position = GetPosition();
	Quatf rotation = GetRotation();
	Vec3f scale = GetScale();

	myParent->RemoveChild(this);
	myPosition = position;
	myRotation = rotation;
	myScale = scale;
}

void Transform::AddChild(Transform* inChild)
{
	check(std::find(myChildren.begin(), myChildren.end(), inChild) == myChildren.end());
	myChildren.push_back(inChild);
}

void Transform::RemoveChild(Transform* inChild)
{
	check(std::find(myChildren.begin(), myChildren.end(), inChild) != myChildren.end());
	myChildren.erase(std::remove(myChildren.begin(), myChildren.end(), inChild));
}

void Transform::SetRotationLocal(const Quatf& inQuaternion)
{
	myRotation = inQuaternion;
}

void Transform::SetScaleLocal(const Vec3f& inScale)
{
	myScale = inScale;
}

void Transform::SetScaleLocal(const float inScale)
{
	myScale = Vec3f(inScale);
}

const Vec3f& Transform::GetScaleLocal() const
{
	return myScale;
}

Mat4f Transform::GetMatrix() const
{
	return Mat4f::CreateScale(GetScaleLocal())
		* GetRotationLocal().ToMatrix()
		* Mat4f::CreateTranslation(GetPositionLocal());
}

Vec3f Transform::GetPosition() const
{
	return Vec3f();
}

Quatf Transform::GetRotation() const
{
	return Quatf();
}

Vec3f Transform::GetScale() const
{
	return Vec3f();
}

const Vec3f& Transform::GetPositionLocal() const
{
	return myPosition;
}

const Quatf& Transform::GetRotationLocal() const
{
	return myRotation;
}

const Vec3f Transform::LocalForward() const
{
	auto m = GetMatrix();
	return m.GetForward();
}

const Vec3f Transform::LocalRight() const
{
	auto m = GetMatrix();
	return m.GetRight();
}

const Vec3f Transform::LocalUp() const
{
	auto m = GetMatrix();
	return m.GetUp();
}

void Transform::SetPositionLocal(const Vec3f& inPosition)
{
	myPosition = inPosition;
}