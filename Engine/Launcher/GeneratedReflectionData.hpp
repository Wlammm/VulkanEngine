#pragma once
#include "Engine/Utils/StdIncludes.hpp"
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/Field.h"
#include "Engine/Reflection/ReflectionSystem.h"


// BEGIN INCLUDES FOR REFLECTED TYPES

#include "../Engine/Vulkan/VulkanDescriptorSet.h"
#include "../Engine/Math/Color.h"
#include "../Engine/Assets/Texture.h"
#include "../Engine/Math/LinearColor.h"
#include "../Engine/ComponentSystem/ComponentSystem.h"
#include "../Engine/Core/Console.h"
#include "../Engine/Components/SphereColliderComponent.h"
#include "../Engine/Utils/String.hpp"
#include "../Engine/Containers/ContainerTypes.hpp"
#include "../Engine/EngineProperties.hpp"
#include "../Engine/Utils/OnScopeExit.h"
#include "../Engine/Serialization/BinaryWriter.h"
#include "../Engine/Rendering/Vertex.hpp"
#include "../Engine/Animation/Skeleton.h"
#include "../Engine/Engine.h"
#include "../Engine/Coroutines/Coroutine.h"
#include "../Engine/Components/PointLightComponent.h"
#include "../Engine/AssetRegistry/Asset.h"
#include "../Engine/Core/Filewatcher.h"
#include "../Engine/AssetRegistry/AssetContainer.h"
#include "../Engine/Assets/Model.h"
#include "../Engine/Core/ThreadPool.h"
#include "../Engine/Assets/TextureCube.h"
#include "../Engine/Utils/ThreadUtils.hpp"
#include "../Engine/AssetRegistry/AssetRegistry.h"
#include "../Engine/Rendering/SkyboxPipeline.h"
#include "../Engine/Components/ConvexColliderComponent.h"
#include "../Engine/Assets/JsonAsset.h"
#include "../Engine/Vulkan/VulkanAllocator.h"
#include "../Engine/Events/EventHandler.h"
#include "../Engine/Vulkan/VulkanBuffer.h"
#include "../Engine/Events/EventTypes.hpp"
#include "../Engine/Components/CapsuleColliderComponent.h"
#include "../Engine/Events/EventObserver.h"
#include "../Engine/System/System.h"
#include "../Engine/Components/StaticMeshComponent.h"
#include "../Engine/Delegates/Internal/LambdaFuncCtor.hpp"
#include "../Engine/Rendering/RenderSystem.h"
#include "../Engine/Components/DirectionalLightComponent.h"
#include "../Engine/Utils/MathUtils.hpp"
#include "../Engine/Vulkan/VulkanDynamicBuffer.hpp"
#include "../Engine/ComponentSystem/GameObject.h"
#include "../Engine/Assets/Material.h"
#include "../Engine/Assets/Shader.h"
#include "../Engine/ComponentSystem/ComponentArray.h"
#include "../Engine/Components/MeshColliderComponent.h"
#include "../Engine/Core/Time.h"
#include "../Engine/Containers/SegmentedList.h"
#include "../Engine/System/WorldSystem.h"
#include "../Engine/Components/LandscapeColliderComponent.h"
#include "../Engine/ComponentSystem/Component.h"
#include "../Engine/Components/TransformComponent.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Components/CharacterControllerComponent.h"
#include "../Engine/Components/RigidbodyComponent.h"
#include "../Engine/Vulkan/GPUSceneSystem.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Components/BoxColliderComponent.h"
#include "../Engine/Components/LandscapeRenderComponent.h"
#include "../Engine/Coroutines/CoroutineManager.h"
#include "../Engine/Components/CameraComponent.h"
#include "../Editor/EditorPch.h"
#include "../Engine/Components/EditorCameraMovementComponent.h"
#include "../Engine/Physics/PhysicsSystem.h"
#include "../Engine/Math/Heightfield.h"
#include "../Engine/Components/SinWaveMovementComponent.h"
#include "../Engine/Core/AutoInit.h"
#include "../Engine/Core/AutoInitManager.h"
#include "../Engine/Coroutines/Awaitable.h"
#include "../Engine/Physics/PhysicsErrorCallback.h"
#include "../Editor/Windows/SelectionSystem.h"
#include "../Engine/Physics/PhysicsListener.h"
#include "../Engine/Physics/PhysicsQueryStructs.h"
#include "../Engine/Delegates/Delegate.hpp"
#include "../Engine/Reflection/Field.h"
#include "../Engine/Reflection/Class.h"
#include "../Engine/Reflection/ReflectionSystem.h"
#include "../Engine/Rendering/DebugPipeline.h"
#include "../Engine/Rendering/FullscreenPipeline.h"
#include "../Engine/Rendering/GDRPipeline.h"
#include "../Engine/Rendering/IndexBufferHandle.h"
#include "../Engine/Shaders/MeshStructs.hpp"
#include "../Engine/Rendering/IndexBufferSystem.h"
#include "../Engine/Rendering/Mesh.h"
#include "../Engine/Rendering/MeshSystem.h"
#include "../Engine/Rendering/MeshUtils.h"
#include "../Engine/Rendering/VertexBufferHandle.h"
#include "../Engine/Rendering/TextureSystem.h"
#include "../Engine/Vulkan/ResizableBuffer.h"
#include "../Engine/Rendering/VertexBufferSystem.h"
#include "../Engine/Serialization/BinaryReader.h"
#include "../Engine/Systems/LandscapeSystem.h"
#include "../Engine/Systems/PointLightSystem.h"
#include "../Engine/Vulkan/VulkanUtils.hpp"
#include "../Engine/Utils/BinaryUtils.hpp"
#include "../Engine/Utils/Debug.h"
#include "../Engine/Vulkan/Aftermath/ShaderDatabase.h"
#include "../Engine/Vulkan/Aftermath/NvidiaAftermathTracker.h"
#include "../Engine/Vulkan/Staging/StagingBuffer.h"
#include "../Engine/Vulkan/VulkanContext.h"
#include "../Engine/Vulkan/Staging/StagingSystem.h"
#include "../Engine/Vulkan/VulkanDevice.h"
#include "../Engine/Vulkan/VulkanImage.h"
#include "../Engine/Vulkan/VulkanImGui.h"
#include "../Engine/Vulkan/VulkanPhysicalDevice.h"
#include "../Engine/Vulkan/VulkanShaderIncluder.h"
#include "../Engine/Vulkan/VulkanSwapChain.h"
#include "../Engine/Windows/WindowHandler.h"
#include "../Engine/World/World.h"
#include "../Editor/Editor.h"
#include "../Editor/Windows/EditorWindow.h"
#include "../Editor/Windows/HierarchyWindow.h"
#include "../Editor/Windows/InspectorWindow.h"
#include "../Editor/Windows/Viewport.h"
#include "../Game/Components/Player/PlayerCameraControllerComponent.h"
#include "../Game/Components/Player/PlayerComponent.h"
#include "../Game/Components/SpringArmComponent.h"
#include "../Game/Game.h"
#include "../Engine/Containers/List.hpp"
#include "../Engine/Containers/MutexList.hpp"
#include "../Engine/Core/CheckDefine.hpp"
#include "../Engine/Core/EngineDefines.hpp"
#include "../Engine/Delegates/Internal/ConstMemberFuncCtor.hpp"
#include "../Engine/Delegates/Internal/FreeFuncCtor.hpp"
#include "../Engine/Delegates/Internal/FuncCtor.hpp"
#include "../Engine/Delegates/Internal/MemberFuncCtor.hpp"
#include "../Engine/Delegates/MulticastDelegate.hpp"
#include "../Engine/EnginePch.h"
#include "../Engine/Math/GlmUtils.hpp"
#include "../Engine/Physics/PhysXInclude.h"
#include "../Engine/System/SystemManager.hpp"
#include "../Engine/Utils/CommonMacros.h"
#include "../Engine/Utils/HashUtils.hpp"
#include "../Engine/Utils/StdIncludes.hpp"
#include "../Game/GamePch.h"
#include "../Game/GameTags.h"


// END INCLUDES FOR REFLECTED TYPES

class GeneratedReflectionData
{
public:
	static void RegisterReflectionData(Engine& inEngine)
	{
		ReflectionSystem& reflectionSystem = inEngine.GetReflectionSystem(); 

		// Create all classes.
		{
		    reflectionSystem.AddClass<VulkanDescriptorSet>("VulkanDescriptorSet", typeid(VulkanDescriptorSet).name());
reflectionSystem.AddClass<Color>("Color", typeid(Color).name());
reflectionSystem.AddClass<ImageData>("ImageData", typeid(ImageData).name());
reflectionSystem.AddClass<Texture>("Texture", typeid(Texture).name());
reflectionSystem.AddClass<LinearColor>("LinearColor", typeid(LinearColor).name());
reflectionSystem.AddClass<ComponentSystem>("ComponentSystem", typeid(ComponentSystem).name());
reflectionSystem.AddClass<Console>("Console", typeid(Console).name());
reflectionSystem.AddClass<SphereColliderComponent>("SphereColliderComponent", typeid(SphereColliderComponent).name());
reflectionSystem.AddClass<String>("String", typeid(String).name());
reflectionSystem.AddClass<IsCopyable<int>>("IsCopyable<int>", typeid(IsCopyable<int>).name());
reflectionSystem.AddClass<EngineProperties>("EngineProperties", typeid(EngineProperties).name());
reflectionSystem.AddClass<OnScopeExit>("OnScopeExit", typeid(OnScopeExit).name());
reflectionSystem.AddClass<BinaryWriter>("BinaryWriter", typeid(BinaryWriter).name());
reflectionSystem.AddClass<Vertex>("Vertex", typeid(Vertex).name());
reflectionSystem.AddClass<Skeleton>("Skeleton", typeid(Skeleton).name());
reflectionSystem.AddClass<Skeleton::Bone>("Skeleton::Bone", typeid(Skeleton::Bone).name());
reflectionSystem.AddClass<Engine>("Engine", typeid(Engine).name());
reflectionSystem.AddClass<PromiseReturnTypeImplementation<void>>("PromiseReturnTypeImplementation<void>", typeid(PromiseReturnTypeImplementation<void>).name());
reflectionSystem.AddClass<PointLightComponent>("PointLightComponent", typeid(PointLightComponent).name());
reflectionSystem.AddClass<Asset>("Asset", typeid(Asset).name());
reflectionSystem.AddClass<Filewatcher>("Filewatcher", typeid(Filewatcher).name());
reflectionSystem.AddClass<Filewatcher::CallbackHandle>("Filewatcher::CallbackHandle", typeid(Filewatcher::CallbackHandle).name());
reflectionSystem.AddClass<Filewatcher::FileData>("Filewatcher::FileData", typeid(Filewatcher::FileData).name());
reflectionSystem.AddClass<IAssetContainer>("IAssetContainer", typeid(IAssetContainer).name());
reflectionSystem.AddClass<SerializationMeshData>("SerializationMeshData", typeid(SerializationMeshData).name());
reflectionSystem.AddClass<Model>("Model", typeid(Model).name());
reflectionSystem.AddClass<ThreadPool>("ThreadPool", typeid(ThreadPool).name());
reflectionSystem.AddClass<TextureCube>("TextureCube", typeid(TextureCube).name());
reflectionSystem.AddClass<ThreadUtils>("ThreadUtils", typeid(ThreadUtils).name());
reflectionSystem.AddClass<AssetRegistry>("AssetRegistry", typeid(AssetRegistry).name());
reflectionSystem.AddClass<SkyboxPipeline>("SkyboxPipeline", typeid(SkyboxPipeline).name());
reflectionSystem.AddClass<ConvexColliderComponent>("ConvexColliderComponent", typeid(ConvexColliderComponent).name());
reflectionSystem.AddClass<JsonAsset>("JsonAsset", typeid(JsonAsset).name());
reflectionSystem.AddClass<VulkanAllocator>("VulkanAllocator", typeid(VulkanAllocator).name());
reflectionSystem.AddClass<EventHandler>("EventHandler", typeid(EventHandler).name());
reflectionSystem.AddClass<VulkanBuffer>("VulkanBuffer", typeid(VulkanBuffer).name());
reflectionSystem.AddClass<EventType>("EventType", typeid(EventType).name());
reflectionSystem.AddClass<CapsuleColliderComponent>("CapsuleColliderComponent", typeid(CapsuleColliderComponent).name());
reflectionSystem.AddClass<EventObserver>("EventObserver", typeid(EventObserver).name());
reflectionSystem.AddClass<System>("System", typeid(System).name());
reflectionSystem.AddClass<StaticMeshComponent>("StaticMeshComponent", typeid(StaticMeshComponent).name());
reflectionSystem.AddClass<RenderSystem>("RenderSystem", typeid(RenderSystem).name());
reflectionSystem.AddClass<DirectionalLightComponent>("DirectionalLightComponent", typeid(DirectionalLightComponent).name());
reflectionSystem.AddClass<MathUtils>("MathUtils", typeid(MathUtils).name());
reflectionSystem.AddClass<IVulkanDynamicBuffer>("IVulkanDynamicBuffer", typeid(IVulkanDynamicBuffer).name());
reflectionSystem.AddClass<GameObject>("GameObject", typeid(GameObject).name());
reflectionSystem.AddClass<Material>("Material", typeid(Material).name());
reflectionSystem.AddClass<Shader>("Shader", typeid(Shader).name());
reflectionSystem.AddClass<IComponentArray>("IComponentArray", typeid(IComponentArray).name());
reflectionSystem.AddClass<MeshColliderComponent>("MeshColliderComponent", typeid(MeshColliderComponent).name());
reflectionSystem.AddClass<Time>("Time", typeid(Time).name());
reflectionSystem.AddClass<WorldSystem>("WorldSystem", typeid(WorldSystem).name());
reflectionSystem.AddClass<LandscapeColliderComponent>("LandscapeColliderComponent", typeid(LandscapeColliderComponent).name());
reflectionSystem.AddClass<Component>("Component", typeid(Component).name());
reflectionSystem.AddClass<TransformComponent>("TransformComponent", typeid(TransformComponent).name());
reflectionSystem.AddClass<Input>("Input", typeid(Input).name());
reflectionSystem.AddClass<Input::KeyCode>("Input::KeyCode", typeid(Input::KeyCode).name());
reflectionSystem.AddClass<Input::MouseButton>("Input::MouseButton", typeid(Input::MouseButton).name());
reflectionSystem.AddClass<CharacterControllerComponent>("CharacterControllerComponent", typeid(CharacterControllerComponent).name());
reflectionSystem.AddClass<ForceMode>("ForceMode", typeid(ForceMode).name());
reflectionSystem.AddClass<RigidbodyComponent>("RigidbodyComponent", typeid(RigidbodyComponent).name());
reflectionSystem.AddClass<GPUSceneSystem>("GPUSceneSystem", typeid(GPUSceneSystem).name());
reflectionSystem.AddClass<ColliderComponent>("ColliderComponent", typeid(ColliderComponent).name());
reflectionSystem.AddClass<BoxColliderComponent>("BoxColliderComponent", typeid(BoxColliderComponent).name());
reflectionSystem.AddClass<LandscapeRenderComponent>("LandscapeRenderComponent", typeid(LandscapeRenderComponent).name());
reflectionSystem.AddClass<CoroutineManager>("CoroutineManager", typeid(CoroutineManager).name());
reflectionSystem.AddClass<CameraComponent>("CameraComponent", typeid(CameraComponent).name());
reflectionSystem.AddClass<EditorCameraMovementComponent>("EditorCameraMovementComponent", typeid(EditorCameraMovementComponent).name());
reflectionSystem.AddClass<PhysicsSystem>("PhysicsSystem", typeid(PhysicsSystem).name());
reflectionSystem.AddClass<Heightfield>("Heightfield", typeid(Heightfield).name());
reflectionSystem.AddClass<SinWaveMovementComponent>("SinWaveMovementComponent", typeid(SinWaveMovementComponent).name());
reflectionSystem.AddClass<AutoInit>("AutoInit", typeid(AutoInit).name());
reflectionSystem.AddClass<AutoInitManager>("AutoInitManager", typeid(AutoInitManager).name());
reflectionSystem.AddClass<Awaitable>("Awaitable", typeid(Awaitable).name());
reflectionSystem.AddClass<ThreadType>("ThreadType", typeid(ThreadType).name());
reflectionSystem.AddClass<PhysicsErrorCallback>("PhysicsErrorCallback", typeid(PhysicsErrorCallback).name());
reflectionSystem.AddClass<SelectionSystem>("SelectionSystem", typeid(SelectionSystem).name());
reflectionSystem.AddClass<PhysicsListener>("PhysicsListener", typeid(PhysicsListener).name());
reflectionSystem.AddClass<RaycastHit>("RaycastHit", typeid(RaycastHit).name());
reflectionSystem.AddClass<Field>("Field", typeid(Field).name());
reflectionSystem.AddClass<Class>("Class", typeid(Class).name());
reflectionSystem.AddClass<ReflectionSystem>("ReflectionSystem", typeid(ReflectionSystem).name());
reflectionSystem.AddClass<DebugPipeline>("DebugPipeline", typeid(DebugPipeline).name());
reflectionSystem.AddClass<FullscreenPipeline>("FullscreenPipeline", typeid(FullscreenPipeline).name());
reflectionSystem.AddClass<GDRPipeline>("GDRPipeline", typeid(GDRPipeline).name());
reflectionSystem.AddClass<IndexBufferHandle>("IndexBufferHandle", typeid(IndexBufferHandle).name());
reflectionSystem.AddClass<MeshData>("MeshData", typeid(MeshData).name());
reflectionSystem.AddClass<VertexBufferData>("VertexBufferData", typeid(VertexBufferData).name());
reflectionSystem.AddClass<IndexBufferData>("IndexBufferData", typeid(IndexBufferData).name());
reflectionSystem.AddClass<MeshInstanceData>("MeshInstanceData", typeid(MeshInstanceData).name());
reflectionSystem.AddClass<PerDrawData>("PerDrawData", typeid(PerDrawData).name());
reflectionSystem.AddClass<PointLightData>("PointLightData", typeid(PointLightData).name());
reflectionSystem.AddClass<IndexBufferSystem>("IndexBufferSystem", typeid(IndexBufferSystem).name());
reflectionSystem.AddClass<Mesh>("Mesh", typeid(Mesh).name());
reflectionSystem.AddClass<MeshSystem>("MeshSystem", typeid(MeshSystem).name());
reflectionSystem.AddClass<MeshUtils>("MeshUtils", typeid(MeshUtils).name());
reflectionSystem.AddClass<VertexBufferHandle>("VertexBufferHandle", typeid(VertexBufferHandle).name());
reflectionSystem.AddClass<TextureSystem>("TextureSystem", typeid(TextureSystem).name());
reflectionSystem.AddClass<ResizableBuffer>("ResizableBuffer", typeid(ResizableBuffer).name());
reflectionSystem.AddClass<VertexBufferSystem>("VertexBufferSystem", typeid(VertexBufferSystem).name());
reflectionSystem.AddClass<BinaryReader>("BinaryReader", typeid(BinaryReader).name());
reflectionSystem.AddClass<LandscapeSystem>("LandscapeSystem", typeid(LandscapeSystem).name());
reflectionSystem.AddClass<PointLightSystem>("PointLightSystem", typeid(PointLightSystem).name());
reflectionSystem.AddClass<SamplerMode>("SamplerMode", typeid(SamplerMode).name());
reflectionSystem.AddClass<VulkanUtils>("VulkanUtils", typeid(VulkanUtils).name());
reflectionSystem.AddClass<BinaryUtils>("BinaryUtils", typeid(BinaryUtils).name());
reflectionSystem.AddClass<Debug>("Debug", typeid(Debug).name());
reflectionSystem.AddClass<Debug::DrawLineInfos>("Debug::DrawLineInfos", typeid(Debug::DrawLineInfos).name());
reflectionSystem.AddClass<ShaderDatabase>("ShaderDatabase", typeid(ShaderDatabase).name());
reflectionSystem.AddClass<NvidiaAftermathTracker>("NvidiaAftermathTracker", typeid(NvidiaAftermathTracker).name());
reflectionSystem.AddClass<StagingBuffer>("StagingBuffer", typeid(StagingBuffer).name());
reflectionSystem.AddClass<VulkanContext>("VulkanContext", typeid(VulkanContext).name());
reflectionSystem.AddClass<StagingSystem>("StagingSystem", typeid(StagingSystem).name());
reflectionSystem.AddClass<VulkanDevice>("VulkanDevice", typeid(VulkanDevice).name());
reflectionSystem.AddClass<VulkanImage>("VulkanImage", typeid(VulkanImage).name());
reflectionSystem.AddClass<VulkanImGui>("VulkanImGui", typeid(VulkanImGui).name());
reflectionSystem.AddClass<VulkanPhysicalDevice>("VulkanPhysicalDevice", typeid(VulkanPhysicalDevice).name());
reflectionSystem.AddClass<VulkanShaderIncluder>("VulkanShaderIncluder", typeid(VulkanShaderIncluder).name());
reflectionSystem.AddClass<VulkanSwapChain>("VulkanSwapChain", typeid(VulkanSwapChain).name());
reflectionSystem.AddClass<WindowHandler>("WindowHandler", typeid(WindowHandler).name());
reflectionSystem.AddClass<World>("World", typeid(World).name());
reflectionSystem.AddClass<Editor>("Editor", typeid(Editor).name());
reflectionSystem.AddClass<EditorWindow>("EditorWindow", typeid(EditorWindow).name());
reflectionSystem.AddClass<HierarchyWindow>("HierarchyWindow", typeid(HierarchyWindow).name());
reflectionSystem.AddClass<InspectorWindow>("InspectorWindow", typeid(InspectorWindow).name());
reflectionSystem.AddClass<Viewport>("Viewport", typeid(Viewport).name());
reflectionSystem.AddClass<PlayerCameraControllerComponent>("PlayerCameraControllerComponent", typeid(PlayerCameraControllerComponent).name());
reflectionSystem.AddClass<PlayerComponent>("PlayerComponent", typeid(PlayerComponent).name());
reflectionSystem.AddClass<SpringArmComponent>("SpringArmComponent", typeid(SpringArmComponent).name());
reflectionSystem.AddClass<Game>("Game", typeid(Game).name());

        }
        
        // Add all fields & add base classes.
        {
            { 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanDescriptorSet>();
	currentClass->AddField(Field("myUsesSharedLayout", 72, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myLayout", 80, reflectionSystem.GetClass<vk::DescriptorSetLayout>()));
	currentClass->AddField(Field("mySet", 88, reflectionSystem.GetClass<vk::DescriptorSet>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Color>();
	currentClass->AddField(Field("myR", 0, reflectionSystem.GetClass<unsigned char>()));
	currentClass->AddField(Field("myG", 1, reflectionSystem.GetClass<unsigned char>()));
	currentClass->AddField(Field("myB", 2, reflectionSystem.GetClass<unsigned char>()));
	currentClass->AddField(Field("myA", 3, reflectionSystem.GetClass<unsigned char>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ImageData>();
	currentClass->AddField(Field("mySourceFile", 0, reflectionSystem.GetClass<std::filesystem::path>()));
	currentClass->AddField(Field("myWidth", 32, reflectionSystem.GetClass<int>()));
	currentClass->AddField(Field("myHeight", 36, reflectionSystem.GetClass<int>()));
	currentClass->AddField(Field("myChannels", 40, reflectionSystem.GetClass<int>()));
	currentClass->AddField(Field("myNumMipLevels", 44, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myPixelData", 48, reflectionSystem.GetClass<List<unsigned char>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Texture>();
	currentClass->AddField(Field("myImage", 56, reflectionSystem.GetClass<VulkanImage *>()));
	currentClass->AddField(Field("myBindlessIndex", 64, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LinearColor>();
	currentClass->AddField(Field("myR", 0, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myG", 4, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myB", 8, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myA", 12, reflectionSystem.GetClass<float>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ComponentSystem>();
	currentClass->AddField(Field("myObjects", 16, reflectionSystem.GetClass<List<GameObject *>>()));
	currentClass->AddField(Field("myObjectsToDestory", 40, reflectionSystem.GetClass<List<GameObject *>>()));
	currentClass->AddField(Field("myComponentArrays", 64, reflectionSystem.GetClass<List<IComponentArray *>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<WorldSystem>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Console>();
	currentClass->AddField(Field("myConsoleHandle", 0, reflectionSystem.GetClass<void *>()));
	currentClass->AddField(Field("myLogToFileEnabled", 8, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myLogToFileStream", 16, reflectionSystem.GetClass<std::basic_ofstream<char>>()));
	currentClass->AddField(Field("myCoutBuffer", 280, reflectionSystem.GetClass<std::basic_streambuf<char> *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SphereColliderComponent>();
	currentClass->AddField(Field("myRadius", 56, reflectionSystem.GetClass<float>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<String>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IsCopyable<int>>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EngineProperties>();
	currentClass->AddField(Field("Title", 0, reflectionSystem.GetClass<std::basic_string<wchar_t>>()));
	currentClass->AddField(Field("WindowWidth", 32, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("WindowHeight", 36, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("WindowX", 40, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("WindowY", 44, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("StartupArguments", 48, reflectionSystem.GetClass<std::unordered_set<std::basic_string<char>>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<OnScopeExit>();
	currentClass->AddField(Field("myOnScopeExit", 0, reflectionSystem.GetClass<std::function<void ()>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryWriter>();
	currentClass->AddField(Field("myStream", 0, reflectionSystem.GetClass<std::basic_ofstream<char>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Vertex>();
	currentClass->AddField(Field("myPosition", 0, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myColor", 12, reflectionSystem.GetClass<int>()));
	currentClass->AddField(Field("myNormal", 16, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myTangents", 28, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myBinormals", 40, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myTexCoords", 52, reflectionSystem.GetClass<glm::vec<2, float>[2]>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Skeleton>();
	currentClass->AddField(Field("myJoints", 0, reflectionSystem.GetClass<List<Skeleton::Bone>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Skeleton::Bone>();
	currentClass->AddField(Field("myBindPosition", 0, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myBindRotation", 12, reflectionSystem.GetClass<glm::qua<float>>()));
	currentClass->AddField(Field("myBindScale", 28, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myIndexToParent", 40, reflectionSystem.GetClass<int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Engine>();
	currentClass->AddField(Field("myFrameIndex", 0, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myIsRunning", 4, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myEngineProperties", 8, reflectionSystem.GetClass<EngineProperties>()));
	currentClass->AddField(Field("mySystemManager", 120, reflectionSystem.GetClass<SystemManager<System> *>()));
	currentClass->AddField(Field("myPostMaster", 128, reflectionSystem.GetClass<EventHandler *>()));
	currentClass->AddField(Field("myThreadPool", 136, reflectionSystem.GetClass<ThreadPool *>()));
	currentClass->AddField(Field("myFilewatcher", 144, reflectionSystem.GetClass<Filewatcher *>()));
	currentClass->AddField(Field("myWindowHandler", 152, reflectionSystem.GetClass<WindowHandler *>()));
	currentClass->AddField(Field("myVulkanContext", 160, reflectionSystem.GetClass<VulkanContext *>()));
	currentClass->AddField(Field("myAssetRegistry", 168, reflectionSystem.GetClass<AssetRegistry *>()));
	currentClass->AddField(Field("myWorld", 176, reflectionSystem.GetClass<World *>()));
	currentClass->AddField(Field("myExternalTickFunction", 184, reflectionSystem.GetClass<std::function<void ()>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PromiseReturnTypeImplementation<void>>();
	currentClass->AddField(Field("myReturnValue", 0, reflectionSystem.GetClass<char>()));
	currentClass->AddField(Field("myHasReturnValue", 1, reflectionSystem.GetClass<bool>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightComponent>();
	currentClass->AddField(Field("myColor", 16, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myIntensity", 28, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myRange", 32, reflectionSystem.GetClass<float>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Asset>();
	currentClass->AddField(Field("myPath", 8, reflectionSystem.GetClass<std::filesystem::path>()));
	currentClass->AddField(Field("myIsValid", 40, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myAssetRegistry", 48, reflectionSystem.GetClass<AssetRegistry *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher>();
	currentClass->AddField(Field("myThread", 0, reflectionSystem.GetClass<std::thread>()));
	currentClass->AddField(Field("myLock", 16, reflectionSystem.GetClass<std::mutex>()));
	currentClass->AddField(Field("myShouldRun", 96, reflectionSystem.GetClass<std::atomic<bool>>()));
	currentClass->AddField(Field("myFilesToWatch", 104, reflectionSystem.GetClass<std::unordered_map<std::filesystem::path, Filewatcher::FileData>>()));
	currentClass->AddField(Field("myModifiedPaths", 168, reflectionSystem.GetClass<std::set<Filewatcher::FileData *>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher::CallbackHandle>();
	currentClass->AddField(Field("myID", 0, reflectionSystem.GetClass<int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher::FileData>();
	currentClass->AddField(Field("myCallbackIDIndexList", 0, reflectionSystem.GetClass<List<Filewatcher::CallbackHandle>>()));
	currentClass->AddField(Field("myCallbacks", 24, reflectionSystem.GetClass<List<std::function<void ()>>>()));
	currentClass->AddField(Field("myLastModifiedTime", 48, reflectionSystem.GetClass<std::chrono::time_point<std::filesystem::_File_time_clock>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IAssetContainer>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SerializationMeshData>();
	currentClass->AddField(Field("myVertices", 0, reflectionSystem.GetClass<List<Vertex>>()));
	currentClass->AddField(Field("myIndices", 24, reflectionSystem.GetClass<List<unsigned int>>()));
	currentClass->AddField(Field("mySphereCenterBounds", 48, reflectionSystem.GetClass<glm::vec<4, float>>()));
	currentClass->AddField(Field("myStagingVertexBuffer", 64, reflectionSystem.GetClass<VulkanBuffer *>()));
	currentClass->AddField(Field("myStagingIndexBuffer", 72, reflectionSystem.GetClass<VulkanBuffer *>()));
	currentClass->AddField(Field("myAlbedoPath", 80, reflectionSystem.GetClass<std::basic_string<char>>()));
	currentClass->AddField(Field("myNormalPath", 112, reflectionSystem.GetClass<std::basic_string<char>>()));
	currentClass->AddField(Field("myMaterialPath", 144, reflectionSystem.GetClass<std::basic_string<char>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Model>();
	currentClass->AddField(Field("myMeshes", 56, reflectionSystem.GetClass<List<Mesh *>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadPool>();
	currentClass->AddField(Field("myThreads", 0, reflectionSystem.GetClass<List<std::thread>>()));
	currentClass->AddField(Field("myLookingForTaskMutex", 24, reflectionSystem.GetClass<std::mutex>()));
	currentClass->AddField(Field("myWaitingForTaskCondition", 104, reflectionSystem.GetClass<std::condition_variable>()));
	currentClass->AddField(Field("myShouldExit", 176, reflectionSystem.GetClass<std::atomic<bool>>()));
	currentClass->AddField(Field("myTasks", 184, reflectionSystem.GetClass<std::queue<std::function<void ()>>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TextureCube>();
	currentClass->AddField(Field("myImage", 56, reflectionSystem.GetClass<VulkanImage *>()));
	currentClass->AddField(Field("myBindlessIndex", 64, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AssetRegistry>();
	currentClass->AddField(Field("myMutex", 0, reflectionSystem.GetClass<std::recursive_mutex>()));
	currentClass->AddField(Field("myContainers", 80, reflectionSystem.GetClass<List<IAssetContainer *>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SkyboxPipeline>();
	currentClass->AddField(Field("myVertexShader", 0, reflectionSystem.GetClass<Shader *>()));
	currentClass->AddField(Field("myFragmentShader", 8, reflectionSystem.GetClass<Shader *>()));
	currentClass->AddField(Field("myFrameDescriptorSet", 16, reflectionSystem.GetClass<VulkanDescriptorSet>()));
	currentClass->AddField(Field("myPipelineLayout", 112, reflectionSystem.GetClass<vk::PipelineLayout>()));
	currentClass->AddField(Field("myPipeline", 120, reflectionSystem.GetClass<vk::Pipeline>()));
	currentClass->AddField(Field("myFrameDataBuffer", 128, reflectionSystem.GetClass<VulkanBuffer *>()));
	currentClass->AddField(Field("mySkyboxModel", 136, reflectionSystem.GetClass<Model *>()));
	currentClass->AddField(Field("mySkybox", 144, reflectionSystem.GetClass<Texture *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ConvexColliderComponent>();
	currentClass->AddField(Field("myModel", 56, reflectionSystem.GetClass<Model *>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<JsonAsset>();
	currentClass->AddField(Field("myJson", 56, reflectionSystem.GetClass<nlohmann::basic_json<>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanAllocator>();
	currentClass->AddField(Field("myAllocator", 0, reflectionSystem.GetClass<VmaAllocator_T *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventHandler>();
	currentClass->AddField(Field("myObservers", 0, reflectionSystem.GetClass<List<EventObserver *>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanBuffer>();
	currentClass->AddField(Field("myAllocation", 0, reflectionSystem.GetClass<VmaAllocation_T *>()));
	currentClass->AddField(Field("myBuffer", 8, reflectionSystem.GetClass<vk::Buffer>()));
	currentClass->AddField(Field("myCreateInfo", 16, reflectionSystem.GetClass<vk::BufferCreateInfo>()));
	currentClass->AddField(Field("myMemoryUsage", 72, reflectionSystem.GetClass<VmaMemoryUsage>()));
	currentClass->AddField(Field("myIsMappingAllowed", 76, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myPtr", 80, reflectionSystem.GetClass<void *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventType>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CapsuleColliderComponent>();
	currentClass->AddField(Field("myRadius", 56, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myHeight", 60, reflectionSystem.GetClass<float>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventObserver>();
	currentClass->AddField(Field("myEvents", 8, reflectionSystem.GetClass<std::map<EventType, std::function<void ()>>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<System>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StaticMeshComponent>();
	currentClass->AddField(Field("myPath", 16, reflectionSystem.GetClass<std::filesystem::path>()));
	currentClass->AddField(Field("myMaterials", 48, reflectionSystem.GetClass<List<Material *>>()));
	currentClass->AddField(Field("myModel", 72, reflectionSystem.GetClass<Model *>()));
	currentClass->AddField(Field("myMeshInstances", 80, reflectionSystem.GetClass<List<unsigned int>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RenderSystem>();
	currentClass->AddField(Field("myIsUsingGPUDrivenRendering", 32, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myCopyPipeline", 40, reflectionSystem.GetClass<FullscreenPipeline *>()));
	currentClass->AddField(Field("myDebugPipeline", 48, reflectionSystem.GetClass<DebugPipeline *>()));
	currentClass->AddField(Field("myGDRPipeline", 56, reflectionSystem.GetClass<GDRPipeline *>()));
	currentClass->AddField(Field("mySkyboxPipeline", 64, reflectionSystem.GetClass<SkyboxPipeline *>()));
	currentClass->AddField(Field("myRenderPass", 72, reflectionSystem.GetClass<vk::RenderPass>()));
	currentClass->AddField(Field("myCopyToSwapchainRenderPass", 80, reflectionSystem.GetClass<vk::RenderPass>()));
	currentClass->AddField(Field("myVkFrameBuffer", 88, reflectionSystem.GetClass<vk::Framebuffer>()));
	currentClass->AddField(Field("myCopyToSwapchainFrameBuffers", 96, reflectionSystem.GetClass<List<vk::Framebuffer>>()));
	currentClass->AddField(Field("myClearValues", 120, reflectionSystem.GetClass<vk::ClearValue[2]>()));
	currentClass->AddField(Field("myDepthBuffer", 152, reflectionSystem.GetClass<VulkanImage *>()));
	currentClass->AddField(Field("myRenderTexture", 160, reflectionSystem.GetClass<VulkanImage *>()));
	currentClass->AddField(Field("myResolvedRenderTexture", 168, reflectionSystem.GetClass<VulkanImage *>()));
	currentClass->AddField(Field("myDirectionalLightShadowMap", 176, reflectionSystem.GetClass<VulkanImage *>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EventObserver>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<DirectionalLightComponent>();
	currentClass->AddField(Field("myIsShadowsEnabled", 16, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myColor", 20, reflectionSystem.GetClass<glm::vec<4, float>>()));
	currentClass->AddField(Field("myLightProjection", 36, reflectionSystem.GetClass<glm::mat<4, 4, float>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MathUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IVulkanDynamicBuffer>();
	currentClass->AddField(Field("OnBufferRecreated", 8, reflectionSystem.GetClass<MulticastDelegate<void ()>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GameObject>();
	currentClass->AddField(Field("OnComponentAdded", 0, reflectionSystem.GetClass<MulticastDelegate<void (Component *)>>()));
	currentClass->AddField(Field("OnComponentRemoved", 104, reflectionSystem.GetClass<MulticastDelegate<void (Component *)>>()));
	currentClass->AddField(Field("myComponentSystem", 208, reflectionSystem.GetClass<ComponentSystem *>()));
	currentClass->AddField(Field("myTransform", 216, reflectionSystem.GetClass<TransformComponent *>()));
	currentClass->AddField(Field("myRenderStateDirty", 224, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myPhysicsStateDirty", 225, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myTags", 228, reflectionSystem.GetClass<unsigned int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Material>();
	currentClass->AddField(Field("myAlbedoTexture", 56, reflectionSystem.GetClass<Texture *>()));
	currentClass->AddField(Field("myNormalTexture", 64, reflectionSystem.GetClass<Texture *>()));
	currentClass->AddField(Field("myMaterialTexture", 72, reflectionSystem.GetClass<Texture *>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Shader>();
	currentClass->AddField(Field("OnShaderRecompiled", 56, reflectionSystem.GetClass<MulticastDelegate<void ()>>()));
	currentClass->AddField(Field("myShaderModule", 160, reflectionSystem.GetClass<vk::ShaderModule>()));
	currentClass->AddField(Field("myCallbackHandle", 168, reflectionSystem.GetClass<Filewatcher::CallbackHandle>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IComponentArray>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshColliderComponent>();
	currentClass->AddField(Field("myModel", 56, reflectionSystem.GetClass<Model *>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Time>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<WorldSystem>();
	currentClass->AddField(Field("myWorld", 8, reflectionSystem.GetClass<World *>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeColliderComponent>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Component>();
	currentClass->AddField(Field("myGameObject", 8, reflectionSystem.GetClass<GameObject *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TransformComponent>();
	currentClass->AddField(Field("OnPositionChanged", 16, reflectionSystem.GetClass<MulticastDelegate<void ()>>()));
	currentClass->AddField(Field("OnRotationChanged", 120, reflectionSystem.GetClass<MulticastDelegate<void ()>>()));
	currentClass->AddField(Field("OnScaleChanged", 224, reflectionSystem.GetClass<MulticastDelegate<void ()>>()));
	currentClass->AddField(Field("myPositionDirty", 328, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myRotationDirty", 329, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myScaleDirty", 330, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myPosition", 332, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myRotation", 344, reflectionSystem.GetClass<glm::qua<float>>()));
	currentClass->AddField(Field("myScale", 360, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("mySkipPhysicsUpdate", 372, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myParent", 376, reflectionSystem.GetClass<TransformComponent *>()));
	currentClass->AddField(Field("myChildren", 384, reflectionSystem.GetClass<List<TransformComponent *>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Input>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Input::KeyCode>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Input::MouseButton>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CharacterControllerComponent>();
	currentClass->AddField(Field("myController", 16, reflectionSystem.GetClass<physx::PxController *>()));
	currentClass->AddField(Field("myCollisionFlags", 24, reflectionSystem.GetClass<physx::PxFlags<physx::PxControllerCollisionFlag::Enum, unsigned char>>()));
	currentClass->AddField(Field("myFilter", 32, reflectionSystem.GetClass<physx::PxControllerFilters>()));
	currentClass->AddField(Field("myHeight", 64, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myRadius", 68, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("mySlopeLimitDegrees", 72, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myMinDist", 76, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myStepOffset", 80, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myUseGravity", 84, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myDownVelocity", 88, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myGravity", 92, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myTerminalVelocity", 96, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myDisplacement", 100, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myPositionOffset", 112, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ForceMode>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RigidbodyComponent>();
	currentClass->AddField(Field("myActor", 16, reflectionSystem.GetClass<physx::PxRigidDynamic *>()));
	currentClass->AddField(Field("myFramesSinceStartSleep", 24, reflectionSystem.GetClass<int>()));
	currentClass->AddField(Field("myMass", 28, reflectionSystem.GetClass<float>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GPUSceneSystem>();
	currentClass->AddField(Field("myFreeSparseIndices", 8, reflectionSystem.GetClass<List<unsigned int>>()));
	currentClass->AddField(Field("myNextFreeSparseIndex", 32, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("mySparseBuffer", 40, reflectionSystem.GetClass<ResizableBuffer *>()));
	currentClass->AddField(Field("myNumGPUObjectUpdatesThisFrame", 48, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myDenseBuffer", 56, reflectionSystem.GetClass<ResizableBuffer *>()));
	currentClass->AddField(Field("myDenseBufferCPURepresentation", 64, reflectionSystem.GetClass<List<unsigned int, unsigned int>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ColliderComponent>();
	currentClass->AddField(Field("myShape", 16, reflectionSystem.GetClass<physx::PxShape *>()));
	currentClass->AddField(Field("myActor", 24, reflectionSystem.GetClass<physx::PxRigidStatic *>()));
	currentClass->AddField(Field("myLocalShapePosition", 32, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myLocalShapeRotation", 44, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BoxColliderComponent>();
	currentClass->AddField(Field("myHalfSize", 56, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeRenderComponent>();
	currentClass->AddField(Field("myHeightfield", 16, reflectionSystem.GetClass<Heightfield>()));
	currentClass->AddField(Field("myVertexBuffer", 280, reflectionSystem.GetClass<VertexBufferHandle *>()));
	currentClass->AddField(Field("myIndexBuffer", 288, reflectionSystem.GetClass<IndexBufferHandle *>()));
	currentClass->AddField(Field("myMesh", 296, reflectionSystem.GetClass<Mesh *>()));
	currentClass->AddField(Field("myMeshInstance", 304, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myMaterial", 312, reflectionSystem.GetClass<Material *>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CoroutineManager>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CameraComponent>();
	currentClass->AddField(Field("myResolution", 16, reflectionSystem.GetClass<glm::vec<2, float>>()));
	currentClass->AddField(Field("myProjection", 24, reflectionSystem.GetClass<glm::mat<4, 4, float>>()));
	currentClass->AddField(Field("myFov", 88, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myNearPlane", 92, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myFarPlane", 96, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myIsOrthographic", 100, reflectionSystem.GetClass<bool>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorCameraMovementComponent>();
	currentClass->AddField(Field("myResetMouseDelta", 16, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myMouseDelta", 20, reflectionSystem.GetClass<glm::vec<2, float>>()));
	currentClass->AddField(Field("myYaw", 28, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myPitch", 32, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myMovementSpeed", 36, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myShiftMultiplier", 40, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myScrollMultiplier", 44, reflectionSystem.GetClass<float>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsSystem>();
	currentClass->AddField(Field("myPhysicsCommands", 16, reflectionSystem.GetClass<List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>>()));
	currentClass->AddField(Field("myDefaultErrorCallback", 40, reflectionSystem.GetClass<physx::PxErrorCallback *>()));
	currentClass->AddField(Field("myDefaultAllocator", 48, reflectionSystem.GetClass<physx::PxDefaultAllocator *>()));
	currentClass->AddField(Field("myToleranceScale", 56, reflectionSystem.GetClass<physx::PxTolerancesScale *>()));
	currentClass->AddField(Field("myFoundation", 64, reflectionSystem.GetClass<physx::PxFoundation *>()));
	currentClass->AddField(Field("myPvd", 72, reflectionSystem.GetClass<physx::PxPvd *>()));
	currentClass->AddField(Field("myPvdTransport", 80, reflectionSystem.GetClass<physx::PxPvdTransport *>()));
	currentClass->AddField(Field("myDefaultMaterial", 88, reflectionSystem.GetClass<physx::PxMaterial *>()));
	currentClass->AddField(Field("myListener", 96, reflectionSystem.GetClass<PhysicsListener *>()));
	currentClass->AddField(Field("myPhysics", 104, reflectionSystem.GetClass<physx::PxPhysics *>()));
	currentClass->AddField(Field("myScene", 112, reflectionSystem.GetClass<physx::PxScene *>()));
	currentClass->AddField(Field("myControllerManager", 120, reflectionSystem.GetClass<physx::PxControllerManager *>()));
	currentClass->AddField(Field("myHasActiveSimulation", 128, reflectionSystem.GetClass<bool>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<WorldSystem>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Heightfield>();
	currentClass->AddField(Field("mySeed", 0, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myPerlinNoise", 4, reflectionSystem.GetClass<siv::BasicPerlinNoise<float>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SinWaveMovementComponent>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AutoInit>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AutoInitManager>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Awaitable>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadType>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsErrorCallback>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxErrorCallback>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SelectionSystem>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsListener>();
	currentClass->AddField(Field("myDequeueCollisionsDelegate", 32, reflectionSystem.GetClass<MulticastDelegate<void ()>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxSimulationEventCallback>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxUserControllerHitReport>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxControllerBehaviorCallback>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxControllerFilterCallback>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RaycastHit>();
	currentClass->AddField(Field("myHitPosition", 0, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myHitNormal", 12, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myHitDistance", 24, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myHitGameObject", 32, reflectionSystem.GetClass<GameObject *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Field>();
	currentClass->AddField(Field("myName", 0, reflectionSystem.GetClass<std::basic_string<char>>()));
	currentClass->AddField(Field("myOffset", 32, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myType", 40, reflectionSystem.GetClass<const Class *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Class>();
	currentClass->AddField(Field("myFullName", 0, reflectionSystem.GetClass<std::basic_string<char>>()));
	currentClass->AddField(Field("myClassName", 32, reflectionSystem.GetClass<std::basic_string<char>>()));
	currentClass->AddField(Field("myFactoryFunction", 64, reflectionSystem.GetClass<Delegate<void *()>>()));
	currentClass->AddField(Field("myBaseClasses", 72, reflectionSystem.GetClass<List<const Class *>>()));
	currentClass->AddField(Field("myDerivedClasses", 96, reflectionSystem.GetClass<List<const Class *>>()));
	currentClass->AddField(Field("myFields", 120, reflectionSystem.GetClass<List<Field>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ReflectionSystem>();
	currentClass->AddField(Field("myClasses", 8, reflectionSystem.GetClass<List<Class>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<DebugPipeline>();
	currentClass->AddField(Field("myVertexShader", 0, reflectionSystem.GetClass<Shader *>()));
	currentClass->AddField(Field("myFragmentShader", 8, reflectionSystem.GetClass<Shader *>()));
	currentClass->AddField(Field("myPipeline", 16, reflectionSystem.GetClass<vk::Pipeline>()));
	currentClass->AddField(Field("myPipelineLayout", 24, reflectionSystem.GetClass<vk::PipelineLayout>()));
	currentClass->AddField(Field("myFrameDescriptorSet", 32, reflectionSystem.GetClass<VulkanDescriptorSet>()));
	currentClass->AddField(Field("myFrameDataBuffer", 128, reflectionSystem.GetClass<VulkanBuffer *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<FullscreenPipeline>();
	currentClass->AddField(Field("myVertexShader", 0, reflectionSystem.GetClass<Shader *>()));
	currentClass->AddField(Field("myFragmentShader", 8, reflectionSystem.GetClass<Shader *>()));
	currentClass->AddField(Field("myPipeline", 16, reflectionSystem.GetClass<vk::Pipeline>()));
	currentClass->AddField(Field("myPipelineLayout", 24, reflectionSystem.GetClass<vk::PipelineLayout>()));
	currentClass->AddField(Field("myRenderPass", 32, reflectionSystem.GetClass<vk::RenderPass>()));
	currentClass->AddField(Field("myDescriptorSet", 40, reflectionSystem.GetClass<VulkanDescriptorSet>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline>();
	currentClass->AddField(Field("myPrePassShader", 0, reflectionSystem.GetClass<Shader *>()));
	currentClass->AddField(Field("myCullShader", 8, reflectionSystem.GetClass<Shader *>()));
	currentClass->AddField(Field("myCubemap", 16, reflectionSystem.GetClass<TextureCube *>()));
	currentClass->AddField(Field("myIndirectCommandsBuffer", 88, reflectionSystem.GetClass<ResizableBuffer *>()));
	currentClass->AddField(Field("myCountBuffer", 96, reflectionSystem.GetClass<VulkanBuffer *>()));
	currentClass->AddField(Field("myPerDrawDataBuffer", 104, reflectionSystem.GetClass<ResizableBuffer *>()));
	currentClass->AddField(Field("myFrameDescriptorSet", 112, reflectionSystem.GetClass<VulkanDescriptorSet>()));
	currentClass->AddField(Field("myPipelineLayout", 208, reflectionSystem.GetClass<vk::PipelineLayout>()));
	currentClass->AddField(Field("myPipeline", 216, reflectionSystem.GetClass<vk::Pipeline>()));
	currentClass->AddField(Field("myVertexShader", 224, reflectionSystem.GetClass<Shader *>()));
	currentClass->AddField(Field("myFragmentShader", 232, reflectionSystem.GetClass<Shader *>()));
	currentClass->AddField(Field("myFrameDataBuffer", 240, reflectionSystem.GetClass<VulkanBuffer *>()));
	currentClass->AddField(Field("myDirectionalLightBuffer", 248, reflectionSystem.GetClass<VulkanBuffer *>()));
	currentClass->AddField(Field("myDirtyTransforms", 256, reflectionSystem.GetClass<List<TransformComponent *>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferHandle>();
	currentClass->AddField(Field("myIndex", 0, reflectionSystem.GetClass<unsigned int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshData>();
	currentClass->AddField(Field("myBoundingSphereModelSpace", 0, reflectionSystem.GetClass<glm::vec<4, float>>()));
	currentClass->AddField(Field("myVertexIndex", 16, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myIndexDataIndex", 20, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myMaterialIndex", 24, reflectionSystem.GetClass<unsigned int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferData>();
	currentClass->AddField(Field("myOffset", 0, reflectionSystem.GetClass<unsigned int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferData>();
	currentClass->AddField(Field("myOffset", 0, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myCount", 4, reflectionSystem.GetClass<unsigned int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshInstanceData>();
	currentClass->AddField(Field("myToWorld", 0, reflectionSystem.GetClass<glm::mat<4, 4, float>>()));
	currentClass->AddField(Field("myMeshIndex", 64, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myAlbedoIndex", 68, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myNormalIndex", 72, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myMaterialIndex", 76, reflectionSystem.GetClass<unsigned int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PerDrawData>();
	currentClass->AddField(Field("myToWorld", 0, reflectionSystem.GetClass<glm::mat<4, 4, float>>()));
	currentClass->AddField(Field("myAlbedoIndex", 64, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myNormalIndex", 68, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myMaterialIndex", 72, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("padding", 76, reflectionSystem.GetClass<unsigned int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightData>();
	currentClass->AddField(Field("myColor", 0, reflectionSystem.GetClass<glm::vec<4, float>>()));
	currentClass->AddField(Field("myPosition", 16, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myRange", 28, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myIntensity", 32, reflectionSystem.GetClass<float>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferSystem>();
	currentClass->AddField(Field("myUsedBufferSize", 8, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myCurrentIndexOffset", 12, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myIndexBuffers", 16, reflectionSystem.GetClass<List<IndexBufferHandle *>>()));
	currentClass->AddField(Field("myBuffer", 40, reflectionSystem.GetClass<ResizableBuffer *>()));
	currentClass->AddField(Field("mySparseIndexDataBuffer", 48, reflectionSystem.GetClass<ResizableBuffer *>()));
	currentClass->AddField(Field("mySparseIndexData_CPURepresentation", 56, reflectionSystem.GetClass<List<IndexBufferData>>()));
	currentClass->AddField(Field("myFreeSparseIndices", 80, reflectionSystem.GetClass<List<unsigned int>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Mesh>();
	currentClass->AddField(Field("myVertexBuffer", 0, reflectionSystem.GetClass<VertexBufferHandle *>()));
	currentClass->AddField(Field("myIndexBuffer", 8, reflectionSystem.GetClass<IndexBufferHandle *>()));
	currentClass->AddField(Field("mySphereBounds", 16, reflectionSystem.GetClass<glm::vec<4, float>>()));
	currentClass->AddField(Field("myHandle", 32, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myAlbedoPath", 40, reflectionSystem.GetClass<std::basic_string<char>>()));
	currentClass->AddField(Field("myNormalPath", 72, reflectionSystem.GetClass<std::basic_string<char>>()));
	currentClass->AddField(Field("myMaterialPath", 104, reflectionSystem.GetClass<std::basic_string<char>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshSystem>();
	currentClass->AddField(Field("myMeshes", 8, reflectionSystem.GetClass<List<Mesh *>>()));
	currentClass->AddField(Field("myBuffer", 32, reflectionSystem.GetClass<ResizableBuffer *>()));
	currentClass->AddField(Field("myNumObjects", 40, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferHandle>();
	currentClass->AddField(Field("myIndex", 0, reflectionSystem.GetClass<unsigned int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TextureSystem>();
	currentClass->AddField(Field("myDescriptorPool", 8, reflectionSystem.GetClass<vk::DescriptorPool>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ResizableBuffer>();
	currentClass->AddField(Field("OnBufferResized", 0, reflectionSystem.GetClass<MulticastDelegate<void ()>>()));
	currentClass->AddField(Field("myHasActiveUpload", 104, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myHasRegisteredForTick", 105, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myBuffer", 112, reflectionSystem.GetClass<VulkanBuffer *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferSystem>();
	currentClass->AddField(Field("myUsedBufferSize", 8, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myCurrentVertexOffset", 12, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myVertexBuffers", 16, reflectionSystem.GetClass<List<VertexBufferHandle *>>()));
	currentClass->AddField(Field("myBuffer", 40, reflectionSystem.GetClass<ResizableBuffer *>()));
	currentClass->AddField(Field("mySparseVertexDataBuffer", 48, reflectionSystem.GetClass<ResizableBuffer *>()));
	currentClass->AddField(Field("mySparseVertexData_CPURepresentation", 56, reflectionSystem.GetClass<List<VertexBufferData>>()));
	currentClass->AddField(Field("myFreeSparseIndices", 80, reflectionSystem.GetClass<List<unsigned int>>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryReader>();
	currentClass->AddField(Field("myBuffer", 0, reflectionSystem.GetClass<List<unsigned char>>()));
	currentClass->AddField(Field("myReadOffset", 24, reflectionSystem.GetClass<unsigned long long>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeSystem>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<WorldSystem>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightSystem>();
	currentClass->AddField(Field("myBuffer", 8, reflectionSystem.GetClass<ResizableBuffer *>()));
	currentClass->AddField(Field("myNumPointLights", 16, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SamplerMode>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Debug>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Debug::DrawLineInfos>();
	currentClass->AddField(Field("myStart", 0, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myEnd", 12, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myColor", 24, reflectionSystem.GetClass<Color>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ShaderDatabase>();
	currentClass->AddField(Field("m_shaderBinaries", 0, reflectionSystem.GetClass<std::map<GFSDK_Aftermath_ShaderBinaryHash, std::vector<unsigned char>>>()));
	currentClass->AddField(Field("m_shaderBinariesWithDebugInfo", 16, reflectionSystem.GetClass<std::map<GFSDK_Aftermath_ShaderDebugName, std::vector<unsigned char>>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<NvidiaAftermathTracker>();
	currentClass->AddField(Field("m_initialized", 0, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("m_mutex", 8, reflectionSystem.GetClass<std::mutex>()));
	currentClass->AddField(Field("m_shaderDebugInfo", 88, reflectionSystem.GetClass<std::map<GFSDK_Aftermath_ShaderDebugInfoIdentifier, std::vector<unsigned char>>>()));
	currentClass->AddField(Field("m_shaderDatabase", 104, reflectionSystem.GetClass<ShaderDatabase>()));
	currentClass->AddField(Field("m_markerMap", 136, reflectionSystem.GetClass<const std::array<std::map<unsigned long long, std::basic_string<char>>, 4> &>()));
	currentClass->AddField(Field("m_AftermathFlags", 144, reflectionSystem.GetClass<const unsigned int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StagingBuffer>();
	currentClass->AddField(Field("myUnderlyingBuffer", 0, reflectionSystem.GetClass<VulkanBuffer *>()));
	currentClass->AddField(Field("mySize", 8, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myOffset", 12, reflectionSystem.GetClass<unsigned int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanContext>();
	currentClass->AddField(Field("myVulkanInstance", 0, reflectionSystem.GetClass<vk::Instance>()));
	currentClass->AddField(Field("myDebugMessenger", 8, reflectionSystem.GetClass<vk::DebugUtilsMessengerEXT>()));
	currentClass->AddField(Field("myPipelineCache", 16, reflectionSystem.GetClass<vk::PipelineCache>()));
	currentClass->AddField(Field("myPhysicalDevice", 24, reflectionSystem.GetClass<VulkanPhysicalDevice *>()));
	currentClass->AddField(Field("myDevice", 32, reflectionSystem.GetClass<VulkanDevice *>()));
	currentClass->AddField(Field("myAllocator", 40, reflectionSystem.GetClass<VulkanAllocator *>()));
	currentClass->AddField(Field("mySwapChain", 48, reflectionSystem.GetClass<VulkanSwapChain *>()));
	currentClass->AddField(Field("markerMap", 56, reflectionSystem.GetClass<std::array<std::map<unsigned long long, std::basic_string<char>>, 4>>()));
	currentClass->AddField(Field("myNvidiaAftermathDebugger", 120, reflectionSystem.GetClass<NvidiaAftermathTracker *>()));
	currentClass->AddField(Field("myDescriptorPool", 128, reflectionSystem.GetClass<vk::DescriptorPool>()));
	currentClass->AddField(Field("myExtensions", 136, reflectionSystem.GetClass<const List<const char *>>()));
	currentClass->AddField(Field("myLayers", 160, reflectionSystem.GetClass<const List<const char *>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StagingSystem>();
	currentClass->AddField(Field("myLastFrameIndex", 8, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanDevice>();
	currentClass->AddField(Field("myPhysicalDevice", 0, reflectionSystem.GetClass<const VulkanPhysicalDevice &>()));
	currentClass->AddField(Field("myDevice", 8, reflectionSystem.GetClass<vk::Device>()));
	currentClass->AddField(Field("myGraphicsQueue", 16, reflectionSystem.GetClass<vk::Queue>()));
	currentClass->AddField(Field("myComputeQueue", 24, reflectionSystem.GetClass<vk::Queue>()));
	currentClass->AddField(Field("myTransferQueue", 32, reflectionSystem.GetClass<vk::Queue>()));
	currentClass->AddField(Field("myCommandPoolsMutex", 40, reflectionSystem.GetClass<std::recursive_mutex>()));
	currentClass->AddField(Field("myCommandPools", 120, reflectionSystem.GetClass<std::map<std::thread::id, vk::CommandPool>>()));
	currentClass->AddField(Field("myComputeCommandPools", 136, reflectionSystem.GetClass<std::map<std::thread::id, vk::CommandPool>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanImage>();
	currentClass->AddField(Field("myAllocation", 0, reflectionSystem.GetClass<VmaAllocation_T *>()));
	currentClass->AddField(Field("myImage", 8, reflectionSystem.GetClass<vk::Image>()));
	currentClass->AddField(Field("myView", 16, reflectionSystem.GetClass<vk::ImageView>()));
	currentClass->AddField(Field("myFormat", 24, reflectionSystem.GetClass<vk::Format>()));
	currentClass->AddField(Field("mySize", 28, reflectionSystem.GetClass<glm::vec<2, float>>()));
	currentClass->AddField(Field("myNumMipLevels", 36, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myNumMSAASamples", 40, reflectionSystem.GetClass<vk::SampleCountFlagBits>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanImGui>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanPhysicalDevice>();
	currentClass->AddField(Field("myDevice", 0, reflectionSystem.GetClass<vk::PhysicalDevice>()));
	currentClass->AddField(Field("myFeatures", 8, reflectionSystem.GetClass<vk::PhysicalDeviceFeatures2>()));
	currentClass->AddField(Field("myMemoryProperties", 248, reflectionSystem.GetClass<vk::PhysicalDeviceMemoryProperties>()));
	currentClass->AddField(Field("myQueueFamilyProperties", 768, reflectionSystem.GetClass<List<vk::QueueFamilyProperties>>()));
	currentClass->AddField(Field("myMSAASampleCount", 792, reflectionSystem.GetClass<vk::SampleCountFlagBits>()));
	currentClass->AddField(Field("myGraphicsQueueIndex", 796, reflectionSystem.GetClass<int>()));
	currentClass->AddField(Field("myComputeQueueIndex", 800, reflectionSystem.GetClass<int>()));
	currentClass->AddField(Field("myTransferQueueIndex", 804, reflectionSystem.GetClass<int>()));
	currentClass->AddField(Field("myPresentQueueIndex", 808, reflectionSystem.GetClass<int>()));
	currentClass->AddField(Field("myDeviceExtensions", 816, reflectionSystem.GetClass<List<const char *>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanShaderIncluder>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<shaderc::CompileOptions::IncluderInterface>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanSwapChain>();
	currentClass->AddField(Field("myDevice", 0, reflectionSystem.GetClass<const VulkanDevice &>()));
	currentClass->AddField(Field("myWindowSurface", 8, reflectionSystem.GetClass<vk::SurfaceKHR>()));
	currentClass->AddField(Field("myFormat", 16, reflectionSystem.GetClass<vk::Format>()));
	currentClass->AddField(Field("myFences", 24, reflectionSystem.GetClass<List<vk::Fence>>()));
	currentClass->AddField(Field("myImageAcquiredSemaphores", 48, reflectionSystem.GetClass<List<vk::Semaphore>>()));
	currentClass->AddField(Field("myDrawCompleteSemaphores", 72, reflectionSystem.GetClass<List<vk::Semaphore>>()));
	currentClass->AddField(Field("myFrameIndex", 96, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("mySyncIndex", 100, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("mySwapChain", 104, reflectionSystem.GetClass<vk::SwapchainKHR>()));
	currentClass->AddField(Field("mySwapChainWidth", 112, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("mySwapChainHeight", 116, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myMinImageCount", 120, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myImages", 128, reflectionSystem.GetClass<List<vk::Image>>()));
	currentClass->AddField(Field("myImageViews", 152, reflectionSystem.GetClass<List<vk::ImageView>>()));
	currentClass->AddField(Field("myCommandPool", 176, reflectionSystem.GetClass<vk::CommandPool>()));
	currentClass->AddField(Field("myCommandBuffers", 184, reflectionSystem.GetClass<List<vk::CommandBuffer>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<WindowHandler>();
	currentClass->AddField(Field("myHWND", 0, reflectionSystem.GetClass<HWND__ *>()));
	currentClass->AddField(Field("myHInstance", 8, reflectionSystem.GetClass<HINSTANCE__ *>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<World>();
	currentClass->AddField(Field("myAssetRegistry", 8, reflectionSystem.GetClass<AssetRegistry *>()));
	currentClass->AddField(Field("mySystemManager", 16, reflectionSystem.GetClass<SystemManager<WorldSystem> *>()));
	currentClass->AddField(Field("myCactus", 24, reflectionSystem.GetClass<GameObject *>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<AutoInit>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Editor>();
	currentClass->AddField(Field("myWindows", 0, reflectionSystem.GetClass<List<EditorWindow *>>()));
	currentClass->AddField(Field("myNextID", 24, reflectionSystem.GetClass<int>()));
	currentClass->AddField(Field("myGameTickFunction", 32, reflectionSystem.GetClass<Delegate<void ()>>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorWindow>();
	currentClass->AddField(Field("myWindowName", 8, reflectionSystem.GetClass<std::basic_string<char>>()));
	currentClass->AddField(Field("myIsClosable", 40, reflectionSystem.GetClass<bool>()));
	currentClass->AddField(Field("myID", 44, reflectionSystem.GetClass<int>()));
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<HierarchyWindow>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorWindow>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<InspectorWindow>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorWindow>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Viewport>();
	currentClass->AddField(Field("myEditorCamera", 72, reflectionSystem.GetClass<EditorCameraMovementComponent *>()));
	currentClass->AddField(Field("myDescriptorSets", 80, reflectionSystem.GetClass<List<vk::DescriptorSet>>()));
	currentClass->AddField(Field("mySampler", 104, reflectionSystem.GetClass<vk::Sampler>()));
	currentClass->AddField(Field("myP0", 112, reflectionSystem.GetClass<ImVec2>()));
	currentClass->AddField(Field("myP1", 120, reflectionSystem.GetClass<ImVec2>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorWindow>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EventObserver>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PlayerCameraControllerComponent>();
	currentClass->AddField(Field("myMouseSensitivity", 16, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("mySpringArmChangeAmount", 20, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myMinSpringArmLength", 24, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myMaxSpringArmLength", 28, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myPitch", 32, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myYaw", 36, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myStoredPitch", 40, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myStoredYaw", 44, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myStoredSpringArmLength", 48, reflectionSystem.GetClass<float>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PlayerComponent>();
	currentClass->AddField(Field("mySpeed", 16, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("mySprintSpeed", 20, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myJumpForce", 24, reflectionSystem.GetClass<float>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SpringArmComponent>();
	currentClass->AddField(Field("myLength", 16, reflectionSystem.GetClass<float>()));
	currentClass->AddField(Field("myOffset", 20, reflectionSystem.GetClass<glm::vec<3, float>>()));
	currentClass->AddField(Field("myExclusionTags", 32, reflectionSystem.GetClass<unsigned int>()));
	currentClass->AddField(Field("myHitOffset", 36, reflectionSystem.GetClass<float>()));
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Game>();
}

        }
    }
};