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
	MarkDirty();
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


void TransformComponent::SetParent(TransformComponent* inParent)
{
	if (myParent)
		RemoveParent();

	myParent = inParent;
	myParent->AddChild(this);
	MarkDirty();
}

void TransformComponent::RemoveParent()
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
	MarkDirty();
}

void TransformComponent::AddChild(TransformComponent* inChild)
{
	check(!myChildren.Contains(inChild));
	myChildren.Add(inChild);
}

void TransformComponent::RemoveChild(TransformComponent* inChild)
{
	check(myChildren.Contains(inChild));
	myChildren.Remove(inChild);
}

void TransformComponent::SetRotationLocal(const glm::quat& inQuaternion)
{
	myRotation = inQuaternion;
	MarkDirty();
}

void TransformComponent::SetScaleLocal(const glm::vec3& inScale)
{
	myScale = inScale;
	MarkDirty();
}

void TransformComponent::SetScaleLocal(const float inScale)
{
	myScale = glm::vec3(inScale);
	MarkDirty();
}

void TransformComponent::SetPosition(const glm::vec3& inPosition)
{
	if (myParent)
		myPosition = glm::vec3((glm::vec4(inPosition, 1.0f) * glm::inverse(myParent->GetMatrix())));
	else
		myPosition = inPosition;
	MarkDirty();
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
	MarkDirty();
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
		myRotation = inQuat * glm::inverse(myParent->GetRotation());
	else
		myRotation = inQuat;
	MarkDirty();
}

void TransformComponent::SetRotation(const physx::PxQuatT<float>& inQuat)
{
	mySkipPhysicsUpdate = true;
	SetRotation(glm::quat(inQuat.w, inQuat.x, inQuat.y, inQuat.z));
	mySkipPhysicsUpdate = false;
}

void TransformComponent::SetRotationRad(const glm::vec3& inRotation)
{
	myRotation = glm::quat(inRotation);
	MarkDirty();
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
		const glm::vec3 rotatedLocalPos = scaledLocalPos * myParent->GetRotation();
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
		return myRotation * myParent->GetRotation();
	else
		return myRotation;
}

glm::vec3 TransformComponent::GetRotationRad() const
{
	return glm::eulerAngles(myRotation);
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
	MarkDirty();
}

void TransformComponent::Move(const float inX, const float inY, const float inZ)
{
	Move({inX, inY, inZ});
}

void TransformComponent::MarkDirty()
{
	if(!mySkipPhysicsUpdate)
		MarkPhysicsStateDirty();
	MarkRenderStateDirty();
	
	for(TransformComponent* child : myChildren)
	{
		if(!mySkipPhysicsUpdate)
			child->MarkPhysicsStateDirty();
		
		child->MarkRenderStateDirty();
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
	MarkDirty();
}