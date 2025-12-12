#pragma once
#include "Engine/AssetRegistry/Asset.h"
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

// I am not sure I am fully comfortable with the world being an asset... I have a feeling I'll come back to this :)
class World : public Asset
{
public:
    inline static constexpr const char* EmptyWorldPath = "NewWorld.world";
    
    World();
    ~World();

    virtual void Init() {}
    
    void PostPropertiesSerialized() override;
    static List<std::string> GetAssetExtensions() { return {".world" }; }
    
    void DoTick();
    virtual void Tick() = 0;
    void Destroy();

    void TickPhysics();

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
        InitActor(actor, inName);
        return actor;
    }

    Actor* SpawnActor(const Type* inActorType, const std::string& inName);

    void RemoveActor(Actor* inActor);
    void RemoveAllActors();

    const List<UniquePtr<Actor>>& GetAllActors() const;

    void OnTriggerEnter(Actor* inFirst, Actor* inOther);
    void OnTrigger(Actor* inFirst, Actor* inOther);
    void OnTriggerExit(Actor* inFirst, Actor* inOther);

    void OnCollisionEnter(Actor* inFirst, Actor* inOther);
    void OnCollision(Actor* inFirst, Actor* inOther);
    void OnCollisionExit(Actor* inFirst, Actor* inOther);

    DirectionalLightComponent* GetDirectionalLight() const;

    template <typename ActorType>
    ActorType* FindActorOfType() const
    {
        for (const UniquePtr<Actor>& actor : myActors)
        {
            if (actor.IsA<ActorType>())
                return static_cast<ActorType*>(actor.Get());
        }
        return nullptr;
    }

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
    
    void InitActor(Actor* inActor, const std::string& inName);

protected:
    UniquePtr<SystemManager<WorldSystem>> mySystemManager = nullptr;

    META(SerializeField)
    List<UniquePtr<Actor>> myActors{};
    
    List<Actor*> myActorsToDelete{};

    mutable DirectionalLightActor* myCachedDirectionalLightActor;

    CameraComponent* myMainCamera = nullptr;
};
