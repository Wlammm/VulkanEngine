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

	glm::vec3 position = GetPosition();
	glm::quat rotation = GetRotation();
	glm::vec3 scale = GetScale();

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

void Transform::SetRotationLocal(const glm::quat& inQuaternion)
{
	myRotation = inQuaternion;
}

void Transform::SetScaleLocal(const glm::vec3& inScale)
{
	myScale = inScale;
}

void Transform::SetScaleLocal(const float inScale)
{
	myScale = glm::vec3(inScale);
}

void Transform::SetPosition(const glm::vec3& inPosition)
{
	if (myParent)
		myPosition = glm::vec3((glm::vec4(inPosition, 1.0f) * glm::inverse(myParent->GetMatrix())));
	else
		myPosition = inPosition;
}

void Transform::SetPosition(const float inX, const float inY, const float inZ)
{
	SetPosition({ inX, inY, inZ });
}

void Transform::SetPositionX(const float inX)
{
	SetPosition({ inX, myPosition.y, myPosition.z });
}

void Transform::SetPositionY(const float inY)
{
	SetPosition({ myPosition.x, inY, myPosition.z });
}

void Transform::SetPositionZ(const float inZ)
{
	SetPosition({ myPosition.x, myPosition.y, inZ });
}

void Transform::SetScale(const glm::vec3& inScale)
{
	if (myParent)
		myScale = inScale * (1.f / myParent->GetScale());
	else
		myScale = inScale;
}

void Transform::SetScale(const float inX, const float inY, const float inZ)
{
	SetScale({ inX, inY, inZ });
}

void Transform::SetScale(const float inScalar)
{
	SetScale({ inScalar, inScalar, inScalar });
}

void Transform::SetRotation(const glm::quat& inQuat)
{
	if (myParent)
		myRotation = inQuat * glm::inverse(myParent->GetRotation());
	else
		myRotation = inQuat;
}

void Transform::SetRotationRad(const glm::vec3& inRotation)
{
	glm::mat4 rotMatrix = glm::mat4(1.0f);

	rotMatrix = glm::rotate(rotMatrix, inRotation.z, { 0, 0, 1 });
	rotMatrix = glm::rotate(rotMatrix, inRotation.x, { 1, 0, 0 });
	rotMatrix = glm::rotate(rotMatrix, inRotation.y, { 0, 1, 0 });

	myRotation = glm::quat(inRotation);
}

void Transform::SetRotationDeg(const glm::vec3& inRotation)
{
	SetRotationRad(inRotation * glm::radians(inRotation));
}

void Transform::SetRotationDeg(const float inX, const float inY, const float inZ)
{
	SetRotationDeg({ inX, inY, inZ });
}

const glm::vec3& Transform::GetScaleLocal() const
{
	return myScale;
}

glm::mat4 Transform::GetMatrix() const
{
	//glm::mat4 m = glm::identity<glm::mat4>();
	//
	//m = glm::scale(m, GetScale());
	//m *= glm::mat4_cast(GetRotation());
	//m = glm::translate(m, GetPosition());
	//return m;

	return glm::translate(glm::mat4(1.0f), GetPosition()) * glm::mat4_cast(GetRotation()) * glm::scale(glm::mat4(1.0f), GetScale());
}

glm::mat4 Transform::GetMatrixLocal() const
{
	glm::mat4 m = glm::identity<glm::mat4>();

	m = glm::scale(m, GetScaleLocal());
	m *= glm::mat4_cast(GetRotationLocal());
	m = glm::translate(m, GetPositionLocal());
	return m;
}

glm::vec3 Transform::GetPosition() const
{
	if (myParent)
	{
		const glm::vec3 scaledLocalPos = myPosition * myParent->myScale;
		const glm::vec3 rotatedLocalPos = scaledLocalPos * myParent->GetRotation();
		return myParent->GetPosition() + rotatedLocalPos;
	}
	else
	{
		return myPosition;
	}
}

glm::quat Transform::GetRotation() const
{
	if (myParent)
		return myRotation * myParent->GetRotation();
	else
		return myRotation;
}

glm::vec3 Transform::GetRotationRad() const
{
	return glm::eulerAngles(myRotation);
}

glm::vec3 Transform::GetRotationDeg() const
{
	return glm::degrees(GetRotationRad());
}

glm::vec3 Transform::GetScale() const
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

glm::vec3 Transform::GetForward() const
{
	return glm::forward(GetMatrix());
}

glm::vec3 Transform::GetUp() const
{
	return glm::up(GetMatrix());
}

glm::vec3 Transform::GetRight() const
{
	return glm::right(GetMatrix());
}

void Transform::Move(const glm::vec3& inDisplacement)
{
	myPosition += inDisplacement;
}

const glm::vec3& Transform::GetPositionLocal() const
{
	return myPosition;
}

const glm::quat& Transform::GetRotationLocal() const
{
	return myRotation;
}

const glm::vec3 Transform::LocalForward() const
{
	auto m = GetMatrixLocal();
	return glm::forward(m);
}

const glm::vec3 Transform::LocalRight() const
{
	auto m = GetMatrixLocal();
	return glm::right(m);
}

const glm::vec3 Transform::LocalUp() const
{
	auto m = GetMatrixLocal();
	return glm::up(m);
}

void Transform::SetPositionLocal(const glm::vec3& inPosition)
{
	myPosition = inPosition;
}