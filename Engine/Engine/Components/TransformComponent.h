#pragma once
#include "ComponentSystem/Component.h"
#include "Delegates/MulticastDelegate.hpp"
#include <foundation/PxTransform.h>

class TransformComponent : public Component
{
public:
    TransformComponent();
    ~TransformComponent();

    physx::PxTransform AsPxTransform() const;
    
    void SetParent(TransformComponent* inParent);
    void RemoveParent();

    void AddChild(TransformComponent* inChild);
    void RemoveChild(TransformComponent* inChild);

    void SetPositionLocal(const glm::vec3& inPosition);
    void SetRotationLocal(const glm::quat& inQuaternion);
    void SetScaleLocal(const glm::vec3& inScale);
    void SetScaleLocal(const float inScale);

    void SetPosition(const glm::vec3& inPosition);
    void SetPosition(const float inX, const float inY, const float inZ);
    void SetPosition(const physx::PxVec3T<float>& inPosition);

    void SetPositionX(const float inX);
    void SetPositionY(const float inY);
    void SetPositionZ(const float inZ);

    void SetScale(const glm::vec3& inScale);
    void SetScale(const float inX, const float inY, const float inZ);
    void SetScale(const float inScalar);

    void SetRotation(const glm::quat& inQuat);
    void SetRotation(const physx::PxQuatT<float>& inQuat);
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

    TransformComponent* GetParent() const;
    bool HasParent() const;

    glm::vec3 GetForward() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetRight() const;

    void Move(const glm::vec3& inDisplacement);
    void Move(const float inX, const float inY, const float inZ);

    MulticastDelegate<void()> OnScaleChanged;

private:
    void MarkScaleChanged();
    
private:
    void MarkDirty();

    glm::vec3 myPosition { 0, 0, 0 };
    glm::quat myRotation = glm::identity<glm::quat>();
    glm::vec3 myScale { 1, 1, 1 };

    // This bool is set to true whenever we get an update from physics.
    // It is done in the physx implementations of SetPosition and SetRotation.
    // This is required as otherwise we will fall into an infinite loop of telling PhysX that we've updated our position when we're just updating the transform from the simulation.
    bool mySkipPhysicsUpdate = false;
    
    TransformComponent* myParent = nullptr;
    List<TransformComponent*> myChildren;
};
