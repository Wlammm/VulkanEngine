#pragma once
#include "Engine/Utils/StdIncludes.hpp"
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/Field.h"
#include "Engine/Reflection/ReflectionSystem.h"


// BEGIN INCLUDES FOR REFLECTED TYPES

#include "../Engine/Physics/PhysicsErrorCallback.h"
#include "../Editor/Windows/SelectionSystem.h"
#include "../Editor/Windows/HierarchyWindow.h"
#include "../Engine/Animation/Skeleton.h"
#include "../Editor/Editor.h"
#include "../Editor/Windows/Viewport.h"
#include "../Editor/EditorPch.h"
#include "../Engine/Components/EditorCameraMovementComponent.h"
#include "../Editor/Windows/EditorWindow.h"
#include "../Editor/Windows/InspectorWindow.h"
#include "../Editor/Windows/ImGuiPropertyDrawer.h"
#include "../Engine/Core/Filewatcher.h"
#include "../Engine/AssetRegistry/Asset.h"
#include "../Engine/Assets/Model.h"
#include "../Engine/AssetRegistry/AssetContainer.h"
#include "../Engine/Rendering/SkyboxPipeline.h"
#include "../Engine/Components/ConvexColliderComponent.h"
#include "../Engine/AssetRegistry/AssetRegistry.h"
#include "../Engine/Containers/MutexList.hpp"
#include "../Engine/Assets/JsonAsset.h"
#include "../Engine/Assets/Material.h"
#include "../Engine/Assets/Shader.h"
#include "../Engine/Assets/Texture.h"
#include "../Engine/Core/ThreadPool.h"
#include "../Engine/Assets/TextureCube.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Components/TransformComponent.h"
#include "../Engine/ComponentSystem/Component.h"
#include "../Engine/Core/Time.h"
#include "../Engine/Components/MeshColliderComponent.h"
#include "../Engine/ComponentSystem/ComponentArray.h"
#include "../Engine/Core/Console.h"
#include "../Engine/ComponentSystem/ComponentSystem.h"
#include "../Engine/Vulkan/VulkanDynamicBuffer.hpp"
#include "../Engine/ComponentSystem/GameObject.h"
#include "../Engine/Coroutines/CoroutineManager.h"
#include "../Engine/Components/LandscapeRenderComponent.h"
#include "../Engine/Components/BoxColliderComponent.h"
#include "../Engine/Components/CameraComponent.h"
#include "../Engine/Components/CapsuleColliderComponent.h"
#include "../Engine/Delegates/Internal/FreeFuncCtor.hpp"
#include "../Engine/Vulkan/GPUSceneSystem.h"
#include "../Engine/Components/RigidbodyComponent.h"
#include "../Engine/Components/CharacterControllerComponent.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Rendering/RenderSystem.h"
#include "../Engine/Delegates/Internal/LambdaFuncCtor.hpp"
#include "../Engine/Components/DirectionalLightComponent.h"
#include "../Engine/System/WorldSystem.h"
#include "../Engine/Components/LandscapeColliderComponent.h"
#include "../Engine/Containers/SegmentedList.h"
#include "../Engine/Components/PointLightComponent.h"
#include "../Engine/Coroutines/Coroutine.h"
#include "../Engine/Components/SinWaveMovementComponent.h"
#include "../Engine/Utils/String.hpp"
#include "../Engine/Components/SphereColliderComponent.h"
#include "../Engine/Components/StaticMeshComponent.h"
#include "../Engine/Containers/ContainerTypes.hpp"
#include "../Engine/Containers/List.hpp"
#include "../Engine/Core/AutoInit.h"
#include "../Engine/Core/AutoInitManager.h"
#include "../Engine/Core/CheckDefine.hpp"
#include "../Engine/Reflection/Field.h"
#include "../Engine/Delegates/Delegate.hpp"
#include "../Engine/Coroutines/Awaitable.h"
#include "../Engine/Core/EngineDefines.hpp"
#include "../Engine/Delegates/Internal/ConstMemberFuncCtor.hpp"
#include "../Engine/Delegates/Internal/FuncCtor.hpp"
#include "../Engine/Delegates/Internal/MemberFuncCtor.hpp"
#include "../Engine/Delegates/MulticastDelegate.hpp"
#include "../Engine/Engine.h"
#include "../Engine/EnginePch.h"
#include "../Engine/EngineProperties.hpp"
#include "../Engine/Events/EventHandler.h"
#include "../Engine/Events/EventObserver.h"
#include "../Engine/Events/EventTypes.hpp"
#include "../Engine/Math/Color.h"
#include "../Engine/Math/GlmUtils.hpp"
#include "../Engine/Physics/PhysicsSystem.h"
#include "../Engine/Math/Heightfield.h"
#include "../Engine/Math/LinearColor.h"
#include "../Engine/Physics/PhysXInclude.h"
#include "../Engine/Physics/PhysicsListener.h"
#include "../Engine/Physics/PhysicsQueryStructs.h"
#include "../Engine/Reflection/Class.h"
#include "../Engine/Reflection/ReflectionSystem.h"
#include "../Engine/Rendering/DebugPipeline.h"
#include "../Engine/Rendering/FullscreenPipeline.h"
#include "../Engine/Rendering/GDRPipeline.h"
#include "../Engine/Rendering/IndexBufferHandle.h"
#include "../Engine/Rendering/IndexBufferSystem.h"
#include "../Engine/Rendering/Mesh.h"
#include "../Engine/Rendering/MeshSystem.h"
#include "../Engine/Rendering/VertexBufferHandle.h"
#include "../Engine/Rendering/MeshUtils.h"
#include "../Engine/Rendering/TextureSystem.h"
#include "../Engine/Serialization/BinaryWriter.h"
#include "../Engine/Rendering/Vertex.hpp"
#include "../Engine/Rendering/VertexBufferSystem.h"
#include "../Engine/Serialization/BinaryReader.h"
#include "../Engine/Shaders/MeshStructs.hpp"
#include "../Engine/System/System.h"
#include "../Engine/System/SystemManager.hpp"
#include "../Engine/Systems/LandscapeSystem.h"
#include "../Engine/Vulkan/VulkanUtils.hpp"
#include "../Engine/Systems/PointLightSystem.h"
#include "../Engine/Utils/BinaryUtils.hpp"
#include "../Engine/Utils/CommonMacros.h"
#include "../Engine/Utils/Debug.h"
#include "../Engine/Utils/HashUtils.hpp"
#include "../Engine/Utils/MathUtils.hpp"
#include "../Engine/Utils/OnScopeExit.h"
#include "../Engine/Utils/StdIncludes.hpp"
#include "../Engine/Utils/ThreadUtils.hpp"
#include "../Engine/Vulkan/Aftermath/NvidiaAftermathTracker.h"
#include "../Engine/Vulkan/Aftermath/ShaderDatabase.h"
#include "../Engine/Vulkan/ResizableBuffer.h"
#include "../Engine/Vulkan/Staging/StagingBuffer.h"
#include "../Engine/Vulkan/Staging/StagingSystem.h"
#include "../Engine/Vulkan/VulkanAllocator.h"
#include "../Engine/Vulkan/VulkanBuffer.h"
#include "../Engine/Vulkan/VulkanContext.h"
#include "../Engine/Vulkan/VulkanDescriptorSet.h"
#include "../Engine/Vulkan/VulkanDevice.h"
#include "../Engine/Vulkan/VulkanImGui.h"
#include "../Engine/Vulkan/VulkanImage.h"
#include "../Engine/Vulkan/VulkanPhysicalDevice.h"
#include "../Engine/Vulkan/VulkanShaderIncluder.h"
#include "../Engine/Vulkan/VulkanSwapChain.h"
#include "../Engine/Windows/WindowHandler.h"
#include "../Engine/World/World.h"
#include "../Game/Components/Player/PlayerCameraControllerComponent.h"
#include "../Game/Components/Player/PlayerComponent.h"
#include "../Game/Components/SpringArmComponent.h"
#include "../Game/Game.h"
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
		    reflectionSystem.AddClass<PhysicsErrorCallback>("PhysicsErrorCallback", typeid(PhysicsErrorCallback).name());
reflectionSystem.AddClass<SelectionSystem>("SelectionSystem", typeid(SelectionSystem).name());
reflectionSystem.AddClass<HierarchyWindow>("HierarchyWindow", typeid(HierarchyWindow).name());
reflectionSystem.AddClass<Skeleton>("Skeleton", typeid(Skeleton).name());
reflectionSystem.AddClass<Skeleton::Bone>("Skeleton::Bone", typeid(Skeleton::Bone).name());
reflectionSystem.AddClass<Editor>("Editor", typeid(Editor).name());
reflectionSystem.AddClass<Viewport>("Viewport", typeid(Viewport).name());
reflectionSystem.AddClass<EditorCameraMovementComponent>("EditorCameraMovementComponent", typeid(EditorCameraMovementComponent).name());
reflectionSystem.AddClass<EditorWindow>("EditorWindow", typeid(EditorWindow).name());
reflectionSystem.AddClass<InspectorWindow>("InspectorWindow", typeid(InspectorWindow).name());
reflectionSystem.AddClass<ImGuiPropertyDrawer>("ImGuiPropertyDrawer", typeid(ImGuiPropertyDrawer).name());
reflectionSystem.AddClass<Filewatcher>("Filewatcher", typeid(Filewatcher).name());
reflectionSystem.AddClass<Filewatcher::CallbackHandle>("Filewatcher::CallbackHandle", typeid(Filewatcher::CallbackHandle).name());
reflectionSystem.AddClass<Filewatcher::FileData>("Filewatcher::FileData", typeid(Filewatcher::FileData).name());
reflectionSystem.AddClass<Asset>("Asset", typeid(Asset).name());
reflectionSystem.AddClass<SerializationMeshData>("SerializationMeshData", typeid(SerializationMeshData).name());
reflectionSystem.AddClass<Model>("Model", typeid(Model).name());
reflectionSystem.AddClass<IAssetContainer>("IAssetContainer", typeid(IAssetContainer).name());
reflectionSystem.AddClass<SkyboxPipeline>("SkyboxPipeline", typeid(SkyboxPipeline).name());
reflectionSystem.AddClass<ConvexColliderComponent>("ConvexColliderComponent", typeid(ConvexColliderComponent).name());
reflectionSystem.AddClass<AssetRegistry>("AssetRegistry", typeid(AssetRegistry).name());
reflectionSystem.AddClass<JsonAsset>("JsonAsset", typeid(JsonAsset).name());
reflectionSystem.AddClass<Material>("Material", typeid(Material).name());
reflectionSystem.AddClass<Shader>("Shader", typeid(Shader).name());
reflectionSystem.AddClass<ImageData>("ImageData", typeid(ImageData).name());
reflectionSystem.AddClass<Texture>("Texture", typeid(Texture).name());
reflectionSystem.AddClass<ThreadPool>("ThreadPool", typeid(ThreadPool).name());
reflectionSystem.AddClass<TextureCube>("TextureCube", typeid(TextureCube).name());
reflectionSystem.AddClass<Input>("Input", typeid(Input).name());
reflectionSystem.AddClass<Input::KeyCode>("Input::KeyCode", typeid(Input::KeyCode).name());
reflectionSystem.AddClass<Input::MouseButton>("Input::MouseButton", typeid(Input::MouseButton).name());
reflectionSystem.AddClass<TransformComponent>("TransformComponent", typeid(TransformComponent).name());
reflectionSystem.AddClass<Component>("Component", typeid(Component).name());
reflectionSystem.AddClass<Time>("Time", typeid(Time).name());
reflectionSystem.AddClass<MeshColliderComponent>("MeshColliderComponent", typeid(MeshColliderComponent).name());
reflectionSystem.AddClass<IComponentArray>("IComponentArray", typeid(IComponentArray).name());
reflectionSystem.AddClass<Console>("Console", typeid(Console).name());
reflectionSystem.AddClass<ComponentSystem>("ComponentSystem", typeid(ComponentSystem).name());
reflectionSystem.AddClass<IVulkanDynamicBuffer>("IVulkanDynamicBuffer", typeid(IVulkanDynamicBuffer).name());
reflectionSystem.AddClass<GameObject>("GameObject", typeid(GameObject).name());
reflectionSystem.AddClass<CoroutineManager>("CoroutineManager", typeid(CoroutineManager).name());
reflectionSystem.AddClass<LandscapeRenderComponent>("LandscapeRenderComponent", typeid(LandscapeRenderComponent).name());
reflectionSystem.AddClass<BoxColliderComponent>("BoxColliderComponent", typeid(BoxColliderComponent).name());
reflectionSystem.AddClass<CameraComponent>("CameraComponent", typeid(CameraComponent).name());
reflectionSystem.AddClass<CapsuleColliderComponent>("CapsuleColliderComponent", typeid(CapsuleColliderComponent).name());
reflectionSystem.AddClass<GPUSceneSystem>("GPUSceneSystem", typeid(GPUSceneSystem).name());
reflectionSystem.AddClass<ForceMode>("ForceMode", typeid(ForceMode).name());
reflectionSystem.AddClass<RigidbodyComponent>("RigidbodyComponent", typeid(RigidbodyComponent).name());
reflectionSystem.AddClass<CharacterControllerComponent>("CharacterControllerComponent", typeid(CharacterControllerComponent).name());
reflectionSystem.AddClass<ColliderComponent>("ColliderComponent", typeid(ColliderComponent).name());
reflectionSystem.AddClass<RenderSystem>("RenderSystem", typeid(RenderSystem).name());
reflectionSystem.AddClass<DirectionalLightComponent>("DirectionalLightComponent", typeid(DirectionalLightComponent).name());
reflectionSystem.AddClass<WorldSystem>("WorldSystem", typeid(WorldSystem).name());
reflectionSystem.AddClass<LandscapeColliderComponent>("LandscapeColliderComponent", typeid(LandscapeColliderComponent).name());
reflectionSystem.AddClass<PointLightComponent>("PointLightComponent", typeid(PointLightComponent).name());
reflectionSystem.AddClass<PromiseReturnTypeImplementation<void>>("PromiseReturnTypeImplementation<void>", typeid(PromiseReturnTypeImplementation<void>).name());
reflectionSystem.AddClass<SinWaveMovementComponent>("SinWaveMovementComponent", typeid(SinWaveMovementComponent).name());
reflectionSystem.AddClass<String>("String", typeid(String).name());
reflectionSystem.AddClass<SphereColliderComponent>("SphereColliderComponent", typeid(SphereColliderComponent).name());
reflectionSystem.AddClass<StaticMeshComponent>("StaticMeshComponent", typeid(StaticMeshComponent).name());
reflectionSystem.AddClass<IsCopyable<int>>("IsCopyable<int>", typeid(IsCopyable<int>).name());
reflectionSystem.AddClass<AutoInit>("AutoInit", typeid(AutoInit).name());
reflectionSystem.AddClass<AutoInitManager>("AutoInitManager", typeid(AutoInitManager).name());
reflectionSystem.AddClass<Field>("Field", typeid(Field).name());
reflectionSystem.AddClass<Awaitable>("Awaitable", typeid(Awaitable).name());
reflectionSystem.AddClass<ThreadType>("ThreadType", typeid(ThreadType).name());
reflectionSystem.AddClass<Engine>("Engine", typeid(Engine).name());
reflectionSystem.AddClass<EngineProperties>("EngineProperties", typeid(EngineProperties).name());
reflectionSystem.AddClass<EventHandler>("EventHandler", typeid(EventHandler).name());
reflectionSystem.AddClass<EventObserver>("EventObserver", typeid(EventObserver).name());
reflectionSystem.AddClass<EventType>("EventType", typeid(EventType).name());
reflectionSystem.AddClass<Color>("Color", typeid(Color).name());
reflectionSystem.AddClass<PhysicsSystem>("PhysicsSystem", typeid(PhysicsSystem).name());
reflectionSystem.AddClass<Heightfield>("Heightfield", typeid(Heightfield).name());
reflectionSystem.AddClass<LinearColor>("LinearColor", typeid(LinearColor).name());
reflectionSystem.AddClass<PhysicsListener>("PhysicsListener", typeid(PhysicsListener).name());
reflectionSystem.AddClass<RaycastHit>("RaycastHit", typeid(RaycastHit).name());
reflectionSystem.AddClass<Class>("Class", typeid(Class).name());
reflectionSystem.AddClass<ReflectionSystem>("ReflectionSystem", typeid(ReflectionSystem).name());
reflectionSystem.AddClass<DebugPipeline>("DebugPipeline", typeid(DebugPipeline).name());
reflectionSystem.AddClass<FullscreenPipeline>("FullscreenPipeline", typeid(FullscreenPipeline).name());
reflectionSystem.AddClass<GDRPipeline>("GDRPipeline", typeid(GDRPipeline).name());
reflectionSystem.AddClass<IndexBufferHandle>("IndexBufferHandle", typeid(IndexBufferHandle).name());
reflectionSystem.AddClass<IndexBufferSystem>("IndexBufferSystem", typeid(IndexBufferSystem).name());
reflectionSystem.AddClass<Mesh>("Mesh", typeid(Mesh).name());
reflectionSystem.AddClass<MeshSystem>("MeshSystem", typeid(MeshSystem).name());
reflectionSystem.AddClass<VertexBufferHandle>("VertexBufferHandle", typeid(VertexBufferHandle).name());
reflectionSystem.AddClass<MeshUtils>("MeshUtils", typeid(MeshUtils).name());
reflectionSystem.AddClass<TextureSystem>("TextureSystem", typeid(TextureSystem).name());
reflectionSystem.AddClass<BinaryWriter>("BinaryWriter", typeid(BinaryWriter).name());
reflectionSystem.AddClass<Vertex>("Vertex", typeid(Vertex).name());
reflectionSystem.AddClass<VertexBufferSystem>("VertexBufferSystem", typeid(VertexBufferSystem).name());
reflectionSystem.AddClass<BinaryReader>("BinaryReader", typeid(BinaryReader).name());
reflectionSystem.AddClass<MeshData>("MeshData", typeid(MeshData).name());
reflectionSystem.AddClass<VertexBufferData>("VertexBufferData", typeid(VertexBufferData).name());
reflectionSystem.AddClass<IndexBufferData>("IndexBufferData", typeid(IndexBufferData).name());
reflectionSystem.AddClass<MeshInstanceData>("MeshInstanceData", typeid(MeshInstanceData).name());
reflectionSystem.AddClass<PerDrawData>("PerDrawData", typeid(PerDrawData).name());
reflectionSystem.AddClass<PointLightData>("PointLightData", typeid(PointLightData).name());
reflectionSystem.AddClass<System>("System", typeid(System).name());
reflectionSystem.AddClass<LandscapeSystem>("LandscapeSystem", typeid(LandscapeSystem).name());
reflectionSystem.AddClass<SamplerMode>("SamplerMode", typeid(SamplerMode).name());
reflectionSystem.AddClass<VulkanUtils>("VulkanUtils", typeid(VulkanUtils).name());
reflectionSystem.AddClass<PointLightSystem>("PointLightSystem", typeid(PointLightSystem).name());
reflectionSystem.AddClass<BinaryUtils>("BinaryUtils", typeid(BinaryUtils).name());
reflectionSystem.AddClass<Debug>("Debug", typeid(Debug).name());
reflectionSystem.AddClass<Debug::DrawLineInfos>("Debug::DrawLineInfos", typeid(Debug::DrawLineInfos).name());
reflectionSystem.AddClass<MathUtils>("MathUtils", typeid(MathUtils).name());
reflectionSystem.AddClass<OnScopeExit>("OnScopeExit", typeid(OnScopeExit).name());
reflectionSystem.AddClass<ThreadUtils>("ThreadUtils", typeid(ThreadUtils).name());
reflectionSystem.AddClass<NvidiaAftermathTracker>("NvidiaAftermathTracker", typeid(NvidiaAftermathTracker).name());
reflectionSystem.AddClass<ShaderDatabase>("ShaderDatabase", typeid(ShaderDatabase).name());
reflectionSystem.AddClass<ResizableBuffer>("ResizableBuffer", typeid(ResizableBuffer).name());
reflectionSystem.AddClass<StagingBuffer>("StagingBuffer", typeid(StagingBuffer).name());
reflectionSystem.AddClass<StagingSystem>("StagingSystem", typeid(StagingSystem).name());
reflectionSystem.AddClass<VulkanAllocator>("VulkanAllocator", typeid(VulkanAllocator).name());
reflectionSystem.AddClass<VulkanBuffer>("VulkanBuffer", typeid(VulkanBuffer).name());
reflectionSystem.AddClass<VulkanContext>("VulkanContext", typeid(VulkanContext).name());
reflectionSystem.AddClass<VulkanDescriptorSet>("VulkanDescriptorSet", typeid(VulkanDescriptorSet).name());
reflectionSystem.AddClass<VulkanDevice>("VulkanDevice", typeid(VulkanDevice).name());
reflectionSystem.AddClass<VulkanImGui>("VulkanImGui", typeid(VulkanImGui).name());
reflectionSystem.AddClass<VulkanImage>("VulkanImage", typeid(VulkanImage).name());
reflectionSystem.AddClass<VulkanPhysicalDevice>("VulkanPhysicalDevice", typeid(VulkanPhysicalDevice).name());
reflectionSystem.AddClass<VulkanShaderIncluder>("VulkanShaderIncluder", typeid(VulkanShaderIncluder).name());
reflectionSystem.AddClass<VulkanSwapChain>("VulkanSwapChain", typeid(VulkanSwapChain).name());
reflectionSystem.AddClass<WindowHandler>("WindowHandler", typeid(WindowHandler).name());
reflectionSystem.AddClass<World>("World", typeid(World).name());
reflectionSystem.AddClass<PlayerCameraControllerComponent>("PlayerCameraControllerComponent", typeid(PlayerCameraControllerComponent).name());
reflectionSystem.AddClass<PlayerComponent>("PlayerComponent", typeid(PlayerComponent).name());
reflectionSystem.AddClass<SpringArmComponent>("SpringArmComponent", typeid(SpringArmComponent).name());
reflectionSystem.AddClass<Game>("Game", typeid(Game).name());

        }
        
        // Add all fields & add base classes.
        {
            { 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsErrorCallback>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxErrorCallback>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SelectionSystem>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<HierarchyWindow>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorWindow>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Skeleton>();
	{
		Field& currentField = currentClass->AddField(Field("myJoints", 0, reflectionSystem.GetOrCreateClass<List<Skeleton::Bone>>("List<Skeleton::Bone>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Skeleton::Bone>();
	{
		Field& currentField = currentClass->AddField(Field("myBindPosition", 0, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindRotation", 12, reflectionSystem.GetOrCreateClass<glm::qua<float>>("glm::qua<float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindScale", 28, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexToParent", 40, reflectionSystem.GetOrCreateClass<int>("int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Editor>();
	{
		Field& currentField = currentClass->AddField(Field("myWindows", 0, reflectionSystem.GetOrCreateClass<List<EditorWindow *>>("List<EditorWindow *>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNextID", 24, reflectionSystem.GetOrCreateClass<int>("int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGameTickFunction", 32, reflectionSystem.GetOrCreateClass<Delegate<void ()>>("Delegate<void ()>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Viewport>();
	{
		Field& currentField = currentClass->AddField(Field("myEditorCamera", 72, reflectionSystem.GetOrCreateClass<EditorCameraMovementComponent *>("EditorCameraMovementComponent *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorSets", 80, reflectionSystem.GetOrCreateClass<List<vk::DescriptorSet>>("List<vk::DescriptorSet>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySampler", 104, reflectionSystem.GetOrCreateClass<vk::Sampler>("vk::Sampler")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myP0", 112, reflectionSystem.GetOrCreateClass<ImVec2>("ImVec2")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myP1", 120, reflectionSystem.GetOrCreateClass<ImVec2>("ImVec2")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorWindow>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EventObserver>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorCameraMovementComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myResetMouseDelta", 16, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMouseDelta", 20, reflectionSystem.GetOrCreateClass<glm::vec<2, float>>("glm::vec<2, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myYaw", 28, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPitch", 32, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMovementSpeed", 36, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShiftMultiplier", 40, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myScrollMultiplier", 44, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorWindow>();
	{
		Field& currentField = currentClass->AddField(Field("myWindowName", 8, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsClosable", 40, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myID", 44, reflectionSystem.GetOrCreateClass<int>("int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<InspectorWindow>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorWindow>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ImGuiPropertyDrawer>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher>();
	{
		Field& currentField = currentClass->AddField(Field("myThread", 0, reflectionSystem.GetOrCreateClass<std::thread>("std::thread")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLock", 16, reflectionSystem.GetOrCreateClass<std::mutex>("std::mutex")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShouldRun", 96, reflectionSystem.GetOrCreateClass<std::atomic<bool>>("std::atomic<bool>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFilesToWatch", 104, reflectionSystem.GetOrCreateClass<std::unordered_map<std::filesystem::path, Filewatcher::FileData>>("std::unordered_map<std::filesystem::path, Filewatcher::FileData>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myModifiedPaths", 168, reflectionSystem.GetOrCreateClass<std::set<Filewatcher::FileData *>>("std::set<Filewatcher::FileData *>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher::CallbackHandle>();
	{
		Field& currentField = currentClass->AddField(Field("myID", 0, reflectionSystem.GetOrCreateClass<int>("int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher::FileData>();
	{
		Field& currentField = currentClass->AddField(Field("myCallbackIDIndexList", 0, reflectionSystem.GetOrCreateClass<List<Filewatcher::CallbackHandle>>("List<Filewatcher::CallbackHandle>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCallbacks", 24, reflectionSystem.GetOrCreateClass<List<std::function<void ()>>>("List<std::function<void ()>>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLastModifiedTime", 48, reflectionSystem.GetOrCreateClass<std::chrono::time_point<std::filesystem::_File_time_clock>>("std::chrono::time_point<std::filesystem::_File_time_clock>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Asset>();
	{
		Field& currentField = currentClass->AddField(Field("myPath", 8, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsValid", 40, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAssetRegistry", 48, reflectionSystem.GetOrCreateClass<AssetRegistry *>("AssetRegistry *")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SerializationMeshData>();
	{
		Field& currentField = currentClass->AddField(Field("myVertices", 0, reflectionSystem.GetOrCreateClass<List<Vertex>>("List<Vertex>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndices", 24, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySphereCenterBounds", 48, reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStagingVertexBuffer", 64, reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStagingIndexBuffer", 72, reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoPath", 80, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalPath", 112, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialPath", 144, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Model>();
	{
		Field& currentField = currentClass->AddField(Field("myMeshes", 56, reflectionSystem.GetOrCreateClass<List<Mesh *>>("List<Mesh *>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IAssetContainer>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SkyboxPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", 0, reflectionSystem.GetOrCreateClass<Shader *>("Shader *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", 8, reflectionSystem.GetOrCreateClass<Shader *>("Shader *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDescriptorSet", 16, reflectionSystem.GetOrCreateClass<VulkanDescriptorSet>("VulkanDescriptorSet")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", 112, reflectionSystem.GetOrCreateClass<vk::PipelineLayout>("vk::PipelineLayout")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", 120, reflectionSystem.GetOrCreateClass<vk::Pipeline>("vk::Pipeline")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", 128, reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkyboxModel", 136, reflectionSystem.GetOrCreateClass<Model *>("Model *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkybox", 144, reflectionSystem.GetOrCreateClass<Texture *>("Texture *")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ConvexColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myModel", 56, reflectionSystem.GetOrCreateClass<Model *>("Model *")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AssetRegistry>();
	{
		Field& currentField = currentClass->AddField(Field("myMutex", 0, reflectionSystem.GetOrCreateClass<std::recursive_mutex>("std::recursive_mutex")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myContainers", 80, reflectionSystem.GetOrCreateClass<List<IAssetContainer *>>("List<IAssetContainer *>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<JsonAsset>();
	{
		Field& currentField = currentClass->AddField(Field("myJson", 56, reflectionSystem.GetOrCreateClass<nlohmann::basic_json<>>("nlohmann::basic_json<>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Material>();
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoTexture", 56, reflectionSystem.GetOrCreateClass<Texture *>("Texture *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalTexture", 64, reflectionSystem.GetOrCreateClass<Texture *>("Texture *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialTexture", 72, reflectionSystem.GetOrCreateClass<Texture *>("Texture *")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Shader>();
	{
		Field& currentField = currentClass->AddField(Field("OnShaderRecompiled", 56, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShaderModule", 160, reflectionSystem.GetOrCreateClass<vk::ShaderModule>("vk::ShaderModule")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCallbackHandle", 168, reflectionSystem.GetOrCreateClass<Filewatcher::CallbackHandle>("Filewatcher::CallbackHandle")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ImageData>();
	{
		Field& currentField = currentClass->AddField(Field("mySourceFile", 0, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWidth", 32, reflectionSystem.GetOrCreateClass<int>("int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHeight", 36, reflectionSystem.GetOrCreateClass<int>("int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myChannels", 40, reflectionSystem.GetOrCreateClass<int>("int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumMipLevels", 44, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPixelData", 48, reflectionSystem.GetOrCreateClass<List<unsigned char>>("List<unsigned char>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Texture>();
	{
		Field& currentField = currentClass->AddField(Field("myImage", 56, reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindlessIndex", 64, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadPool>();
	{
		Field& currentField = currentClass->AddField(Field("myThreads", 0, reflectionSystem.GetOrCreateClass<List<std::thread>>("List<std::thread>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLookingForTaskMutex", 24, reflectionSystem.GetOrCreateClass<std::mutex>("std::mutex")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWaitingForTaskCondition", 104, reflectionSystem.GetOrCreateClass<std::condition_variable>("std::condition_variable")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShouldExit", 176, reflectionSystem.GetOrCreateClass<std::atomic<bool>>("std::atomic<bool>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTasks", 184, reflectionSystem.GetOrCreateClass<std::queue<std::function<void ()>>>("std::queue<std::function<void ()>>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TextureCube>();
	{
		Field& currentField = currentClass->AddField(Field("myImage", 56, reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindlessIndex", 64, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
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
	Class* currentClass = reflectionSystem.GetMutableClass<TransformComponent>();
	{
		Field& currentField = currentClass->AddField(Field("OnPositionChanged", 16, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("OnRotationChanged", 120, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("OnScaleChanged", 224, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPositionDirty", 328, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRotationDirty", 329, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myScaleDirty", 330, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPosition", 332, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
		currentField.AddMetadata("ExposeToEditor");
	}
	{
		Field& currentField = currentClass->AddField(Field("myRotation", 344, reflectionSystem.GetOrCreateClass<glm::qua<float>>("glm::qua<float>")));
		currentField.AddMetadata("ExposeToEditor");
	}
	{
		Field& currentField = currentClass->AddField(Field("myScale", 360, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
		currentField.AddMetadata("ExposeToEditor");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkipPhysicsUpdate", 372, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myParent", 376, reflectionSystem.GetOrCreateClass<TransformComponent *>("TransformComponent *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myChildren", 384, reflectionSystem.GetOrCreateClass<List<TransformComponent *>>("List<TransformComponent *>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Component>();
	{
		Field& currentField = currentClass->AddField(Field("myGameObject", 8, reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Time>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myModel", 56, reflectionSystem.GetOrCreateClass<Model *>("Model *")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IComponentArray>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Console>();
	{
		Field& currentField = currentClass->AddField(Field("myConsoleHandle", 0, reflectionSystem.GetOrCreateClass<void *>("void *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLogToFileEnabled", 8, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLogToFileStream", 16, reflectionSystem.GetOrCreateClass<std::basic_ofstream<char>>("std::basic_ofstream<char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCoutBuffer", 280, reflectionSystem.GetOrCreateClass<std::basic_streambuf<char> *>("std::basic_streambuf<char> *")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ComponentSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myObjects", 16, reflectionSystem.GetOrCreateClass<List<GameObject *>>("List<GameObject *>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myObjectsToDestory", 40, reflectionSystem.GetOrCreateClass<List<GameObject *>>("List<GameObject *>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComponentArrays", 64, reflectionSystem.GetOrCreateClass<List<IComponentArray *>>("List<IComponentArray *>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<WorldSystem>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IVulkanDynamicBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("OnBufferRecreated", 8, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GameObject>();
	{
		Field& currentField = currentClass->AddField(Field("OnComponentAdded", 0, reflectionSystem.GetOrCreateClass<MulticastDelegate<void (Component *)>>("MulticastDelegate<void (Component *)>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("OnComponentRemoved", 104, reflectionSystem.GetOrCreateClass<MulticastDelegate<void (Component *)>>("MulticastDelegate<void (Component *)>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComponentSystem", 208, reflectionSystem.GetOrCreateClass<ComponentSystem *>("ComponentSystem *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTransform", 216, reflectionSystem.GetOrCreateClass<TransformComponent *>("TransformComponent *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderStateDirty", 224, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPhysicsStateDirty", 225, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTags", 228, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CoroutineManager>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeRenderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myHeightfield", 16, reflectionSystem.GetOrCreateClass<Heightfield>("Heightfield")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexBuffer", 280, reflectionSystem.GetOrCreateClass<VertexBufferHandle *>("VertexBufferHandle *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexBuffer", 288, reflectionSystem.GetOrCreateClass<IndexBufferHandle *>("IndexBufferHandle *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMesh", 296, reflectionSystem.GetOrCreateClass<Mesh *>("Mesh *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMeshInstance", 304, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterial", 312, reflectionSystem.GetOrCreateClass<Material *>("Material *")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BoxColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myHalfSize", 56, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CameraComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myResolution", 16, reflectionSystem.GetOrCreateClass<glm::vec<2, float>>("glm::vec<2, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myProjection", 24, reflectionSystem.GetOrCreateClass<glm::mat<4, 4, float>>("glm::mat<4, 4, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFov", 88, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNearPlane", 92, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFarPlane", 96, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsOrthographic", 100, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CapsuleColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myRadius", 56, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHeight", 60, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GPUSceneSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myFreeSparseIndices", 8, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNextFreeSparseIndex", 32, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseBuffer", 40, reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumGPUObjectUpdatesThisFrame", 48, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDenseBuffer", 56, reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDenseBufferCPURepresentation", 64, reflectionSystem.GetOrCreateClass<List<unsigned int, unsigned int>>("List<unsigned int, unsigned int>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ForceMode>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RigidbodyComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myActor", 16, reflectionSystem.GetOrCreateClass<physx::PxRigidDynamic *>("physx::PxRigidDynamic *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFramesSinceStartSleep", 24, reflectionSystem.GetOrCreateClass<int>("int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMass", 28, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CharacterControllerComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myController", 16, reflectionSystem.GetOrCreateClass<physx::PxController *>("physx::PxController *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCollisionFlags", 24, reflectionSystem.GetOrCreateClass<physx::PxFlags<physx::PxControllerCollisionFlag::Enum, unsigned char>>("physx::PxFlags<physx::PxControllerCollisionFlag::Enum, unsigned char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFilter", 32, reflectionSystem.GetOrCreateClass<physx::PxControllerFilters>("physx::PxControllerFilters")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHeight", 64, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRadius", 68, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySlopeLimitDegrees", 72, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMinDist", 76, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStepOffset", 80, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myUseGravity", 84, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDownVelocity", 88, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGravity", 92, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTerminalVelocity", 96, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDisplacement", 100, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPositionOffset", 112, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myShape", 16, reflectionSystem.GetOrCreateClass<physx::PxShape *>("physx::PxShape *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myActor", 24, reflectionSystem.GetOrCreateClass<physx::PxRigidStatic *>("physx::PxRigidStatic *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLocalShapePosition", 32, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLocalShapeRotation", 44, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RenderSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myIsUsingGPUDrivenRendering", 32, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCopyPipeline", 40, reflectionSystem.GetOrCreateClass<FullscreenPipeline *>("FullscreenPipeline *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDebugPipeline", 48, reflectionSystem.GetOrCreateClass<DebugPipeline *>("DebugPipeline *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGDRPipeline", 56, reflectionSystem.GetOrCreateClass<GDRPipeline *>("GDRPipeline *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkyboxPipeline", 64, reflectionSystem.GetOrCreateClass<SkyboxPipeline *>("SkyboxPipeline *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderPass", 72, reflectionSystem.GetOrCreateClass<vk::RenderPass>("vk::RenderPass")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCopyToSwapchainRenderPass", 80, reflectionSystem.GetOrCreateClass<vk::RenderPass>("vk::RenderPass")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVkFrameBuffer", 88, reflectionSystem.GetOrCreateClass<vk::Framebuffer>("vk::Framebuffer")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCopyToSwapchainFrameBuffers", 96, reflectionSystem.GetOrCreateClass<List<vk::Framebuffer>>("List<vk::Framebuffer>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myClearValues", 120, reflectionSystem.GetOrCreateClass<vk::ClearValue[2]>("vk::ClearValue[2]")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDepthBuffer", 152, reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderTexture", 160, reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myResolvedRenderTexture", 168, reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirectionalLightShadowMap", 176, reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EventObserver>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<DirectionalLightComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myIsShadowsEnabled", 16, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myColor", 20, reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLightProjection", 36, reflectionSystem.GetOrCreateClass<glm::mat<4, 4, float>>("glm::mat<4, 4, float>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<WorldSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myWorld", 8, reflectionSystem.GetOrCreateClass<World *>("World *")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeColliderComponent>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myColor", 16, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIntensity", 28, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRange", 32, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PromiseReturnTypeImplementation<void>>();
	{
		Field& currentField = currentClass->AddField(Field("myReturnValue", 0, reflectionSystem.GetOrCreateClass<char>("char")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasReturnValue", 1, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SinWaveMovementComponent>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<String>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SphereColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myRadius", 56, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StaticMeshComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myPath", 16, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterials", 48, reflectionSystem.GetOrCreateClass<List<Material *>>("List<Material *>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myModel", 72, reflectionSystem.GetOrCreateClass<Model *>("Model *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMeshInstances", 80, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IsCopyable<int>>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AutoInit>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AutoInitManager>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Field>();
	{
		Field& currentField = currentClass->AddField(Field("myName", 0, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myOffset", 32, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myType", 40, reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMetadata", 48, reflectionSystem.GetOrCreateClass<List<std::basic_string<char>>>("List<std::basic_string<char>>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Awaitable>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadType>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Engine>();
	{
		Field& currentField = currentClass->AddField(Field("myFrameIndex", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsRunning", 4, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myEngineProperties", 8, reflectionSystem.GetOrCreateClass<EngineProperties>("EngineProperties")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySystemManager", 120, reflectionSystem.GetOrCreateClass<SystemManager<System> *>("SystemManager<System> *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPostMaster", 128, reflectionSystem.GetOrCreateClass<EventHandler *>("EventHandler *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myThreadPool", 136, reflectionSystem.GetOrCreateClass<ThreadPool *>("ThreadPool *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFilewatcher", 144, reflectionSystem.GetOrCreateClass<Filewatcher *>("Filewatcher *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWindowHandler", 152, reflectionSystem.GetOrCreateClass<WindowHandler *>("WindowHandler *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVulkanContext", 160, reflectionSystem.GetOrCreateClass<VulkanContext *>("VulkanContext *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAssetRegistry", 168, reflectionSystem.GetOrCreateClass<AssetRegistry *>("AssetRegistry *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWorld", 176, reflectionSystem.GetOrCreateClass<World *>("World *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myExternalTickFunction", 184, reflectionSystem.GetOrCreateClass<std::function<void ()>>("std::function<void ()>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EngineProperties>();
	{
		Field& currentField = currentClass->AddField(Field("Title", 0, reflectionSystem.GetOrCreateClass<std::basic_string<wchar_t>>("std::basic_string<wchar_t>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowWidth", 32, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowHeight", 36, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowX", 40, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowY", 44, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("StartupArguments", 48, reflectionSystem.GetOrCreateClass<std::unordered_set<std::basic_string<char>>>("std::unordered_set<std::basic_string<char>>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventHandler>();
	{
		Field& currentField = currentClass->AddField(Field("myObservers", 0, reflectionSystem.GetOrCreateClass<List<EventObserver *>>("List<EventObserver *>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventObserver>();
	{
		Field& currentField = currentClass->AddField(Field("myEvents", 8, reflectionSystem.GetOrCreateClass<std::map<EventType, std::function<void ()>>>("std::map<EventType, std::function<void ()>>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventType>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Color>();
	{
		Field& currentField = currentClass->AddField(Field("myR", 0, reflectionSystem.GetOrCreateClass<unsigned char>("unsigned char")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myG", 1, reflectionSystem.GetOrCreateClass<unsigned char>("unsigned char")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myB", 2, reflectionSystem.GetOrCreateClass<unsigned char>("unsigned char")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myA", 3, reflectionSystem.GetOrCreateClass<unsigned char>("unsigned char")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myPhysicsCommands", 16, reflectionSystem.GetOrCreateClass<List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>>("List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDefaultErrorCallback", 40, reflectionSystem.GetOrCreateClass<physx::PxErrorCallback *>("physx::PxErrorCallback *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDefaultAllocator", 48, reflectionSystem.GetOrCreateClass<physx::PxDefaultAllocator *>("physx::PxDefaultAllocator *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myToleranceScale", 56, reflectionSystem.GetOrCreateClass<physx::PxTolerancesScale *>("physx::PxTolerancesScale *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFoundation", 64, reflectionSystem.GetOrCreateClass<physx::PxFoundation *>("physx::PxFoundation *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPvd", 72, reflectionSystem.GetOrCreateClass<physx::PxPvd *>("physx::PxPvd *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPvdTransport", 80, reflectionSystem.GetOrCreateClass<physx::PxPvdTransport *>("physx::PxPvdTransport *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDefaultMaterial", 88, reflectionSystem.GetOrCreateClass<physx::PxMaterial *>("physx::PxMaterial *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myListener", 96, reflectionSystem.GetOrCreateClass<PhysicsListener *>("PhysicsListener *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPhysics", 104, reflectionSystem.GetOrCreateClass<physx::PxPhysics *>("physx::PxPhysics *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myScene", 112, reflectionSystem.GetOrCreateClass<physx::PxScene *>("physx::PxScene *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myControllerManager", 120, reflectionSystem.GetOrCreateClass<physx::PxControllerManager *>("physx::PxControllerManager *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasActiveSimulation", 128, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<WorldSystem>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Heightfield>();
	{
		Field& currentField = currentClass->AddField(Field("mySeed", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPerlinNoise", 4, reflectionSystem.GetOrCreateClass<siv::BasicPerlinNoise<float>>("siv::BasicPerlinNoise<float>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LinearColor>();
	{
		Field& currentField = currentClass->AddField(Field("myR", 0, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myG", 4, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myB", 8, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myA", 12, reflectionSystem.GetOrCreateClass<float>("float")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsListener>();
	{
		Field& currentField = currentClass->AddField(Field("myDequeueCollisionsDelegate", 32, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxSimulationEventCallback>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxUserControllerHitReport>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxControllerBehaviorCallback>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxControllerFilterCallback>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RaycastHit>();
	{
		Field& currentField = currentClass->AddField(Field("myHitPosition", 0, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitNormal", 12, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitDistance", 24, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitGameObject", 32, reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Class>();
	{
		Field& currentField = currentClass->AddField(Field("myFullName", 0, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myClassName", 32, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFactoryFunction", 64, reflectionSystem.GetOrCreateClass<Delegate<void *()>>("Delegate<void *()>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBaseClasses", 72, reflectionSystem.GetOrCreateClass<List<const Class *>>("List<const Class *>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDerivedClasses", 96, reflectionSystem.GetOrCreateClass<List<const Class *>>("List<const Class *>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFields", 120, reflectionSystem.GetOrCreateClass<List<Field>>("List<Field>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ReflectionSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myClasses", 8, reflectionSystem.GetOrCreateClass<List<Class *>>("List<Class *>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<DebugPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", 0, reflectionSystem.GetOrCreateClass<Shader *>("Shader *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", 8, reflectionSystem.GetOrCreateClass<Shader *>("Shader *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", 16, reflectionSystem.GetOrCreateClass<vk::Pipeline>("vk::Pipeline")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", 24, reflectionSystem.GetOrCreateClass<vk::PipelineLayout>("vk::PipelineLayout")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDescriptorSet", 32, reflectionSystem.GetOrCreateClass<VulkanDescriptorSet>("VulkanDescriptorSet")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", 128, reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<FullscreenPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", 0, reflectionSystem.GetOrCreateClass<Shader *>("Shader *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", 8, reflectionSystem.GetOrCreateClass<Shader *>("Shader *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", 16, reflectionSystem.GetOrCreateClass<vk::Pipeline>("vk::Pipeline")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", 24, reflectionSystem.GetOrCreateClass<vk::PipelineLayout>("vk::PipelineLayout")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderPass", 32, reflectionSystem.GetOrCreateClass<vk::RenderPass>("vk::RenderPass")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorSet", 40, reflectionSystem.GetOrCreateClass<VulkanDescriptorSet>("VulkanDescriptorSet")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myPrePassShader", 0, reflectionSystem.GetOrCreateClass<Shader *>("Shader *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCullShader", 8, reflectionSystem.GetOrCreateClass<Shader *>("Shader *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCubemap", 16, reflectionSystem.GetOrCreateClass<TextureCube *>("TextureCube *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndirectCommandsBuffer", 88, reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCountBuffer", 96, reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPerDrawDataBuffer", 104, reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDescriptorSet", 112, reflectionSystem.GetOrCreateClass<VulkanDescriptorSet>("VulkanDescriptorSet")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", 208, reflectionSystem.GetOrCreateClass<vk::PipelineLayout>("vk::PipelineLayout")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", 216, reflectionSystem.GetOrCreateClass<vk::Pipeline>("vk::Pipeline")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", 224, reflectionSystem.GetOrCreateClass<Shader *>("Shader *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", 232, reflectionSystem.GetOrCreateClass<Shader *>("Shader *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", 240, reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirectionalLightBuffer", 248, reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirtyTransforms", 256, reflectionSystem.GetOrCreateClass<List<TransformComponent *>>("List<TransformComponent *>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferHandle>();
	{
		Field& currentField = currentClass->AddField(Field("myIndex", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myUsedBufferSize", 8, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCurrentIndexOffset", 12, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexBuffers", 16, reflectionSystem.GetOrCreateClass<List<IndexBufferHandle *>>("List<IndexBufferHandle *>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 40, reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseIndexDataBuffer", 48, reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseIndexData_CPURepresentation", 56, reflectionSystem.GetOrCreateClass<List<IndexBufferData>>("List<IndexBufferData>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFreeSparseIndices", 80, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Mesh>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexBuffer", 0, reflectionSystem.GetOrCreateClass<VertexBufferHandle *>("VertexBufferHandle *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexBuffer", 8, reflectionSystem.GetOrCreateClass<IndexBufferHandle *>("IndexBufferHandle *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySphereBounds", 16, reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHandle", 32, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoPath", 40, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalPath", 72, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialPath", 104, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myMeshes", 8, reflectionSystem.GetOrCreateClass<List<Mesh *>>("List<Mesh *>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 32, reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumObjects", 40, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferHandle>();
	{
		Field& currentField = currentClass->AddField(Field("myIndex", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TextureSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorPool", 8, reflectionSystem.GetOrCreateClass<vk::DescriptorPool>("vk::DescriptorPool")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryWriter>();
	{
		Field& currentField = currentClass->AddField(Field("myStream", 0, reflectionSystem.GetOrCreateClass<std::basic_ofstream<char>>("std::basic_ofstream<char>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Vertex>();
	{
		Field& currentField = currentClass->AddField(Field("myPosition", 0, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myColor", 12, reflectionSystem.GetOrCreateClass<int>("int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormal", 16, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTangents", 28, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBinormals", 40, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTexCoords", 52, reflectionSystem.GetOrCreateClass<glm::vec<2, float>[2]>("glm::vec<2, float>[2]")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myUsedBufferSize", 8, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCurrentVertexOffset", 12, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexBuffers", 16, reflectionSystem.GetOrCreateClass<List<VertexBufferHandle *>>("List<VertexBufferHandle *>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 40, reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseVertexDataBuffer", 48, reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseVertexData_CPURepresentation", 56, reflectionSystem.GetOrCreateClass<List<VertexBufferData>>("List<VertexBufferData>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFreeSparseIndices", 80, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryReader>();
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 0, reflectionSystem.GetOrCreateClass<List<unsigned char>>("List<unsigned char>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myReadOffset", 24, reflectionSystem.GetOrCreateClass<unsigned long long>("unsigned long long")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshData>();
	{
		Field& currentField = currentClass->AddField(Field("myBoundingSphereModelSpace", 0, reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexIndex", 16, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexDataIndex", 20, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialIndex", 24, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferData>();
	{
		Field& currentField = currentClass->AddField(Field("myOffset", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferData>();
	{
		Field& currentField = currentClass->AddField(Field("myOffset", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCount", 4, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshInstanceData>();
	{
		Field& currentField = currentClass->AddField(Field("myToWorld", 0, reflectionSystem.GetOrCreateClass<glm::mat<4, 4, float>>("glm::mat<4, 4, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMeshIndex", 64, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoIndex", 68, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalIndex", 72, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialIndex", 76, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PerDrawData>();
	{
		Field& currentField = currentClass->AddField(Field("myToWorld", 0, reflectionSystem.GetOrCreateClass<glm::mat<4, 4, float>>("glm::mat<4, 4, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoIndex", 64, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalIndex", 68, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialIndex", 72, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("padding", 76, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightData>();
	{
		Field& currentField = currentClass->AddField(Field("myColor", 0, reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPosition", 16, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRange", 28, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIntensity", 32, reflectionSystem.GetOrCreateClass<float>("float")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<System>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeSystem>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<WorldSystem>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SamplerMode>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 8, reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumPointLights", 16, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Debug>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Debug::DrawLineInfos>();
	{
		Field& currentField = currentClass->AddField(Field("myStart", 0, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myEnd", 12, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myColor", 24, reflectionSystem.GetOrCreateClass<Color>("Color")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MathUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<OnScopeExit>();
	{
		Field& currentField = currentClass->AddField(Field("myOnScopeExit", 0, reflectionSystem.GetOrCreateClass<std::function<void ()>>("std::function<void ()>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<NvidiaAftermathTracker>();
	{
		Field& currentField = currentClass->AddField(Field("m_initialized", 0, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_mutex", 8, reflectionSystem.GetOrCreateClass<std::mutex>("std::mutex")));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_shaderDebugInfo", 88, reflectionSystem.GetOrCreateClass<std::map<GFSDK_Aftermath_ShaderDebugInfoIdentifier, std::vector<unsigned char>>>("std::map<GFSDK_Aftermath_ShaderDebugInfoIdentifier, std::vector<unsigned char>>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_shaderDatabase", 104, reflectionSystem.GetOrCreateClass<ShaderDatabase>("ShaderDatabase")));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_markerMap", 136, reflectionSystem.GetOrCreateClass<const std::array<std::map<unsigned long long, std::basic_string<char>>, 4> &>("const std::array<std::map<unsigned long long, std::basic_string<char>>, 4> &")));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_AftermathFlags", 144, reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ShaderDatabase>();
	{
		Field& currentField = currentClass->AddField(Field("m_shaderBinaries", 0, reflectionSystem.GetOrCreateClass<std::map<GFSDK_Aftermath_ShaderBinaryHash, std::vector<unsigned char>>>("std::map<GFSDK_Aftermath_ShaderBinaryHash, std::vector<unsigned char>>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_shaderBinariesWithDebugInfo", 16, reflectionSystem.GetOrCreateClass<std::map<GFSDK_Aftermath_ShaderDebugName, std::vector<unsigned char>>>("std::map<GFSDK_Aftermath_ShaderDebugName, std::vector<unsigned char>>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ResizableBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("OnBufferResized", 0, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasActiveUpload", 104, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasRegisteredForTick", 105, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 112, reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StagingBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("myUnderlyingBuffer", 0, reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySize", 8, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myOffset", 12, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StagingSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myLastFrameIndex", 8, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanAllocator>();
	{
		Field& currentField = currentClass->AddField(Field("myAllocator", 0, reflectionSystem.GetOrCreateClass<VmaAllocator_T *>("VmaAllocator_T *")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("myAllocation", 0, reflectionSystem.GetOrCreateClass<VmaAllocation_T *>("VmaAllocation_T *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 8, reflectionSystem.GetOrCreateClass<vk::Buffer>("vk::Buffer")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCreateInfo", 16, reflectionSystem.GetOrCreateClass<vk::BufferCreateInfo>("vk::BufferCreateInfo")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMemoryUsage", 72, reflectionSystem.GetOrCreateClass<VmaMemoryUsage>("VmaMemoryUsage")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsMappingAllowed", 76, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPtr", 80, reflectionSystem.GetOrCreateClass<void *>("void *")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanContext>();
	{
		Field& currentField = currentClass->AddField(Field("myVulkanInstance", 0, reflectionSystem.GetOrCreateClass<vk::Instance>("vk::Instance")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDebugMessenger", 8, reflectionSystem.GetOrCreateClass<vk::DebugUtilsMessengerEXT>("vk::DebugUtilsMessengerEXT")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineCache", 16, reflectionSystem.GetOrCreateClass<vk::PipelineCache>("vk::PipelineCache")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPhysicalDevice", 24, reflectionSystem.GetOrCreateClass<VulkanPhysicalDevice *>("VulkanPhysicalDevice *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDevice", 32, reflectionSystem.GetOrCreateClass<VulkanDevice *>("VulkanDevice *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAllocator", 40, reflectionSystem.GetOrCreateClass<VulkanAllocator *>("VulkanAllocator *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChain", 48, reflectionSystem.GetOrCreateClass<VulkanSwapChain *>("VulkanSwapChain *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("markerMap", 56, reflectionSystem.GetOrCreateClass<std::array<std::map<unsigned long long, std::basic_string<char>>, 4>>("std::array<std::map<unsigned long long, std::basic_string<char>>, 4>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNvidiaAftermathDebugger", 120, reflectionSystem.GetOrCreateClass<NvidiaAftermathTracker *>("NvidiaAftermathTracker *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorPool", 128, reflectionSystem.GetOrCreateClass<vk::DescriptorPool>("vk::DescriptorPool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myExtensions", 136, reflectionSystem.GetOrCreateClass<const List<const char *>>("const List<const char *>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLayers", 160, reflectionSystem.GetOrCreateClass<const List<const char *>>("const List<const char *>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanDescriptorSet>();
	{
		Field& currentField = currentClass->AddField(Field("myUsesSharedLayout", 72, reflectionSystem.GetOrCreateClass<bool>("bool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLayout", 80, reflectionSystem.GetOrCreateClass<vk::DescriptorSetLayout>("vk::DescriptorSetLayout")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySet", 88, reflectionSystem.GetOrCreateClass<vk::DescriptorSet>("vk::DescriptorSet")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanDevice>();
	{
		Field& currentField = currentClass->AddField(Field("myPhysicalDevice", 0, reflectionSystem.GetOrCreateClass<const VulkanPhysicalDevice &>("const VulkanPhysicalDevice &")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDevice", 8, reflectionSystem.GetOrCreateClass<vk::Device>("vk::Device")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGraphicsQueue", 16, reflectionSystem.GetOrCreateClass<vk::Queue>("vk::Queue")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComputeQueue", 24, reflectionSystem.GetOrCreateClass<vk::Queue>("vk::Queue")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTransferQueue", 32, reflectionSystem.GetOrCreateClass<vk::Queue>("vk::Queue")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandPoolsMutex", 40, reflectionSystem.GetOrCreateClass<std::recursive_mutex>("std::recursive_mutex")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandPools", 120, reflectionSystem.GetOrCreateClass<std::map<std::thread::id, vk::CommandPool>>("std::map<std::thread::id, vk::CommandPool>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComputeCommandPools", 136, reflectionSystem.GetOrCreateClass<std::map<std::thread::id, vk::CommandPool>>("std::map<std::thread::id, vk::CommandPool>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanImGui>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanImage>();
	{
		Field& currentField = currentClass->AddField(Field("myAllocation", 0, reflectionSystem.GetOrCreateClass<VmaAllocation_T *>("VmaAllocation_T *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImage", 8, reflectionSystem.GetOrCreateClass<vk::Image>("vk::Image")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myView", 16, reflectionSystem.GetOrCreateClass<vk::ImageView>("vk::ImageView")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFormat", 24, reflectionSystem.GetOrCreateClass<vk::Format>("vk::Format")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySize", 28, reflectionSystem.GetOrCreateClass<glm::vec<2, float>>("glm::vec<2, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumMipLevels", 36, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumMSAASamples", 40, reflectionSystem.GetOrCreateClass<vk::SampleCountFlagBits>("vk::SampleCountFlagBits")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanPhysicalDevice>();
	{
		Field& currentField = currentClass->AddField(Field("myDevice", 0, reflectionSystem.GetOrCreateClass<vk::PhysicalDevice>("vk::PhysicalDevice")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFeatures", 8, reflectionSystem.GetOrCreateClass<vk::PhysicalDeviceFeatures2>("vk::PhysicalDeviceFeatures2")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMemoryProperties", 248, reflectionSystem.GetOrCreateClass<vk::PhysicalDeviceMemoryProperties>("vk::PhysicalDeviceMemoryProperties")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myQueueFamilyProperties", 768, reflectionSystem.GetOrCreateClass<List<vk::QueueFamilyProperties>>("List<vk::QueueFamilyProperties>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMSAASampleCount", 792, reflectionSystem.GetOrCreateClass<vk::SampleCountFlagBits>("vk::SampleCountFlagBits")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGraphicsQueueIndex", 796, reflectionSystem.GetOrCreateClass<int>("int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComputeQueueIndex", 800, reflectionSystem.GetOrCreateClass<int>("int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTransferQueueIndex", 804, reflectionSystem.GetOrCreateClass<int>("int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPresentQueueIndex", 808, reflectionSystem.GetOrCreateClass<int>("int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDeviceExtensions", 816, reflectionSystem.GetOrCreateClass<List<const char *>>("List<const char *>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanShaderIncluder>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<shaderc::CompileOptions::IncluderInterface>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanSwapChain>();
	{
		Field& currentField = currentClass->AddField(Field("myDevice", 0, reflectionSystem.GetOrCreateClass<const VulkanDevice &>("const VulkanDevice &")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWindowSurface", 8, reflectionSystem.GetOrCreateClass<vk::SurfaceKHR>("vk::SurfaceKHR")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFormat", 16, reflectionSystem.GetOrCreateClass<vk::Format>("vk::Format")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFences", 24, reflectionSystem.GetOrCreateClass<List<vk::Fence>>("List<vk::Fence>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImageAcquiredSemaphores", 48, reflectionSystem.GetOrCreateClass<List<vk::Semaphore>>("List<vk::Semaphore>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDrawCompleteSemaphores", 72, reflectionSystem.GetOrCreateClass<List<vk::Semaphore>>("List<vk::Semaphore>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameIndex", 96, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySyncIndex", 100, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChain", 104, reflectionSystem.GetOrCreateClass<vk::SwapchainKHR>("vk::SwapchainKHR")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChainWidth", 112, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChainHeight", 116, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMinImageCount", 120, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImages", 128, reflectionSystem.GetOrCreateClass<List<vk::Image>>("List<vk::Image>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImageViews", 152, reflectionSystem.GetOrCreateClass<List<vk::ImageView>>("List<vk::ImageView>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandPool", 176, reflectionSystem.GetOrCreateClass<vk::CommandPool>("vk::CommandPool")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandBuffers", 184, reflectionSystem.GetOrCreateClass<List<vk::CommandBuffer>>("List<vk::CommandBuffer>")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<WindowHandler>();
	{
		Field& currentField = currentClass->AddField(Field("myHWND", 0, reflectionSystem.GetOrCreateClass<HWND__ *>("HWND__ *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHInstance", 8, reflectionSystem.GetOrCreateClass<HINSTANCE__ *>("HINSTANCE__ *")));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<World>();
	{
		Field& currentField = currentClass->AddField(Field("myAssetRegistry", 8, reflectionSystem.GetOrCreateClass<AssetRegistry *>("AssetRegistry *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySystemManager", 16, reflectionSystem.GetOrCreateClass<SystemManager<WorldSystem> *>("SystemManager<WorldSystem> *")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCactus", 24, reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<AutoInit>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PlayerCameraControllerComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myMouseSensitivity", 16, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySpringArmChangeAmount", 20, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMinSpringArmLength", 24, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaxSpringArmLength", 28, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPitch", 32, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myYaw", 36, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStoredPitch", 40, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStoredYaw", 44, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStoredSpringArmLength", 48, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PlayerComponent>();
	{
		Field& currentField = currentClass->AddField(Field("mySpeed", 16, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySprintSpeed", 20, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myJumpForce", 24, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SpringArmComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myLength", 16, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myOffset", 20, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myExclusionTags", 32, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitOffset", 36, reflectionSystem.GetOrCreateClass<float>("float")));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Game>();
}

        }
    }
};