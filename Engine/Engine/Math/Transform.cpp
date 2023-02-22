#include "EnginePch.h"
#include "Transform.h"

Transform::Transform()
{

}

Transform::~Transform()
{
	if (myParent)
		RemoveParent();

	// Remove all childrens parent without making it call back to us.
	for(Transform*& child : myChildren)
	{
		child->myParent = nullptr;
	}
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

	myParent = nullptr;
}

void Transform::AddChild(Transform* inChild)
{
	check(!myChildren.Contains(inChild));
	myChildren.Add(inChild);
}

void Transform::RemoveChild(Transform* inChild)
{
	check(myChildren.Contains(inChild));
	myChildren.Remove(inChild);
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

void Transform::SetPosition(const Vec3f& inPosition)
{
	if (myParent)
		myPosition = (inPosition.ToVec4(1.f) * myParent->GetMatrix().Inverse()).ToVec3();
	else
		myPosition = inPosition;
}

void Transform::SetScale(const Vec3f& inScale)
{
	if (myParent)
		myScale = inScale * (1.f / myParent->GetScale());
	else
		myScale = inScale;
}

void Transform::SetRotation(const Quatf& inQuat)
{
	if (myParent)
		myRotation = inQuat * myParent->GetRotation().GetInverse();
	else
		myRotation = inQuat;
}

const Vec3f& Transform::GetScaleLocal() const
{
	return myScale;
}

Mat4f Transform::GetMatrix() const
{
	return Mat4f::CreateScale(GetScale())
		* GetRotation().ToMatrix()
		* Mat4f::CreateTranslation(GetPosition());
}

Mat4f Transform::GetMatrixLocal() const
{
	return Mat4f::CreateScale(GetScaleLocal())
		* GetRotationLocal().ToMatrix()
		* Mat4f::CreateTranslation(GetPositionLocal());
}

Vec3f Transform::GetPosition() const
{
	if (myParent)
	{
		const Vec3f scaledLocalPos = myPosition * myParent->myScale;
		const Vec3f rotatedLocalPos = scaledLocalPos * myParent->GetRotation();
		return myParent->GetPosition() + rotatedLocalPos;
	}
	else
	{
		return myPosition;
	}
}

Quatf Transform::GetRotation() const
{
	if (myParent)
		return myRotation * myParent->GetRotation();
	else
		return myRotation;
}

Vec3f Transform::GetScale() const
{
	if (myParent)
		return myScale * myParent->GetScale();
	else
		return myScale;
}

Transform* Transform::GetParent() const
{
	return myParent;
}

bool Transform::HasParent() const
{
	return myParent;
}

Vec3f Transform::GetForward() const
{
	return GetMatrix().GetForward();
}

Vec3f Transform::GetUp() const
{
	return GetMatrix().GetUp();
}

Vec3f Transform::GetRight() const
{
	return GetMatrix().GetRight();
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
	auto m = GetMatrixLocal();
	return m.GetForward();
}

const Vec3f Transform::LocalRight() const
{
	auto m = GetMatrixLocal();
	return m.GetRight();
}

const Vec3f Transform::LocalUp() const
{
	auto m = GetMatrixLocal();
	return m.GetUp();
}

void Transform::SetPositionLocal(const Vec3f& inPosition)
{
	myPosition = inPosition;
}