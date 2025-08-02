#pragma once
#include "Engine/ComponentSystem/GameObjectTag.hpp"
#include "Engine/Core/AutoInit.h"
#include "Engine/System/SystemManager.hpp"
#include "Engine/System/WorldSystem.h"
#include "Engine/Physics/PhysicsQueryStructs.h"

class DirectionalLightActor;
class Actor;
class CameraComponent;
class DirectionalLightComponent;
class ECSRegistry;
struct DirectionalLight;

class World : public AutoInit
{
public:
    World();
    ~World();

    void Init() override;

    void DoTick();
    virtual void Tick() = 0;
    void Destroy();

    void TickPhysics();

    void SaveToFile(const std::filesystem::path& inPath);
    void LoadFromFile(const std::filesystem::path& inPath);

    bool Raycast(const glm::vec3& inOrigin, const glm::vec3& inDirection, RaycastHit& outHit,
                 const float inMaxDistance = FLOAT_MAX, const TagMask inExcludedTags = 0, bool inIgnoreTriggers = true);
    bool RaycastAll(const glm::vec3& inOrigin, const glm::vec3& inDirection, List<RaycastHit>& outHits,
                    const float inMaxDistance = FLOAT_MAX, const TagMask inExcludedTags = 0,
                    bool inIgnoreTriggers = true);

    template <typename ActorType, typename... Args>
    ActorType* SpawnActor(const std::string& inName, Args&&... inArgs)
    {
        myActors.Add(MakeUnique<ActorType>(std::forward<Args>(inArgs)...));
        ActorType* actor = static_cast<ActorType*>(myActors.Last().Get());
        actor->myWorld = this;
        actor->SetName(inName);
        actor->RegisterComponents();
        actor->DoOnCreate();
        return actor;
    }

    void RemoveActor(Actor* inActor);

    const List<UniquePtr<Actor>>& GetAllActors() const;

    class AssetRegistry& GetAssetRegistry() const;

    void OnTriggerEnter(Actor* inFirst, Actor* inOther);
    void OnTrigger(Actor* inFirst, Actor* inOther);
    void OnTriggerExit(Actor* inFirst, Actor* inOther);

    void OnCollisionEnter(Actor* inFirst, Actor* inOther);
    void OnCollision(Actor* inFirst, Actor* inOther);
    void OnCollisionExit(Actor* inFirst, Actor* inOther);
    
    DirectionalLightComponent* GetDirectionalLight() const;

    template <typename SystemType>
    SystemType& GetWorldSystem() const
    {
        return mySystemManager->GetSystem<SystemType>();
    }

    void SetMainCamera(CameraComponent* inCamera);
    CameraComponent* GetMainCamera() const;

private:
    void CreateWorldSystems();

    void TickActorDeletes();

protected:
    UniquePtr<class AssetRegistry> myAssetRegistry = nullptr;
    UniquePtr<SystemManager<WorldSystem>> mySystemManager = nullptr;

    META(SerializeField)
    List<UniquePtr<Actor>> myActors{};

    List<Actor*> myActorsToDelete{};

    DirectionalLightActor* myDirectionalLightActor;

    CameraComponent* myMainCamera = nullptr;
};
