#include "EnginePch.h"
#include "TransformComponent.h"


TransformComponent::TransformComponent()
{

}

TransformComponent::~TransformComponent()
{
	if (myParent)
		RemoveParent();

	// Remove all childrens parent without making it call back to us.
	for(TransformComponent*& child : myChildren)
	{
		child->myParent = nullptr;
	}
	MarkDirty(true, true, true);
}

physx::PxTransform TransformComponent::AsPxTransform() const
{
	glm::vec3 pos = GetPosition();
	glm::quat quat = GetRotation();
	
	physx::PxTransform transform;
	transform.p = {pos.x, pos.y, pos.z};
	transform.q.x = quat.x;
	transform.q.y = quat.y;
	transform.q.z = quat.z;
	transform.q.w = quat.w;

	return transform;
}

void TransformComponent::TickPhysics()
{
	Component::TickPhysics();

	if (myPositionDirty)
		OnPositionChanged();

	if (myRotationDirty)
		OnRotationChanged();

	if (myScaleDirty)
		OnScaleChanged();

	myPositionDirty = false;
	myRotationDirty = false;
	myScaleDirty = false;
}

void TransformComponent::SetParent(TransformComponent& inParent)
{
	if (myParent)
		RemoveParent();

	myParent = &inParent;
	myParent->AddChild(*this);
	MarkDirty(true, true, true);
}

void TransformComponent::RemoveParent()
{
	if (!myParent)
		return;

	glm::vec3 position = GetPosition();
	glm::quat rotation = GetRotation();
	glm::vec3 scale = GetScale();

	myParent->RemoveChild(*this);
	myPosition = position;
	myRotation = rotation;
	myScale = scale;

	myParent = nullptr;
	MarkDirty(true, true, true);
}

void TransformComponent::AddChild(TransformComponent& inChild)
{
	check(!myChildren.Contains(&inChild));
	myChildren.Add(&inChild);
}

void TransformComponent::RemoveChild(TransformComponent& inChild)
{
	check(myChildren.Contains(&inChild));
	myChildren.Remove(&inChild);
}

const List<TransformComponent*>& TransformComponent::GetChildren() const
{
	return myChildren;
}

void TransformComponent::SetRotationLocal(const glm::quat& inQuaternion)
{
	myRotation = inQuaternion;
	MarkDirty(false, true, false);
}

void TransformComponent::SetScaleLocal(const glm::vec3& inScale)
{
	myScale = inScale;
	MarkDirty(false, false, true);
}

void TransformComponent::SetScaleLocal(const float inScale)
{
	SetScaleLocal(glm::vec3(inScale));
}

void TransformComponent::SetPosition(const glm::vec3& inPosition)
{
	if (myParent)
		myPosition = glm::vec3(glm::inverse(myParent->GetMatrix()) * (glm::vec4(inPosition, 1.0f)));
	else
		myPosition = inPosition;
	
	MarkDirty(true, false, false);
}

void TransformComponent::SetPosition(const float inX, const float inY, const float inZ)
{
	SetPosition(glm::vec3{ inX, inY, inZ });
}

void TransformComponent::SetPosition(const physx::PxVec3T<float>& inPosition)
{
	mySkipPhysicsUpdate = true;
	SetPosition(glm::vec3(inPosition.x, inPosition.y, inPosition.z));
	mySkipPhysicsUpdate = false;
}

void TransformComponent::SetPositionX(const float inX)
{
	SetPosition(glm::vec3{ inX, myPosition.y, myPosition.z });
}

void TransformComponent::SetPositionY(const float inY)
{
	SetPosition(glm::vec3{ myPosition.x, inY, myPosition.z });
}

void TransformComponent::SetPositionZ(const float inZ)
{
	SetPosition(glm::vec3{ myPosition.x, myPosition.y, inZ });
}

void TransformComponent::SetScale(const glm::vec3& inScale)
{
	if (myParent)
		myScale = inScale * (1.f / myParent->GetScale());
	else
		myScale = inScale;
	MarkDirty(false, false, true);
}

void TransformComponent::SetScale(const float inX, const float inY, const float inZ)
{
	SetScale({ inX, inY, inZ });
}

void TransformComponent::SetScale(const float inScalar)
{
	SetScale({ inScalar, inScalar, inScalar });
}

void TransformComponent::SetRotation(const glm::quat& inQuat)
{
	if (myParent)
		myRotation = glm::inverse(myParent->GetRotation()) * inQuat;
	else
		myRotation = inQuat;
	MarkDirty(false, true, false);
}

void TransformComponent::SetRotation(const physx::PxQuatT<float>& inQuat)
{
	mySkipPhysicsUpdate = true;
	SetRotation(glm::quat(inQuat.w, inQuat.x, inQuat.y, inQuat.z));
	mySkipPhysicsUpdate = false;
}

void TransformComponent::SetRotationRad(const glm::vec3& inRotation)
{
	SetRotation(glm::quat(inRotation));
}

void TransformComponent::SetRotationDeg(const glm::vec3& inRotation)
{
	SetRotationRad(glm::radians(inRotation));
}

void TransformComponent::SetRotationDeg(const float inX, const float inY, const float inZ)
{
	SetRotationDeg({ inX, inY, inZ });
}

const glm::vec3& TransformComponent::GetScaleLocal() const
{
	return myScale;
}

glm::mat4 TransformComponent::GetMatrix() const
{
	//glm::mat4 m = glm::identity<glm::mat4>();
	//
	//m = glm::scale(m, GetScale());
	//m *= glm::mat4_cast(GetRotation());
	//m = glm::translate(m, GetPosition());
	//return m;
	return glm::translate(glm::mat4(1.0f), GetPosition()) * glm::mat4_cast(GetRotation()) * glm::scale(glm::mat4(1.0f), GetScale());
}

glm::mat4 TransformComponent::GetMatrixLocal() const
{
	glm::mat4 m = glm::identity<glm::mat4>();

	m = glm::scale(m, GetScaleLocal());
	m *= glm::mat4_cast(GetRotationLocal());
	m = glm::translate(m, GetPositionLocal());
	return m;
}

glm::vec3 TransformComponent::GetPosition() const
{
	if (myParent)
	{
		const glm::vec3 scaledLocalPos = myPosition * myParent->myScale;
		const glm::vec3 rotatedLocalPos = myParent->GetRotation() * scaledLocalPos;
		return myParent->GetPosition() + rotatedLocalPos;
	}
	else
	{
		return myPosition;
	}
}

glm::quat TransformComponent::GetRotation() const
{
	if (myParent)
		return myParent->GetRotation() * myRotation;
	else
		return myRotation;
}

glm::vec3 TransformComponent::GetRotationRad() const
{
	return glm::eulerAngles(GetRotation());
}

glm::vec3 TransformComponent::GetRotationDeg() const
{
	return glm::degrees(GetRotationRad());
}

glm::vec3 TransformComponent::GetScale() const
{
	if (myParent)
		return myScale * myParent->GetScale();
	else
		return myScale;
}

TransformComponent* TransformComponent::GetParent() const
{
	return myParent;
}

bool TransformComponent::HasParent() const
{
	return myParent;
}

glm::vec3 TransformComponent::GetForward() const
{
	return glm::forward(GetMatrix());
}

glm::vec3 TransformComponent::GetUp() const
{
	return glm::up(GetMatrix());
}

glm::vec3 TransformComponent::GetRight() const
{
	return glm::right(GetMatrix());
}

void TransformComponent::Move(const glm::vec3& inDisplacement)
{
	myPosition += inDisplacement;
	MarkDirty(true, false, false);
}

void TransformComponent::Move(const float inX, const float inY, const float inZ)
{
	Move({inX, inY, inZ});
}

void TransformComponent::Rotate(const glm::vec3& inRotation)
{
	SetRotationDeg(GetRotationDeg() + inRotation);
}

void TransformComponent::Rotate(const float inX, const float inY, const float inZ)
{
	Rotate({inX, inY, inZ});
}

bool TransformComponent::IsPositionDirty() const
{
	return myPositionDirty;
}

bool TransformComponent::IsRotationDirty() const
{
	return myRotationDirty;
}

bool TransformComponent::IsScaleDirty() const
{
	return myScaleDirty;
}

void TransformComponent::MarkDirtyFromInspector()
{
	MarkDirty(true, true, true);
}

void TransformComponent::MarkDirty(bool inPosition, bool inRotation, bool inScale)
{
	MarkRenderStateDirty();

	if (!mySkipPhysicsUpdate)
	{
		myPositionDirty = myPositionDirty || inPosition;
		myRotationDirty = myRotationDirty || inRotation;
		myScaleDirty = myScaleDirty || inScale;
	}
	
	for(TransformComponent* child : myChildren)
	{
		child->MarkDirty(inPosition, inRotation, inScale);
	}
}

const glm::vec3& TransformComponent::GetPositionLocal() const
{
	return myPosition;
}

const glm::quat& TransformComponent::GetRotationLocal() const
{
	return myRotation;
}

const glm::vec3 TransformComponent::LocalForward() const
{
	auto m = GetMatrixLocal();
	return glm::forward(m);
}

const glm::vec3 TransformComponent::LocalRight() const
{
	auto m = GetMatrixLocal();
	return glm::right(m);
}

const glm::vec3 TransformComponent::LocalUp() const
{
	auto m = GetMatrixLocal();
	return glm::up(m);
}

void TransformComponent::SetPositionLocal(const glm::vec3& inPosition)
{
	myPosition = inPosition;
	MarkDirty(true, false, false);
}