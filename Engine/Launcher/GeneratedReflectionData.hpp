#pragma once
#include "Engine/Utils/StdIncludes.hpp"
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/Field.h"
#include "Engine/Reflection/ReflectionSystem.h"

// BEGIN INCLUDES FOR REFLECTED TYPES

#include "../Editor/Toolbar/File/SaveLoadToolbar.h"
#include "../Engine/Animation/Skeleton.h"
#include "../Editor/Editor.h"
#include "../Editor/EditorSystem/EditorSystem.h"
#include "../Editor/Windows/HierarchyWindow.h"
#include "../Editor/EditorSystem/ImGuiDemoSystem.h"
#include "../Editor/EditorPch.h"
#include "../Engine/Components/EditorCameraMovementComponent.h"
#include "../Editor/EditorSystem/EditorToolbar.h"
#include "../Editor/Toolbar/Themes/EditorThemes.h"
#include "../Engine/Components/CapsuleColliderComponent.h"
#include "../Editor/EditorSystem/SelectionSystem.h"
#include "../Editor/Utils/EditorConfirmPrompt.h"
#include "../Engine/Components/SinWaveMovementComponent.h"
#include "../Editor/Utils/ImGuiTextureUtils.h"
#include "../Editor/Windows/ContentBrowserWindow.h"
#include "../Editor/Windows/InspectorWindow.h"
#include "../Editor/Windows/EditorWindow.h"
#include "../Editor/Windows/ImGuiPropertyDrawer.h"
#include "../Editor/Windows/Viewport.h"
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
#include "../Engine/Reflection/Method.h"
#include "../Engine/Assets/Texture.h"
#include "../Engine/Core/ThreadPool.h"
#include "../Engine/Assets/TextureCube.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Components/TransformComponent.h"
#include "../Engine/ComponentSystem/Component.h"
#include "../Engine/Core/Time.h"
#include "../Engine/Serialization/TypeSerializers/TypeSerializer.h"
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
#include "../Engine/Vulkan/GPUSceneSystem.h"
#include "../Engine/Components/RigidbodyComponent.h"
#include "../Engine/Components/CharacterControllerComponent.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Components/DirectionalLightComponent.h"
#include "../Engine/System/WorldSystem.h"
#include "../Engine/Components/LandscapeColliderComponent.h"
#include "../Engine/Containers/SegmentedList.h"
#include "../Engine/Components/PointLightComponent.h"
#include "../Engine/Coroutines/Coroutine.h"
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
#include "../Engine/Delegates/Internal/FreeFuncCtor.hpp"
#include "../Engine/Delegates/Internal/FuncCtor.hpp"
#include "../Engine/Rendering/RenderSystem.h"
#include "../Engine/Delegates/Internal/LambdaFuncCtor.hpp"
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
#include "../Engine/Physics/PhysicsErrorCallback.h"
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
#include "../Engine/Serialization/BinarySerializer.h"
#include "../Engine/Serialization/TypeSerializers/ListSerializer.h"
#include "../Engine/Serialization/TypeSerializers/StringSerializer.h"
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
#include "../Engine/Serialization/TypeSerializers/PrimitiveSerializer.h"


// END INCLUDES FOR REFLECTED TYPES

template<typename T>
std::any WrapReturnValue(T&& value)
{
    using Decayed = std::decay_t<T>;
    
    if constexpr (std::is_reference_v<T>)
    {
        return std::any(std::ref(value));
    }
    else if constexpr (std::is_copy_constructible_v<Decayed>)
    {
        return std::any(std::forward<T>(value));
    }
    else
    {
        return std::any(std::make_shared<Decayed>(std::forward<T>(value)));
    }
}

class GeneratedReflectionData
{
public:
	static void RegisterReflectionData(Engine& inEngine)
	{
		ReflectionSystem& reflectionSystem = inEngine.GetReflectionSystem(); 

		// Create all classes.
		{
		    reflectionSystem.AddClass<SaveLoadToolbar>("SaveLoadToolbar", typeid(SaveLoadToolbar).name());
reflectionSystem.AddClass<Skeleton>("Skeleton", typeid(Skeleton).name());
reflectionSystem.AddClass<Skeleton::Bone>("Skeleton::Bone", typeid(Skeleton::Bone).name());
reflectionSystem.AddClass<Editor>("Editor", typeid(Editor).name());
reflectionSystem.AddClass<EditorSystem>("EditorSystem", typeid(EditorSystem).name());
reflectionSystem.AddClass<HierarchyWindow>("HierarchyWindow", typeid(HierarchyWindow).name());
reflectionSystem.AddClass<ImGuiDemoSystem>("ImGuiDemoSystem", typeid(ImGuiDemoSystem).name());
reflectionSystem.AddClass<EditorCameraMovementComponent>("EditorCameraMovementComponent", typeid(EditorCameraMovementComponent).name());
reflectionSystem.AddClass<EditorToolbar>("EditorToolbar", typeid(EditorToolbar).name());
reflectionSystem.AddClass<EditorThemes>("EditorThemes", typeid(EditorThemes).name());
reflectionSystem.AddClass<CapsuleColliderComponent>("CapsuleColliderComponent", typeid(CapsuleColliderComponent).name());
reflectionSystem.AddClass<SelectionSystem>("SelectionSystem", typeid(SelectionSystem).name());
reflectionSystem.AddClass<EditorConfirmPrompt>("EditorConfirmPrompt", typeid(EditorConfirmPrompt).name());
reflectionSystem.AddClass<SinWaveMovementComponent>("SinWaveMovementComponent", typeid(SinWaveMovementComponent).name());
reflectionSystem.AddClass<ImGuiTextureUtils>("ImGuiTextureUtils", typeid(ImGuiTextureUtils).name());
reflectionSystem.AddClass<ContentBrowserItem>("ContentBrowserItem", typeid(ContentBrowserItem).name());
reflectionSystem.AddClass<ContentBrowserWindow>("ContentBrowserWindow", typeid(ContentBrowserWindow).name());
reflectionSystem.AddClass<InspectorWindow>("InspectorWindow", typeid(InspectorWindow).name());
reflectionSystem.AddClass<EditorWindow>("EditorWindow", typeid(EditorWindow).name());
reflectionSystem.AddClass<ImGuiPropertyDrawer>("ImGuiPropertyDrawer", typeid(ImGuiPropertyDrawer).name());
reflectionSystem.AddClass<Viewport>("Viewport", typeid(Viewport).name());
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
reflectionSystem.AddClass<MethodArgument>("MethodArgument", typeid(MethodArgument).name());
reflectionSystem.AddClass<Method>("Method", typeid(Method).name());
reflectionSystem.AddClass<ImageData>("ImageData", typeid(ImageData).name());
reflectionSystem.AddClass<Texture>("Texture", typeid(Texture).name());
reflectionSystem.AddClass<ThreadPool>("ThreadPool", typeid(ThreadPool).name());
reflectionSystem.AddClass<TextureCube>("TextureCube", typeid(TextureCube).name());
reflectionSystem.AddClass<Input>("Input", typeid(Input).name());
reflectionSystem.AddClass<TransformComponent>("TransformComponent", typeid(TransformComponent).name());
reflectionSystem.AddClass<Component>("Component", typeid(Component).name());
reflectionSystem.AddClass<Time>("Time", typeid(Time).name());
reflectionSystem.AddClass<TypeSerializer>("TypeSerializer", typeid(TypeSerializer).name());
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
reflectionSystem.AddClass<GPUSceneSystem>("GPUSceneSystem", typeid(GPUSceneSystem).name());
reflectionSystem.AddClass<RigidbodyComponent>("RigidbodyComponent", typeid(RigidbodyComponent).name());
reflectionSystem.AddClass<CharacterControllerComponent>("CharacterControllerComponent", typeid(CharacterControllerComponent).name());
reflectionSystem.AddClass<ColliderComponent>("ColliderComponent", typeid(ColliderComponent).name());
reflectionSystem.AddClass<DirectionalLightComponent>("DirectionalLightComponent", typeid(DirectionalLightComponent).name());
reflectionSystem.AddClass<WorldSystem>("WorldSystem", typeid(WorldSystem).name());
reflectionSystem.AddClass<LandscapeColliderComponent>("LandscapeColliderComponent", typeid(LandscapeColliderComponent).name());
reflectionSystem.AddClass<PointLightComponent>("PointLightComponent", typeid(PointLightComponent).name());
reflectionSystem.AddClass<PromiseReturnTypeImplementation<void>>("PromiseReturnTypeImplementation<void>", typeid(PromiseReturnTypeImplementation<void>).name());
reflectionSystem.AddClass<String>("String", typeid(String).name());
reflectionSystem.AddClass<SphereColliderComponent>("SphereColliderComponent", typeid(SphereColliderComponent).name());
reflectionSystem.AddClass<StaticMeshComponent>("StaticMeshComponent", typeid(StaticMeshComponent).name());
reflectionSystem.AddClass<IsCopyable<int>>("IsCopyable<int>", typeid(IsCopyable<int>).name());
reflectionSystem.AddClass<IList>("IList", typeid(IList).name());
reflectionSystem.AddClass<List<std::basic_string<char>>>("List<std::basic_string<char>>", typeid(List<std::basic_string<char>>).name());
reflectionSystem.AddClass<List<MethodArgument>>("List<MethodArgument>", typeid(List<MethodArgument>).name());
reflectionSystem.AddClass<List<const Class *>>("List<const Class *>", typeid(List<const Class *>).name());
reflectionSystem.AddClass<List<Field>>("List<Field>", typeid(List<Field>).name());
reflectionSystem.AddClass<List<Method>>("List<Method>", typeid(List<Method>).name());
reflectionSystem.AddClass<List<ClassTemplateArgument>>("List<ClassTemplateArgument>", typeid(List<ClassTemplateArgument>).name());
reflectionSystem.AddClass<List<Class *>>("List<Class *>", typeid(List<Class *>).name());
reflectionSystem.AddClass<AutoInit>("AutoInit", typeid(AutoInit).name());
reflectionSystem.AddClass<AutoInitManager>("AutoInitManager", typeid(AutoInitManager).name());
reflectionSystem.AddClass<Field>("Field", typeid(Field).name());
reflectionSystem.AddClass<Delegate<void *(void *, const List<void *> &)>>("Delegate<void *(void *, const List<void *> &)>", typeid(Delegate<void *(void *, const List<void *> &)>).name());
reflectionSystem.AddClass<Delegate<void *()>>("Delegate<void *()>", typeid(Delegate<void *()>).name());
reflectionSystem.AddClass<Awaitable>("Awaitable", typeid(Awaitable).name());
reflectionSystem.AddClass<RenderSystem>("RenderSystem", typeid(RenderSystem).name());
reflectionSystem.AddClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>", typeid(MulticastDelegate<void ()>).name());
reflectionSystem.AddClass<MulticastDelegate<void (Component *)>>("MulticastDelegate<void (Component *)>", typeid(MulticastDelegate<void (Component *)>).name());
reflectionSystem.AddClass<TestClass>("TestClass", typeid(TestClass).name());
reflectionSystem.AddClass<Engine>("Engine", typeid(Engine).name());
reflectionSystem.AddClass<EngineProperties>("EngineProperties", typeid(EngineProperties).name());
reflectionSystem.AddClass<EventHandler>("EventHandler", typeid(EventHandler).name());
reflectionSystem.AddClass<EventObserver>("EventObserver", typeid(EventObserver).name());
reflectionSystem.AddClass<Color>("Color", typeid(Color).name());
reflectionSystem.AddClass<PhysicsSystem>("PhysicsSystem", typeid(PhysicsSystem).name());
reflectionSystem.AddClass<Heightfield>("Heightfield", typeid(Heightfield).name());
reflectionSystem.AddClass<LinearColor>("LinearColor", typeid(LinearColor).name());
reflectionSystem.AddClass<PhysicsErrorCallback>("PhysicsErrorCallback", typeid(PhysicsErrorCallback).name());
reflectionSystem.AddClass<PhysicsListener>("PhysicsListener", typeid(PhysicsListener).name());
reflectionSystem.AddClass<RaycastHit>("RaycastHit", typeid(RaycastHit).name());
reflectionSystem.AddClass<ClassTemplateArgument>("ClassTemplateArgument", typeid(ClassTemplateArgument).name());
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
reflectionSystem.AddClass<BinarySerializer>("BinarySerializer", typeid(BinarySerializer).name());
reflectionSystem.AddClass<ListSerializer>("ListSerializer", typeid(ListSerializer).name());
reflectionSystem.AddClass<StringSerializer>("StringSerializer", typeid(StringSerializer).name());
reflectionSystem.AddClass<MeshData>("MeshData", typeid(MeshData).name());
reflectionSystem.AddClass<VertexBufferData>("VertexBufferData", typeid(VertexBufferData).name());
reflectionSystem.AddClass<IndexBufferData>("IndexBufferData", typeid(IndexBufferData).name());
reflectionSystem.AddClass<MeshInstanceData>("MeshInstanceData", typeid(MeshInstanceData).name());
reflectionSystem.AddClass<PerDrawData>("PerDrawData", typeid(PerDrawData).name());
reflectionSystem.AddClass<PointLightData>("PointLightData", typeid(PointLightData).name());
reflectionSystem.AddClass<System>("System", typeid(System).name());
reflectionSystem.AddClass<LandscapeSystem>("LandscapeSystem", typeid(LandscapeSystem).name());
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
reflectionSystem.AddClass<PrimitiveSerializer>("PrimitiveSerializer", typeid(PrimitiveSerializer).name());

        }
        
        // Add all fields & add base classes.
        {
            { 
	Class* currentClass = reflectionSystem.GetMutableClass<SaveLoadToolbar>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SaveLoadToolbar* instance = static_cast<SaveLoadToolbar*>(inInstance);
instance->Save();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Save", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
currentMethod.AddMetadata(R"delim(EditorMenuItem("File/Save"))delim");
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SaveLoadToolbar* instance = static_cast<SaveLoadToolbar*>(inInstance);
instance->Load();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Load", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
currentMethod.AddMetadata(R"delim(EditorMenuItem("File/Load"))delim");
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Skeleton>();
	{
		Field& currentField = currentClass->AddField(Field("myJoints", 0, reflectionSystem.GetOrCreateClass<List<Skeleton::Bone>>("List<Skeleton::Bone>"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Skeleton::Bone>();
	{
		Field& currentField = currentClass->AddField(Field("myBindPosition", 0, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindRotation", 12, reflectionSystem.GetOrCreateClass<glm::qua<float>>("glm::qua<float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindScale", 28, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexToParent", 40, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Editor>();
	{
		Field& currentField = currentClass->AddField(Field("myWindows", 0, reflectionSystem.GetOrCreateClass<List<EditorWindow *>>("List<EditorWindow *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySystems", 24, reflectionSystem.GetOrCreateClass<List<EditorSystem *>>("List<EditorSystem *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNextID", 48, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGameTickFunction", 56, reflectionSystem.GetOrCreateClass<Delegate<void ()>>("Delegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAssetRegistry", 64, reflectionSystem.GetOrCreateClass<AssetRegistry>("AssetRegistry"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
instance->StaticTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("StaticTick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
AssetRegistry * result = instance->GetAssetRegistry();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetRegistry", reflectionSystem.GetOrCreateClass<AssetRegistry *>("AssetRegistry *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
EditorWindow * arg0 = (EditorWindow*)inArguments[0];
instance->RemoveWindow(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inEditorWindow", reflectionSystem.GetOrCreateClass<EditorWindow *>("EditorWindow *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveWindow", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
const Delegate<void ()> & arg0 = *(const Delegate<void ()>*)inArguments[0];
instance->SetGameTickFunction(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTickFunction", reflectionSystem.GetOrCreateClass<const Delegate<void ()> &>("const Delegate<void ()> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetGameTickFunction", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myID", 8, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<HierarchyWindow>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorWindow>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
HierarchyWindow* instance = static_cast<HierarchyWindow*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ImGuiDemoSystem>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorSystem>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiDemoSystem* instance = static_cast<ImGuiDemoSystem*>(inInstance);
instance->OpenImGuiDemoWindow();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OpenImGuiDemoWindow", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
currentMethod.AddMetadata(R"delim(EditorMenuItem("ImGui Demo"))delim");
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiDemoSystem* instance = static_cast<ImGuiDemoSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorCameraMovementComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myResetMouseDelta", 16, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMouseDelta", 20, reflectionSystem.GetOrCreateClass<glm::vec<2, float>>("glm::vec<2, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myYaw", 28, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPitch", 32, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMovementSpeed", 36, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShiftMultiplier", 40, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myScrollMultiplier", 44, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorCameraMovementComponent* instance = static_cast<EditorCameraMovementComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorCameraMovementComponent* instance = static_cast<EditorCameraMovementComponent*>(inInstance);
instance->ResetMouseDelta();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ResetMouseDelta", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorToolbar>();
	{
		Field& currentField = currentClass->AddField(Field("myToolbarMethods", 16, reflectionSystem.GetOrCreateClass<List<const Method *>>("List<const Method *>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorSystem>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorToolbar* instance = static_cast<EditorToolbar*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorToolbar* instance = static_cast<EditorToolbar*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Delegate<void ()> & arg1 = *(const Delegate<void ()>*)inArguments[1];
instance->AddToolbarButton(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inCallback", reflectionSystem.GetOrCreateClass<const Delegate<void ()> &>("const Delegate<void ()> &")));
Method& currentMethod = currentClass->AddMethod(Method("AddToolbarButton", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorThemes>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorThemes* instance = static_cast<EditorThemes*>(inInstance);
instance->DefaultTheme();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DefaultTheme", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
currentMethod.AddMetadata(R"delim(EditorMenuItem("Themes/Default"))delim");
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CapsuleColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myRadius", 56, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myHeight", 60, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CapsuleColliderComponent* instance = static_cast<CapsuleColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CapsuleColliderComponent* instance = static_cast<CapsuleColliderComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetRadius(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRadius", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetRadius", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CapsuleColliderComponent* instance = static_cast<CapsuleColliderComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetHeight(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inHeight", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetHeight", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SelectionSystem>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SelectionSystem* instance = static_cast<SelectionSystem*>(inInstance);
const List<GameObject *> & result = instance->GetSelectedObjects();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSelectedObjects", reflectionSystem.GetOrCreateClass<const List<GameObject *> &>("const List<GameObject *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SelectionSystem* instance = static_cast<SelectionSystem*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
static thread_local bool result = instance->IsObjectSelected(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("IsObjectSelected", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SelectionSystem* instance = static_cast<SelectionSystem*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
instance->SelectObject(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("SelectObject", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SelectionSystem* instance = static_cast<SelectionSystem*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
instance->DeselectObject(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("DeselectObject", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SelectionSystem* instance = static_cast<SelectionSystem*>(inInstance);
instance->ClearSelection();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ClearSelection", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorConfirmPrompt>();
	{
		Field& currentField = currentClass->AddField(Field("myTitle", 0, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBody", 32, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCallback", 64, reflectionSystem.GetOrCreateClass<Delegate<void ()>>("Delegate<void ()>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorConfirmPrompt* instance = static_cast<EditorConfirmPrompt*>(inInstance);
instance->Show();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Show", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorConfirmPrompt* instance = static_cast<EditorConfirmPrompt*>(inInstance);
instance->Update();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Update", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SinWaveMovementComponent>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SinWaveMovementComponent* instance = static_cast<SinWaveMovementComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ImGuiTextureUtils>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiTextureUtils* instance = static_cast<ImGuiTextureUtils*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
static thread_local vk::DescriptorSet result = instance->CreateDescriptorSetForTexture(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("CreateDescriptorSetForTexture", reflectionSystem.GetOrCreateClass<vk::DescriptorSet>("vk::DescriptorSet"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ContentBrowserItem>();
	{
		Field& currentField = currentClass->AddField(Field("myPath", 0, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTexture", 32, reflectionSystem.GetOrCreateClass<vk::DescriptorSet>("vk::DescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsSelected", 40, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsDirectory", 41, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ContentBrowserWindow>();
	{
		Field& currentField = currentClass->AddField(Field("myTooltipProgress", 48, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTooltipDuration", 52, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCanSeeTooltip", 56, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRoot", 64, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCurrentPath", 96, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySelectedTreePath", 128, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySearchString", 160, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySearchPaths", 192, reflectionSystem.GetOrCreateClass<List<std::filesystem::path>>("List<std::filesystem::path>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirectories", 216, reflectionSystem.GetOrCreateClass<std::map<std::filesystem::path, List<std::filesystem::path>>>("std::map<std::filesystem::path, List<std::filesystem::path>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myItems", 232, reflectionSystem.GetOrCreateClass<List<ContentBrowserItem>>("List<ContentBrowserItem>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHistory", 256, reflectionSystem.GetOrCreateClass<std::deque<std::filesystem::path>>("std::deque<std::filesystem::path>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myForwardHistory", 296, reflectionSystem.GetOrCreateClass<std::deque<std::filesystem::path>>("std::deque<std::filesystem::path>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFolderIcons", 336, reflectionSystem.GetOrCreateClass<std::array<vk::DescriptorSet, 2>>("std::array<vk::DescriptorSet, 2>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myExcludedExtensions", 352, reflectionSystem.GetOrCreateClass<List<std::basic_string<char>>>("List<std::basic_string<char>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDragDropPath", 376, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySelectedItem", 408, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenamingItem", 412, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNewFileName", 416, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenamingInProgress", 448, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySearchBarOpenedThisFrame", 449, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTextureSize", 452, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTextureSizeMin", 456, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTextureSizeMax", 460, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFocusedResult", 464, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myConfirmPrompt", 472, reflectionSystem.GetOrCreateClass<EditorConfirmPrompt>("EditorConfirmPrompt"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorWindow>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ContentBrowserWindow* instance = static_cast<ContentBrowserWindow*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ContentBrowserWindow* instance = static_cast<ContentBrowserWindow*>(inInstance);
instance->ReloadDirectory();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ReloadDirectory", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<InspectorWindow>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorWindow>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
InspectorWindow* instance = static_cast<InspectorWindow*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorWindow>();
	{
		Field& currentField = currentClass->AddField(Field("myWindowName", 8, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsClosable", 40, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myID", 44, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorWindow* instance = static_cast<EditorWindow*>(inInstance);
instance->DoTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DoTick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ImGuiPropertyDrawer>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiPropertyDrawer* instance = static_cast<ImGuiPropertyDrawer*>(inInstance);
instance->RegisterDrawers();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RegisterDrawers", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiPropertyDrawer* instance = static_cast<ImGuiPropertyDrawer*>(inInstance);
const Field & arg0 = *(const Field*)inArguments[0];
void * arg1 = (void*)inArguments[1];
static thread_local bool result = instance->DrawProperty(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inField", reflectionSystem.GetOrCreateClass<const Field &>("const Field &")));
arguments.Add(MethodArgument("inInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
Method& currentMethod = currentClass->AddMethod(Method("DrawProperty", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Viewport>();
	{
		Field& currentField = currentClass->AddField(Field("myEditorCamera", 72, reflectionSystem.GetOrCreateClass<EditorCameraMovementComponent>("EditorCameraMovementComponent"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorSets", 80, reflectionSystem.GetOrCreateClass<List<vk::DescriptorSet>>("List<vk::DescriptorSet>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySampler", 104, reflectionSystem.GetOrCreateClass<vk::Sampler>("vk::Sampler"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myP0", 112, reflectionSystem.GetOrCreateClass<ImVec2>("ImVec2"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myP1", 120, reflectionSystem.GetOrCreateClass<ImVec2>("ImVec2"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EditorWindow>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EventObserver>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Viewport* instance = static_cast<Viewport*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher>();
	{
		Field& currentField = currentClass->AddField(Field("myThread", 0, reflectionSystem.GetOrCreateClass<std::thread>("std::thread"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLock", 16, reflectionSystem.GetOrCreateClass<std::mutex>("std::mutex"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShouldRun", 96, reflectionSystem.GetOrCreateClass<std::atomic<bool>>("std::atomic<bool>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFilesToWatch", 104, reflectionSystem.GetOrCreateClass<std::unordered_map<std::filesystem::path, Filewatcher::FileData>>("std::unordered_map<std::filesystem::path, Filewatcher::FileData>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myModifiedPaths", 168, reflectionSystem.GetOrCreateClass<std::set<Filewatcher::FileData *>>("std::set<Filewatcher::FileData *>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Filewatcher* instance = static_cast<Filewatcher*>(inInstance);
const bool arg0 = *(const bool*)inArguments[0];
instance->SetShouldRun(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inState", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetShouldRun", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Filewatcher* instance = static_cast<Filewatcher*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const std::function<void ()> arg1 = *(const std::function<void ()>*)inArguments[1];
static thread_local Filewatcher::CallbackHandle result = instance->InsertWatch_TS(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inCallback", reflectionSystem.GetOrCreateClass<const std::function<void ()>>("const std::function<void ()>")));
Method& currentMethod = currentClass->AddMethod(Method("InsertWatch_TS", reflectionSystem.GetOrCreateClass<Filewatcher::CallbackHandle>("Filewatcher::CallbackHandle"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Filewatcher* instance = static_cast<Filewatcher*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const Filewatcher::CallbackHandle & arg1 = *(const Filewatcher::CallbackHandle*)inArguments[1];
instance->RemoveWatch_TS(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inHandle", reflectionSystem.GetOrCreateClass<const Filewatcher::CallbackHandle &>("const Filewatcher::CallbackHandle &")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveWatch_TS", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Filewatcher* instance = static_cast<Filewatcher*>(inInstance);
instance->FlushChanges();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("FlushChanges", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher::CallbackHandle>();
	{
		Field& currentField = currentClass->AddField(Field("myID", 0, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Filewatcher::CallbackHandle* instance = static_cast<Filewatcher::CallbackHandle*>(inInstance);
const Filewatcher::CallbackHandle & arg0 = *(const Filewatcher::CallbackHandle*)inArguments[0];
static thread_local bool result = instance->operator==(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<const Filewatcher::CallbackHandle &>("const Filewatcher::CallbackHandle &")));
Method& currentMethod = currentClass->AddMethod(Method("operator==", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher::FileData>();
	{
		Field& currentField = currentClass->AddField(Field("myCallbackIDIndexList", 0, reflectionSystem.GetOrCreateClass<List<Filewatcher::CallbackHandle>>("List<Filewatcher::CallbackHandle>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCallbacks", 24, reflectionSystem.GetOrCreateClass<List<std::function<void ()>>>("List<std::function<void ()>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLastModifiedTime", 48, reflectionSystem.GetOrCreateClass<std::chrono::time_point<std::filesystem::_File_time_clock>>("std::chrono::time_point<std::filesystem::_File_time_clock>"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Asset>();
	{
		Field& currentField = currentClass->AddField(Field("myPath", 8, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsValid", 40, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAssetRegistry", 48, reflectionSystem.GetOrCreateClass<AssetRegistry>("AssetRegistry"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
static thread_local bool result = instance->IsValid();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsValid", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
static thread_local const std::filesystem::path result = instance->GetPath();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SerializationMeshData>();
	{
		Field& currentField = currentClass->AddField(Field("myVertices", 0, reflectionSystem.GetOrCreateClass<List<Vertex>>("List<Vertex>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndices", 24, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySphereCenterBounds", 48, reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStagingVertexBuffer", 64, reflectionSystem.GetOrCreateClass<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStagingIndexBuffer", 72, reflectionSystem.GetOrCreateClass<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoPath", 80, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalPath", 112, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialPath", 144, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Model>();
	{
		Field& currentField = currentClass->AddField(Field("myMeshes", 56, reflectionSystem.GetOrCreateClass<List<Mesh *>>("List<Mesh *>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Model* instance = static_cast<Model*>(inInstance);
const std::filesystem::path arg0 = *(const std::filesystem::path*)inArguments[0];
static thread_local Coroutine<void, void, false> result = instance->Load(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path")));
Method& currentMethod = currentClass->AddMethod(Method("Load", reflectionSystem.GetOrCreateClass<Coroutine<void, void, false>>("Coroutine<void, void, false>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Model* instance = static_cast<Model*>(inInstance);
instance->Unload();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Unload", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Model* instance = static_cast<Model*>(inInstance);
const List<Mesh *> & result = instance->GetMeshes();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMeshes", reflectionSystem.GetOrCreateClass<const List<Mesh *> &>("const List<Mesh *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Model* instance = static_cast<Model*>(inInstance);
Model * arg0 = (Model*)inArguments[0];
static thread_local List<SerializationMeshData> result = instance->GetSerializationDataForModel(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inModel", reflectionSystem.GetOrCreateClass<Model *>("Model *")));
Method& currentMethod = currentClass->AddMethod(Method("GetSerializationDataForModel", reflectionSystem.GetOrCreateClass<List<SerializationMeshData>>("List<SerializationMeshData>"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IAssetContainer>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SkyboxPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", 0, reflectionSystem.GetOrCreateClass<Shader>("Shader"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", 8, reflectionSystem.GetOrCreateClass<Shader>("Shader"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDescriptorSet", 16, reflectionSystem.GetOrCreateClass<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", 112, reflectionSystem.GetOrCreateClass<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", 120, reflectionSystem.GetOrCreateClass<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", 128, reflectionSystem.GetOrCreateClass<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkyboxModel", 136, reflectionSystem.GetOrCreateClass<Model>("Model"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkybox", 144, reflectionSystem.GetOrCreateClass<Texture>("Texture"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SkyboxPipeline* instance = static_cast<SkyboxPipeline*>(inInstance);
vk::CommandBuffer arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->AddGraphicsCommands(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", reflectionSystem.GetOrCreateClass<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("AddGraphicsCommands", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ConvexColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myModel", 56, reflectionSystem.GetOrCreateClass<Model>("Model"), true, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ConvexColliderComponent* instance = static_cast<ConvexColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ConvexColliderComponent* instance = static_cast<ConvexColliderComponent*>(inInstance);
instance->OnScaleChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnScaleChanged", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ConvexColliderComponent* instance = static_cast<ConvexColliderComponent*>(inInstance);
Model * arg0 = (Model*)inArguments[0];
instance->SetModel(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inModel", reflectionSystem.GetOrCreateClass<Model *>("Model *")));
Method& currentMethod = currentClass->AddMethod(Method("SetModel", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AssetRegistry>();
	{
		Field& currentField = currentClass->AddField(Field("myMutex", 0, reflectionSystem.GetOrCreateClass<std::recursive_mutex>("std::recursive_mutex"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myContainers", 80, reflectionSystem.GetOrCreateClass<List<IAssetContainer *>>("List<IAssetContainer *>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
instance->ScanAssetsFolder();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ScanAssetsFolder", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local std::filesystem::path result = instance->GetPathFromAssetName(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inAssetName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetPathFromAssetName", reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<JsonAsset>();
	{
		Field& currentField = currentClass->AddField(Field("myJson", 56, reflectionSystem.GetOrCreateClass<nlohmann::basic_json<>>("nlohmann::basic_json<>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
JsonAsset* instance = static_cast<JsonAsset*>(inInstance);
const std::filesystem::path arg0 = *(const std::filesystem::path*)inArguments[0];
static thread_local Coroutine<void, void, false> result = instance->Load(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path")));
Method& currentMethod = currentClass->AddMethod(Method("Load", reflectionSystem.GetOrCreateClass<Coroutine<void, void, false>>("Coroutine<void, void, false>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
JsonAsset* instance = static_cast<JsonAsset*>(inInstance);
instance->Unload();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Unload", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
JsonAsset* instance = static_cast<JsonAsset*>(inInstance);
const nlohmann::basic_json<> & result = instance->GetJson();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetJson", reflectionSystem.GetOrCreateClass<const nlohmann::basic_json<> &>("const nlohmann::basic_json<> &"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Material>();
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoTexture", 56, reflectionSystem.GetOrCreateClass<Texture>("Texture"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalTexture", 64, reflectionSystem.GetOrCreateClass<Texture>("Texture"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialTexture", 72, reflectionSystem.GetOrCreateClass<Texture>("Texture"), true, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
const std::filesystem::path arg0 = *(const std::filesystem::path*)inArguments[0];
static thread_local Coroutine<void, void, false> result = instance->Load(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path")));
Method& currentMethod = currentClass->AddMethod(Method("Load", reflectionSystem.GetOrCreateClass<Coroutine<void, void, false>>("Coroutine<void, void, false>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
const std::filesystem::path arg0 = *(const std::filesystem::path*)inArguments[0];
const std::filesystem::path arg1 = *(const std::filesystem::path*)inArguments[1];
const std::filesystem::path arg2 = *(const std::filesystem::path*)inArguments[2];
const std::filesystem::path arg3 = *(const std::filesystem::path*)inArguments[3];
static thread_local Coroutine<void, void, false> result = instance->Load(arg0, arg1, arg2, arg3);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path")));
arguments.Add(MethodArgument("inAlbedo", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path")));
arguments.Add(MethodArgument("inNormal", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path")));
arguments.Add(MethodArgument("inMaterial", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path")));
Method& currentMethod = currentClass->AddMethod(Method("Load", reflectionSystem.GetOrCreateClass<Coroutine<void, void, false>>("Coroutine<void, void, false>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
instance->Unload();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Unload", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
Texture * result = instance->GetAlbedo();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAlbedo", reflectionSystem.GetOrCreateClass<Texture *>("Texture *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
Texture * result = instance->GetNormal();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNormal", reflectionSystem.GetOrCreateClass<Texture *>("Texture *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
Texture * result = instance->GetMaterial();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMaterial", reflectionSystem.GetOrCreateClass<Texture *>("Texture *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Shader>();
	{
		Field& currentField = currentClass->AddField(Field("OnShaderRecompiled", 56, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShaderModule", 160, reflectionSystem.GetOrCreateClass<vk::ShaderModule>("vk::ShaderModule"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCallbackHandle", 168, reflectionSystem.GetOrCreateClass<Filewatcher::CallbackHandle>("Filewatcher::CallbackHandle"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
const std::filesystem::path arg0 = *(const std::filesystem::path*)inArguments[0];
static thread_local Coroutine<void, void, false> result = instance->Load(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path")));
Method& currentMethod = currentClass->AddMethod(Method("Load", reflectionSystem.GetOrCreateClass<Coroutine<void, void, false>>("Coroutine<void, void, false>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
instance->Unload();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Unload", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
instance->Compile();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Compile", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
static thread_local vk::ShaderModule result = instance->GetAPIResource();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAPIResource", reflectionSystem.GetOrCreateClass<vk::ShaderModule>("vk::ShaderModule"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MethodArgument>();
	{
		Field& currentField = currentClass->AddField(Field("myArgumentName", 0, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myArgumentType", 32, reflectionSystem.GetOrCreateClass<const Class>("const Class"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MethodArgument* instance = static_cast<MethodArgument*>(inInstance);
const std::basic_string<char> & result = instance->GetArgumentName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetArgumentName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MethodArgument* instance = static_cast<MethodArgument*>(inInstance);
const Class * result = instance->GetArgumentType();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetArgumentType", reflectionSystem.GetOrCreateClass<const Class *>("const Class *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Method>();
	{
		Field& currentField = currentClass->AddField(Field("myMethodName", 0, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myReturnType", 32, reflectionSystem.GetOrCreateClass<const Class>("const Class"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myArguments", 40, reflectionSystem.GetOrCreateClass<List<MethodArgument>>("List<MethodArgument>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMetadata", 64, reflectionSystem.GetOrCreateClass<List<std::basic_string<char>>>("List<std::basic_string<char>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myInvoker", 88, reflectionSystem.GetOrCreateClass<Delegate<void *(void *, const List<void *> &)>>("Delegate<void *(void *, const List<void *> &)>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Method* instance = static_cast<Method*>(inInstance);
const std::basic_string<char> & result = instance->GetMethodName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMethodName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Method* instance = static_cast<Method*>(inInstance);
const List<std::basic_string<char>> & result = instance->GetMetadata();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMetadata", reflectionSystem.GetOrCreateClass<const List<std::basic_string<char>> &>("const List<std::basic_string<char>> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Method* instance = static_cast<Method*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local bool result = instance->HasMetadata(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMetadata", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("HasMetadata", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Method* instance = static_cast<Method*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local List<std::basic_string<char>> result = instance->GetMetadataArgs(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMetadata", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetMetadataArgs", reflectionSystem.GetOrCreateClass<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ImageData>();
	{
		Field& currentField = currentClass->AddField(Field("mySourceFile", 0, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWidth", 32, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHeight", 36, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myChannels", 40, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumMipLevels", 44, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPixelData", 48, reflectionSystem.GetOrCreateClass<List<unsigned char>>("List<unsigned char>"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Texture>();
	{
		Field& currentField = currentClass->AddField(Field("myImage", 56, reflectionSystem.GetOrCreateClass<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindlessIndex", 64, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
const std::filesystem::path arg0 = *(const std::filesystem::path*)inArguments[0];
static thread_local Coroutine<void, void, false> result = instance->Load(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path")));
Method& currentMethod = currentClass->AddMethod(Method("Load", reflectionSystem.GetOrCreateClass<Coroutine<void, void, false>>("Coroutine<void, void, false>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
instance->Unload();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Unload", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
VulkanImage * result = instance->GetImage();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImage", reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
static thread_local vk::ImageView result = instance->GetImageView();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImageView", reflectionSystem.GetOrCreateClass<vk::ImageView>("vk::ImageView"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
static thread_local unsigned int result = instance->GetBindlessIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBindlessIndex", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadPool>();
	{
		Field& currentField = currentClass->AddField(Field("myThreads", 0, reflectionSystem.GetOrCreateClass<List<std::thread>>("List<std::thread>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLookingForTaskMutex", 24, reflectionSystem.GetOrCreateClass<std::mutex>("std::mutex"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWaitingForTaskCondition", 104, reflectionSystem.GetOrCreateClass<std::condition_variable>("std::condition_variable"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShouldExit", 176, reflectionSystem.GetOrCreateClass<std::atomic<bool>>("std::atomic<bool>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTasks", 184, reflectionSystem.GetOrCreateClass<std::queue<std::function<void ()>>>("std::queue<std::function<void ()>>"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TextureCube>();
	{
		Field& currentField = currentClass->AddField(Field("myImage", 56, reflectionSystem.GetOrCreateClass<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindlessIndex", 64, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureCube* instance = static_cast<TextureCube*>(inInstance);
const std::filesystem::path arg0 = *(const std::filesystem::path*)inArguments[0];
static thread_local Coroutine<void, void, false> result = instance->Load(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path>("const std::filesystem::path")));
Method& currentMethod = currentClass->AddMethod(Method("Load", reflectionSystem.GetOrCreateClass<Coroutine<void, void, false>>("Coroutine<void, void, false>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureCube* instance = static_cast<TextureCube*>(inInstance);
instance->Unload();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Unload", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureCube* instance = static_cast<TextureCube*>(inInstance);
static thread_local vk::ImageView result = instance->GetImageView();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImageView", reflectionSystem.GetOrCreateClass<vk::ImageView>("vk::ImageView"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureCube* instance = static_cast<TextureCube*>(inInstance);
static thread_local unsigned int result = instance->GetBindlessIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBindlessIndex", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Input>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const int arg0 = *(const int*)inArguments[0];
static thread_local bool result = instance->IsKeyDown(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", reflectionSystem.GetOrCreateClass<const int>("const int")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyDown", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::KeyCode arg0 = *(const Input::KeyCode*)inArguments[0];
static thread_local bool result = instance->IsKeyDown(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", reflectionSystem.GetOrCreateClass<const Input::KeyCode>("const Input::KeyCode")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyDown", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::MouseButton arg0 = *(const Input::MouseButton*)inArguments[0];
static thread_local bool result = instance->IsKeyDown(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aMouseButton", reflectionSystem.GetOrCreateClass<const Input::MouseButton>("const Input::MouseButton")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyDown", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const int arg0 = *(const int*)inArguments[0];
static thread_local bool result = instance->IsKeyUp(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", reflectionSystem.GetOrCreateClass<const int>("const int")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyUp", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::KeyCode arg0 = *(const Input::KeyCode*)inArguments[0];
static thread_local bool result = instance->IsKeyUp(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", reflectionSystem.GetOrCreateClass<const Input::KeyCode>("const Input::KeyCode")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyUp", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::MouseButton arg0 = *(const Input::MouseButton*)inArguments[0];
static thread_local bool result = instance->IsKeyUp(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aMouseButton", reflectionSystem.GetOrCreateClass<const Input::MouseButton>("const Input::MouseButton")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyUp", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const int arg0 = *(const int*)inArguments[0];
static thread_local bool result = instance->IsKeyPressed(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", reflectionSystem.GetOrCreateClass<const int>("const int")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyPressed", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::KeyCode arg0 = *(const Input::KeyCode*)inArguments[0];
static thread_local bool result = instance->IsKeyPressed(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", reflectionSystem.GetOrCreateClass<const Input::KeyCode>("const Input::KeyCode")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyPressed", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::MouseButton arg0 = *(const Input::MouseButton*)inArguments[0];
static thread_local bool result = instance->IsKeyPressed(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aMouseButton", reflectionSystem.GetOrCreateClass<const Input::MouseButton>("const Input::MouseButton")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyPressed", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const glm::vec<2, float> & result = instance->GetMousePosition();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMousePosition", reflectionSystem.GetOrCreateClass<const glm::vec<2, float> &>("const glm::vec<2, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const glm::vec<2, float> & result = instance->GetMousePositionClientRelative();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMousePositionClientRelative", reflectionSystem.GetOrCreateClass<const glm::vec<2, float> &>("const glm::vec<2, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
static thread_local const glm::vec<2, float> result = instance->GetMouseDelta();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMouseDelta", reflectionSystem.GetOrCreateClass<const glm::vec<2, float>>("const glm::vec<2, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
static thread_local const glm::vec<2, float> result = instance->GetMouseDeltaClientRelative();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMouseDeltaClientRelative", reflectionSystem.GetOrCreateClass<const glm::vec<2, float>>("const glm::vec<2, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
unsigned int arg0 = *(unsigned int*)inArguments[0];
unsigned long long arg1 = *(unsigned long long*)inArguments[1];
long long arg2 = *(long long*)inArguments[2];
static thread_local bool result = instance->UpdateEvents(arg0, arg1, arg2);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("message", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
arguments.Add(MethodArgument("wParam", reflectionSystem.GetOrCreateClass<unsigned long long>("unsigned long long")));
arguments.Add(MethodArgument("lParam", reflectionSystem.GetOrCreateClass<long long>("long long")));
Method& currentMethod = currentClass->AddMethod(Method("UpdateEvents", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
instance->EndFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("EndFrame", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TransformComponent>();
	{
		Field& currentField = currentClass->AddField(Field("OnPositionChanged", 16, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("OnRotationChanged", 120, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("OnScaleChanged", 224, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPositionDirty", 328, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRotationDirty", 329, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myScaleDirty", 330, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPosition", 332, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
		currentField.AddMetadata(R"delim(OnInspectorChangedEvent(MarkDirtyFromInspector))delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myRotation", 344, reflectionSystem.GetOrCreateClass<glm::qua<float>>("glm::qua<float>"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myScale", 360, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkipPhysicsUpdate", 372, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myParent", 376, reflectionSystem.GetOrCreateClass<TransformComponent>("TransformComponent"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myChildren", 384, reflectionSystem.GetOrCreateClass<List<TransformComponent *>>("List<TransformComponent *>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local physx::PxTransformT<float> result = instance->AsPxTransform();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("AsPxTransform", reflectionSystem.GetOrCreateClass<physx::PxTransformT<float>>("physx::PxTransformT<float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
TransformComponent * arg0 = (TransformComponent*)inArguments[0];
instance->SetParent(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inParent", reflectionSystem.GetOrCreateClass<TransformComponent *>("TransformComponent *")));
Method& currentMethod = currentClass->AddMethod(Method("SetParent", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
instance->RemoveParent();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RemoveParent", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
TransformComponent * arg0 = (TransformComponent*)inArguments[0];
instance->AddChild(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inChild", reflectionSystem.GetOrCreateClass<TransformComponent *>("TransformComponent *")));
Method& currentMethod = currentClass->AddMethod(Method("AddChild", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
TransformComponent * arg0 = (TransformComponent*)inArguments[0];
instance->RemoveChild(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inChild", reflectionSystem.GetOrCreateClass<TransformComponent *>("TransformComponent *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveChild", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const List<TransformComponent *> & result = instance->GetChildren();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetChildren", reflectionSystem.GetOrCreateClass<const List<TransformComponent *> &>("const List<TransformComponent *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetPositionLocal(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPosition", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetPositionLocal", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::qua<float> & arg0 = *(const glm::qua<float>*)inArguments[0];
instance->SetRotationLocal(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inQuaternion", reflectionSystem.GetOrCreateClass<const glm::qua<float> &>("const glm::qua<float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotationLocal", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetScaleLocal(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inScale", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetScaleLocal", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetScaleLocal(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inScale", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetScaleLocal", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetPosition(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPosition", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetPosition", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
const float arg1 = *(const float*)inArguments[1];
const float arg2 = *(const float*)inArguments[2];
instance->SetPosition(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inY", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inZ", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetPosition", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const physx::PxVec3T<float> & arg0 = *(const physx::PxVec3T<float>*)inArguments[0];
instance->SetPosition(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPosition", reflectionSystem.GetOrCreateClass<const physx::PxVec3T<float> &>("const physx::PxVec3T<float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetPosition", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetPositionX(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetPositionX", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetPositionY(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inY", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetPositionY", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetPositionZ(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inZ", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetPositionZ", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetScale(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inScale", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetScale", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
const float arg1 = *(const float*)inArguments[1];
const float arg2 = *(const float*)inArguments[2];
instance->SetScale(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inY", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inZ", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetScale", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetScale(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inScalar", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetScale", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::qua<float> & arg0 = *(const glm::qua<float>*)inArguments[0];
instance->SetRotation(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inQuat", reflectionSystem.GetOrCreateClass<const glm::qua<float> &>("const glm::qua<float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotation", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const physx::PxQuatT<float> & arg0 = *(const physx::PxQuatT<float>*)inArguments[0];
instance->SetRotation(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inQuat", reflectionSystem.GetOrCreateClass<const physx::PxQuatT<float> &>("const physx::PxQuatT<float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotation", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetRotationRad(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRotation", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotationRad", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetRotationDeg(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRotation", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotationDeg", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
const float arg1 = *(const float*)inArguments[1];
const float arg2 = *(const float*)inArguments[2];
instance->SetRotationDeg(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inY", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inZ", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotationDeg", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & result = instance->GetPositionLocal();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPositionLocal", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::qua<float> & result = instance->GetRotationLocal();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRotationLocal", reflectionSystem.GetOrCreateClass<const glm::qua<float> &>("const glm::qua<float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & result = instance->GetScaleLocal();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetScaleLocal", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local const glm::vec<3, float> result = instance->LocalForward();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LocalForward", reflectionSystem.GetOrCreateClass<const glm::vec<3, float>>("const glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local const glm::vec<3, float> result = instance->LocalRight();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LocalRight", reflectionSystem.GetOrCreateClass<const glm::vec<3, float>>("const glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local const glm::vec<3, float> result = instance->LocalUp();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LocalUp", reflectionSystem.GetOrCreateClass<const glm::vec<3, float>>("const glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::mat<4, 4, float> result = instance->GetMatrix();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMatrix", reflectionSystem.GetOrCreateClass<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::mat<4, 4, float> result = instance->GetMatrixLocal();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMatrixLocal", reflectionSystem.GetOrCreateClass<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetPosition();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPosition", reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::qua<float> result = instance->GetRotation();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRotation", reflectionSystem.GetOrCreateClass<glm::qua<float>>("glm::qua<float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetRotationRad();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRotationRad", reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetRotationDeg();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRotationDeg", reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetScale();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetScale", reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
currentMethod.AddMetadata(R"delim(AllowPrivateAccess)delim");
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
TransformComponent * result = instance->GetParent();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetParent", reflectionSystem.GetOrCreateClass<TransformComponent *>("TransformComponent *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local bool result = instance->HasParent();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("HasParent", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetForward();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetForward", reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetUp();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetUp", reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetRight();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRight", reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->Move(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inDisplacement", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("Move", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
const float arg1 = *(const float*)inArguments[1];
const float arg2 = *(const float*)inArguments[2];
instance->Move(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inY", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inZ", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("Move", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->Rotate(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRotation", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("Rotate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
const float arg1 = *(const float*)inArguments[1];
const float arg2 = *(const float*)inArguments[2];
instance->Rotate(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inY", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inZ", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("Rotate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
instance->MarkDirtyFromInspector();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("MarkDirtyFromInspector", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
currentMethod.AddMetadata(R"delim(AllowPrivateAccess)delim");
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Component>();
	{
		Field& currentField = currentClass->AddField(Field("myGameObject", 8, reflectionSystem.GetOrCreateClass<GameObject>("GameObject"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->OnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnDestroy", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
instance->OnTriggerEnter(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerEnter", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
instance->OnTrigger(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTrigger", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
instance->OnTriggerExit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerExit", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
instance->OnCollisionEnter(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionEnter", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
instance->OnCollision(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollision", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
instance->OnCollisionExit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionExit", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->MarkRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("MarkRenderStateDirty", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->OnRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnRenderStateDirty", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
GameObject * result = instance->GetGameObject();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGameObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
TransformComponent * result = instance->GetTransform();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTransform", reflectionSystem.GetOrCreateClass<TransformComponent *>("TransformComponent *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
World * result = instance->GetWorld();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWorld", reflectionSystem.GetOrCreateClass<World *>("World *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Time>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
static thread_local float result = instance->GetDeltaTime();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDeltaTime", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
static thread_local float result = instance->GetUnscaledDeltaTime();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetUnscaledDeltaTime", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetTimeScale(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTimeScale", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetTimeScale", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
static thread_local double result = instance->GetSeconds();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSeconds", reflectionSystem.GetOrCreateClass<double>("double"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
static thread_local float result = instance->GetTimeSinceStart();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTimeSinceStart", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TypeSerializer>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TypeSerializer* instance = static_cast<TypeSerializer*>(inInstance);
const Class * arg0 = (const Class*)inArguments[0];
TypeSerializer * result = instance->GetSerializer(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
Method& currentMethod = currentClass->AddMethod(Method("GetSerializer", reflectionSystem.GetOrCreateClass<TypeSerializer *>("TypeSerializer *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myModel", 56, reflectionSystem.GetOrCreateClass<Model>("Model"), true, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshColliderComponent* instance = static_cast<MeshColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshColliderComponent* instance = static_cast<MeshColliderComponent*>(inInstance);
instance->OnScaleChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnScaleChanged", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshColliderComponent* instance = static_cast<MeshColliderComponent*>(inInstance);
Model * arg0 = (Model*)inArguments[0];
instance->SetModel(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inModel", reflectionSystem.GetOrCreateClass<Model *>("Model *")));
Method& currentMethod = currentClass->AddMethod(Method("SetModel", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IComponentArray>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Console>();
	{
		Field& currentField = currentClass->AddField(Field("myConsoleHandle", 0, reflectionSystem.GetOrCreateClass<void>("void"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLogToFileEnabled", 8, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLogToFileStream", 16, reflectionSystem.GetOrCreateClass<std::basic_ofstream<char>>("std::basic_ofstream<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCoutBuffer", 280, reflectionSystem.GetOrCreateClass<std::basic_streambuf<char>>("std::basic_streambuf<char>"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Console* instance = static_cast<Console*>(inInstance);
Console & result = instance->Get();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Get", reflectionSystem.GetOrCreateClass<Console &>("Console &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Console* instance = static_cast<Console*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
instance->Log(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("Log", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Console* instance = static_cast<Console*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
instance->LogWarning(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("LogWarning", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Console* instance = static_cast<Console*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
instance->LogError(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("LogError", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ComponentSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myObjects", 16, reflectionSystem.GetOrCreateClass<List<GameObject *>>("List<GameObject *>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myObjectsToDestory", 40, reflectionSystem.GetOrCreateClass<List<GameObject *>>("List<GameObject *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComponentArrays", 64, reflectionSystem.GetOrCreateClass<List<IComponentArray *>>("List<IComponentArray *>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<WorldSystem>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
GameObject * arg1 = (GameObject*)inArguments[1];
instance->OnCollisionEnterForGameObject(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionEnterForGameObject", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
GameObject * arg1 = (GameObject*)inArguments[1];
instance->OnCollisionForGameObject(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionForGameObject", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
GameObject * arg1 = (GameObject*)inArguments[1];
instance->OnCollisionExitForGameObject(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionExitForGameObject", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
GameObject * arg1 = (GameObject*)inArguments[1];
instance->OnTriggerEnterForGameObject(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerEnterForGameObject", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
GameObject * arg1 = (GameObject*)inArguments[1];
instance->OnTriggerForGameObject(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerForGameObject", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
GameObject * arg1 = (GameObject*)inArguments[1];
instance->OnTriggerExitForGameObject(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
arguments.Add(MethodArgument("inOther", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerExitForGameObject", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
GameObject * result = instance->CreateGameObject(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObjectName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("CreateGameObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
GameObject * arg0 = (GameObject*)inArguments[0];
instance->DestroyGameObject(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inGameObject", reflectionSystem.GetOrCreateClass<GameObject *>("GameObject *")));
Method& currentMethod = currentClass->AddMethod(Method("DestroyGameObject", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
const List<GameObject *> & result = instance->GetAllGameObjects();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAllGameObjects", reflectionSystem.GetOrCreateClass<const List<GameObject *> &>("const List<GameObject *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComponentSystem* instance = static_cast<ComponentSystem*>(inInstance);
const List<IComponentArray *> & result = instance->GetAllComponentArrays();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAllComponentArrays", reflectionSystem.GetOrCreateClass<const List<IComponentArray *> &>("const List<IComponentArray *> &"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IVulkanDynamicBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("OnBufferRecreated", 8, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GameObject>();
	{
		Field& currentField = currentClass->AddField(Field("OnComponentAdded", 0, reflectionSystem.GetOrCreateClass<MulticastDelegate<void (Component *)>>("MulticastDelegate<void (Component *)>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("OnComponentRemoved", 104, reflectionSystem.GetOrCreateClass<MulticastDelegate<void (Component *)>>("MulticastDelegate<void (Component *)>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComponentSystem", 208, reflectionSystem.GetOrCreateClass<ComponentSystem>("ComponentSystem"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTransform", 216, reflectionSystem.GetOrCreateClass<TransformComponent>("TransformComponent"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderStateDirty", 224, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPhysicsStateDirty", 225, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTags", 228, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
static thread_local bool result = instance->IsRenderStateDirty();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsRenderStateDirty", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
instance->ResetRenderStateDirtyFlag();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ResetRenderStateDirtyFlag", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
instance->SetName(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetName", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
const std::basic_string<char> & result = instance->GetName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
static thread_local List<Component *> result = instance->GetComponents();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetComponents", reflectionSystem.GetOrCreateClass<List<Component *>>("List<Component *>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
TransformComponent * result = instance->GetTransform();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTransform", reflectionSystem.GetOrCreateClass<TransformComponent *>("TransformComponent *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
instance->Destroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Destroy", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
static thread_local unsigned int result = instance->GetTags();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTags", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
unsigned int arg0 = *(unsigned int*)inArguments[0];
instance->AddTags(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTags", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("AddTags", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
unsigned int arg0 = *(unsigned int*)inArguments[0];
instance->RemoveTags(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTags", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveTags", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
instance->ClearTags();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ClearTags", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
unsigned int arg0 = *(unsigned int*)inArguments[0];
static thread_local bool result = instance->HasAnyTag(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTags", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("HasAnyTag", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
unsigned int arg0 = *(unsigned int*)inArguments[0];
static thread_local bool result = instance->HasAllTags(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTags", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("HasAllTags", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
instance->MarkRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("MarkRenderStateDirty", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
instance->MarkPhysicsStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("MarkPhysicsStateDirty", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameObject* instance = static_cast<GameObject*>(inInstance);
World * result = instance->GetWorld();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWorld", reflectionSystem.GetOrCreateClass<World *>("World *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CoroutineManager>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CoroutineManager* instance = static_cast<CoroutineManager*>(inInstance);
static thread_local Coroutine<void, void> result = instance->Load();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Load", reflectionSystem.GetOrCreateClass<Coroutine<void, void>>("Coroutine<void, void>"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeRenderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myHeightfield", 16, reflectionSystem.GetOrCreateClass<Heightfield>("Heightfield"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexBuffer", 280, reflectionSystem.GetOrCreateClass<VertexBufferHandle>("VertexBufferHandle"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexBuffer", 288, reflectionSystem.GetOrCreateClass<IndexBufferHandle>("IndexBufferHandle"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMesh", 296, reflectionSystem.GetOrCreateClass<Mesh>("Mesh"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMeshInstance", 304, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterial", 312, reflectionSystem.GetOrCreateClass<Material>("Material"), true, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeRenderComponent* instance = static_cast<LandscapeRenderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeRenderComponent* instance = static_cast<LandscapeRenderComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeRenderComponent* instance = static_cast<LandscapeRenderComponent*>(inInstance);
instance->OnRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnRenderStateDirty", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeRenderComponent* instance = static_cast<LandscapeRenderComponent*>(inInstance);
const Heightfield & result = instance->GetHeightfield();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHeightfield", reflectionSystem.GetOrCreateClass<const Heightfield &>("const Heightfield &"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BoxColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myHalfSize", 56, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BoxColliderComponent* instance = static_cast<BoxColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BoxColliderComponent* instance = static_cast<BoxColliderComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetHalfSize(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inHalfSize", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetHalfSize", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CameraComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myResolution", 16, reflectionSystem.GetOrCreateClass<glm::vec<2, float>>("glm::vec<2, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myProjection", 24, reflectionSystem.GetOrCreateClass<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFov", 88, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myNearPlane", 92, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myFarPlane", 96, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsOrthographic", 100, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const glm::mat<4, 4, float> & result = instance->GetProjection();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetProjection", reflectionSystem.GetOrCreateClass<const glm::mat<4, 4, float> &>("const glm::mat<4, 4, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
const float arg1 = *(const float*)inArguments[1];
const float arg2 = *(const float*)inArguments[2];
instance->CreateOrthographic(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inResolution", reflectionSystem.GetOrCreateClass<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
arguments.Add(MethodArgument("inNearPlane", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inFarPlane", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("CreateOrthographic", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
const float arg1 = *(const float*)inArguments[1];
const float arg2 = *(const float*)inArguments[2];
const float arg3 = *(const float*)inArguments[3];
instance->CreatePerspective(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inResolution", reflectionSystem.GetOrCreateClass<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
arguments.Add(MethodArgument("inFov", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inNearPlane", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inFarPlane", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("CreatePerspective", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GPUSceneSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myFreeSparseIndices", 8, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNextFreeSparseIndex", 32, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseBuffer", 40, reflectionSystem.GetOrCreateClass<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumGPUObjectUpdatesThisFrame", 48, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDenseBuffer", 56, reflectionSystem.GetOrCreateClass<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDenseBufferCPURepresentation", 64, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
const MeshInstanceData & arg0 = *(const MeshInstanceData*)inArguments[0];
static thread_local unsigned int result = instance->AddMeshInstance(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMeshInstanceData", reflectionSystem.GetOrCreateClass<const MeshInstanceData &>("const MeshInstanceData &")));
Method& currentMethod = currentClass->AddMethod(Method("AddMeshInstance", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
const unsigned int arg0 = *(const unsigned int*)inArguments[0];
const MeshInstanceData & arg1 = *(const MeshInstanceData*)inArguments[1];
instance->UpdateMeshInstance(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMeshIndex", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inMeshInstanceData", reflectionSystem.GetOrCreateClass<const MeshInstanceData &>("const MeshInstanceData &")));
Method& currentMethod = currentClass->AddMethod(Method("UpdateMeshInstance", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
const unsigned int arg0 = *(const unsigned int*)inArguments[0];
instance->RemoveMeshInstance(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMeshInstance", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveMeshInstance", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
const ResizableBuffer * result = instance->GetSparseBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSparseBuffer", reflectionSystem.GetOrCreateClass<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
const ResizableBuffer * result = instance->GetDenseBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDenseBuffer", reflectionSystem.GetOrCreateClass<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
static thread_local unsigned int result = instance->GetNumObjects();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNumObjects", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RigidbodyComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myActor", 16, reflectionSystem.GetOrCreateClass<physx::PxRigidDynamic>("physx::PxRigidDynamic"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFramesSinceStartSleep", 24, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMass", 28, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
instance->OnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnDestroy", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetVelocity(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inVelocity", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetVelocity", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetVelocity();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetVelocity", reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetAngularVelocity(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inAngularVelocity", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetAngularVelocity", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetAngularVelocity();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAngularVelocity", reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const ForceMode arg1 = *(const ForceMode*)inArguments[1];
instance->AddForce(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inForce", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inForceMode", reflectionSystem.GetOrCreateClass<const ForceMode>("const ForceMode")));
Method& currentMethod = currentClass->AddMethod(Method("AddForce", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const ForceMode arg1 = *(const ForceMode*)inArguments[1];
instance->AddTorque(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTorque", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inForceMode", reflectionSystem.GetOrCreateClass<const ForceMode>("const ForceMode")));
Method& currentMethod = currentClass->AddMethod(Method("AddTorque", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetMass(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMass", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetMass", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
static thread_local float result = instance->GetMass();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMass", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetDrag(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inDrag", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetDrag", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
static thread_local float result = instance->GetDrag();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDrag", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetAngularDrag(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inDrag", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetAngularDrag", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
static thread_local float result = instance->GetAngularDrag();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAngularDrag", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const bool arg0 = *(const bool*)inArguments[0];
instance->UseGravity(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inValue", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("UseGravity", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const bool arg0 = *(const bool*)inArguments[0];
instance->SetKinematic(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inValue", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetKinematic", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
ColliderComponent * arg0 = (ColliderComponent*)inArguments[0];
instance->AttachCollider(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCollider", reflectionSystem.GetOrCreateClass<ColliderComponent *>("ColliderComponent *")));
Method& currentMethod = currentClass->AddMethod(Method("AttachCollider", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
ColliderComponent * arg0 = (ColliderComponent*)inArguments[0];
instance->DetachCollider(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCollider", reflectionSystem.GetOrCreateClass<ColliderComponent *>("ColliderComponent *")));
Method& currentMethod = currentClass->AddMethod(Method("DetachCollider", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const bool arg0 = *(const bool*)inArguments[0];
const bool arg1 = *(const bool*)inArguments[1];
const bool arg2 = *(const bool*)inArguments[2];
instance->SetRotationConstraint(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
arguments.Add(MethodArgument("inY", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
arguments.Add(MethodArgument("inZ", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotationConstraint", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CharacterControllerComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myController", 16, reflectionSystem.GetOrCreateClass<physx::PxController>("physx::PxController"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCollisionFlags", 24, reflectionSystem.GetOrCreateClass<physx::PxFlags<physx::PxControllerCollisionFlag::Enum, unsigned char>>("physx::PxFlags<physx::PxControllerCollisionFlag::Enum, unsigned char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFilter", 32, reflectionSystem.GetOrCreateClass<physx::PxControllerFilters>("physx::PxControllerFilters"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHeight", 64, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myRadius", 68, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySlopeLimitDegrees", 72, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMinDist", 76, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStepOffset", 80, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myUseGravity", 84, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myDownVelocity", 88, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGravity", 92, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myTerminalVelocity", 96, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myDisplacement", 100, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPositionOffset", 112, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
instance->OnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnDestroy", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
instance->OnPositionChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnPositionChanged", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->Move(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMovement", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("Move", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
static thread_local float result = instance->GetHeight();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHeight", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetHeight(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inHeight", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetHeight", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
static thread_local float result = instance->GetRadius();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRadius", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetRadius(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRadius", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetRadius", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
static thread_local bool result = instance->IsGrounded();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsGrounded", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetPositionOffset(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOffset", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetPositionOffset", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->Jump(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inJumpForce", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("Jump", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myShape", 16, reflectionSystem.GetOrCreateClass<physx::PxShape>("physx::PxShape"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myActor", 24, reflectionSystem.GetOrCreateClass<physx::PxRigidStatic>("physx::PxRigidStatic"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLocalShapePosition", 32, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLocalShapeRotation", 44, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
instance->OnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnDestroy", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
instance->OnPhysicsStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnPhysicsStateDirty", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
physx::PxShape * result = instance->GetShape();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetShape", reflectionSystem.GetOrCreateClass<physx::PxShape *>("physx::PxShape *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
const bool arg0 = *(const bool*)inArguments[0];
instance->SetIsTrigger(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIsTrigger", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetIsTrigger", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
Component * arg0 = (Component*)inArguments[0];
instance->OnComponentAdded(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inComponent", reflectionSystem.GetOrCreateClass<Component *>("Component *")));
Method& currentMethod = currentClass->AddMethod(Method("OnComponentAdded", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
Component * arg0 = (Component*)inArguments[0];
instance->OnComponentRemoved(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inComponent", reflectionSystem.GetOrCreateClass<Component *>("Component *")));
Method& currentMethod = currentClass->AddMethod(Method("OnComponentRemoved", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetLocalShapePosition(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOffset", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetLocalShapePosition", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetLocalShapeRotation(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRotation", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetLocalShapeRotation", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<DirectionalLightComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myIsShadowsEnabled", 16, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myColor", 20, reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
		currentField.AddMetadata(R"delim(ExposeAsColor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myLightProjection", 36, reflectionSystem.GetOrCreateClass<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
const glm::vec<4, float> & arg0 = *(const glm::vec<4, float>*)inArguments[0];
instance->SetColor(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inColor", reflectionSystem.GetOrCreateClass<const glm::vec<4, float> &>("const glm::vec<4, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetColor", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetIntensity(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inValue", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetIntensity", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
const glm::vec<4, float> & result = instance->GetColor();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetColor", reflectionSystem.GetOrCreateClass<const glm::vec<4, float> &>("const glm::vec<4, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
const glm::mat<4, 4, float> & result = instance->GetLightProjection();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetLightProjection", reflectionSystem.GetOrCreateClass<const glm::mat<4, 4, float> &>("const glm::mat<4, 4, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
static thread_local bool result = instance->IsShadowsEnabled();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsShadowsEnabled", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
instance->EnableShadows();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("EnableShadows", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<WorldSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myWorld", 8, reflectionSystem.GetOrCreateClass<World>("World"), true, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WorldSystem* instance = static_cast<WorldSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WorldSystem* instance = static_cast<WorldSystem*>(inInstance);
World * result = instance->GetWorld();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWorld", reflectionSystem.GetOrCreateClass<World *>("World *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeColliderComponent>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeColliderComponent* instance = static_cast<LandscapeColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeColliderComponent* instance = static_cast<LandscapeColliderComponent*>(inInstance);
instance->OnScaleChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnScaleChanged", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myColor", 16, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
		currentField.AddMetadata(R"delim(ExposeAsColor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myIntensity", 28, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myRange", 32, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
instance->TEMP_SendToGPU();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TEMP_SendToGPU", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
const glm::vec<3, float> & result = instance->GetColor();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetColor", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetColor(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inColor", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetColor", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
static thread_local float result = instance->GetIntensity();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIntensity", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetIntensity(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIntensity", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetIntensity", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
static thread_local float result = instance->GetRange();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRange", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetRange(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRange", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetRange", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PromiseReturnTypeImplementation<void>>();
	{
		Field& currentField = currentClass->AddField(Field("myReturnValue", 0, reflectionSystem.GetOrCreateClass<char>("char"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasReturnValue", 1, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<void>("void"), false, false);
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PromiseReturnTypeImplementation<void>* instance = static_cast<PromiseReturnTypeImplementation<void>*>(inInstance);
instance->return_void();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("return_void", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<String>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
const std::basic_string<wchar_t> & arg0 = *(const std::basic_string<wchar_t>*)inArguments[0];
static thread_local std::basic_string<char> result = instance->ToString(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<wchar_t> &>("const std::basic_string<wchar_t> &")));
Method& currentMethod = currentClass->AddMethod(Method("ToString", reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const std::basic_string<char> & arg1 = *(const std::basic_string<char>*)inArguments[1];
static thread_local bool result = instance->Contains(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inToSearchFor", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("Contains", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const std::basic_string<char> & arg1 = *(const std::basic_string<char>*)inArguments[1];
const std::basic_string<char> & arg2 = *(const std::basic_string<char>*)inArguments[2];
static thread_local std::basic_string<char> result = instance->Replace(arg0, arg1, arg2);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inToSearchFor", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inNewPart", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("Replace", reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local std::basic_string<wchar_t> result = instance->ToWString(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("ToWString", reflectionSystem.GetOrCreateClass<std::basic_string<wchar_t>>("std::basic_string<wchar_t>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const std::basic_string<char> & arg1 = *(const std::basic_string<char>*)inArguments[1];
static thread_local bool result = instance->EqualsIgnoreCase(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inA", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inB", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("EqualsIgnoreCase", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
std::basic_string<char> & arg0 = *(std::basic_string<char>*)inArguments[0];
std::basic_string<char> & result = instance->ToLower(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<std::basic_string<char> &>("std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("ToLower", reflectionSystem.GetOrCreateClass<std::basic_string<char> &>("std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local std::basic_string<char> result = instance->ToLowerCopy(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("ToLowerCopy", reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const std::basic_string<char> & arg1 = *(const std::basic_string<char>*)inArguments[1];
static thread_local List<std::basic_string<char>> result = instance->SplitString(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("string", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("delimiters", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("SplitString", reflectionSystem.GetOrCreateClass<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const char arg1 = *(const char*)inArguments[1];
static thread_local List<std::basic_string<char>> result = instance->SplitString(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("string", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("delimiter", reflectionSystem.GetOrCreateClass<const char>("const char")));
Method& currentMethod = currentClass->AddMethod(Method("SplitString", reflectionSystem.GetOrCreateClass<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SphereColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myRadius", 56, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SphereColliderComponent* instance = static_cast<SphereColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SphereColliderComponent* instance = static_cast<SphereColliderComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetRadius(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRadius", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetRadius", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StaticMeshComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myPath", 16, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterials", 48, reflectionSystem.GetOrCreateClass<List<Material *>>("List<Material *>"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myModel", 72, reflectionSystem.GetOrCreateClass<Model>("Model"), true, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMeshInstances", 80, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
Model * arg0 = (Model*)inArguments[0];
instance->SetModel(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inModel", reflectionSystem.GetOrCreateClass<Model *>("Model *")));
Method& currentMethod = currentClass->AddMethod(Method("SetModel", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
Model * result = instance->GetModel();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetModel", reflectionSystem.GetOrCreateClass<Model *>("Model *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
instance->SetMaterialAsync(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMaterialPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inIndex", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetMaterialAsync", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
Material * arg0 = (Material*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
instance->SetMaterial(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMaterial", reflectionSystem.GetOrCreateClass<Material *>("Material *")));
arguments.Add(MethodArgument("inIndex", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetMaterial", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
const unsigned int arg0 = *(const unsigned int*)inArguments[0];
const Material * result = instance->GetMaterial(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIndex", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("GetMaterial", reflectionSystem.GetOrCreateClass<const Material *>("const Material *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
Material * arg0 = (Material*)inArguments[0];
Mesh * arg1 = (Mesh*)inArguments[1];
instance->SetMaterialForMesh(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMaterial", reflectionSystem.GetOrCreateClass<Material *>("Material *")));
arguments.Add(MethodArgument("inMesh", reflectionSystem.GetOrCreateClass<Mesh *>("Mesh *")));
Method& currentMethod = currentClass->AddMethod(Method("SetMaterialForMesh", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
Mesh * arg0 = (Mesh*)inArguments[0];
const Material * result = instance->GetMaterialForMesh(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMesh", reflectionSystem.GetOrCreateClass<Mesh *>("Mesh *")));
Method& currentMethod = currentClass->AddMethod(Method("GetMaterialForMesh", reflectionSystem.GetOrCreateClass<const Material *>("const Material *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
instance->OnRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnRenderStateDirty", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IsCopyable<int>>();
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<int>("int"), false, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IList>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<List<std::basic_string<char>>>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<IList>());
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<List<MethodArgument>>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<IList>());
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<MethodArgument>("MethodArgument"), false, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<List<const Class *>>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<IList>());
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<const Class>("const Class"), true, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<List<Field>>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<IList>());
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<Field>("Field"), false, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<List<Method>>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<IList>());
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<Method>("Method"), false, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<List<ClassTemplateArgument>>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<IList>());
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<ClassTemplateArgument>("ClassTemplateArgument"), false, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<List<Class *>>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<IList>());
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<Class>("Class"), true, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AutoInit>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AutoInitManager>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AutoInitManager* instance = static_cast<AutoInitManager*>(inInstance);
AutoInit * arg0 = (AutoInit*)inArguments[0];
instance->AddInitObject(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", reflectionSystem.GetOrCreateClass<AutoInit *>("AutoInit *")));
Method& currentMethod = currentClass->AddMethod(Method("AddInitObject", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AutoInitManager* instance = static_cast<AutoInitManager*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Field>();
	{
		Field& currentField = currentClass->AddField(Field("myName", 0, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myOffset", 32, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myType", 40, reflectionSystem.GetOrCreateClass<const Class>("const Class"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsPointer", 48, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsReference", 49, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMetadata", 56, reflectionSystem.GetOrCreateClass<List<std::basic_string<char>>>("List<std::basic_string<char>>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
const std::basic_string<char> & result = instance->GetName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
static thread_local unsigned int result = instance->GetOffset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetOffset", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
const Class * result = instance->GetClass();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
const List<std::basic_string<char>> & result = instance->GetMetadata();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMetadata", reflectionSystem.GetOrCreateClass<const List<std::basic_string<char>> &>("const List<std::basic_string<char>> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local bool result = instance->HasMetadata(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMetadata", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("HasMetadata", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
static thread_local bool result = instance->IsPointer();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsPointer", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
static thread_local bool result = instance->IsReference();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsReference", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local List<std::basic_string<char>> result = instance->GetMetadataArgs(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMetadata", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetMetadataArgs", reflectionSystem.GetOrCreateClass<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
void * arg0 = (void*)inArguments[0];
void * result = instance->GetPointerToValue(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
Method& currentMethod = currentClass->AddMethod(Method("GetPointerToValue", reflectionSystem.GetOrCreateClass<void *>("void *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Delegate<void *(void *, const List<void *> &)>>();
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<void *(void *, const List<void *> &)>("void *(void *, const List<void *> &)"), false, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Delegate<void *()>>();
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<void *()>("void *()"), false, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Awaitable>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Awaitable* instance = static_cast<Awaitable*>(inInstance);
static thread_local bool result = instance->await_ready();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("await_ready", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Awaitable* instance = static_cast<Awaitable*>(inInstance);
std::coroutine_handle<void> arg0 = *(std::coroutine_handle<void>*)inArguments[0];
instance->await_suspend(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCoroutineHandle", reflectionSystem.GetOrCreateClass<std::coroutine_handle<void>>("std::coroutine_handle<void>")));
Method& currentMethod = currentClass->AddMethod(Method("await_suspend", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Awaitable* instance = static_cast<Awaitable*>(inInstance);
instance->await_resume();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("await_resume", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RenderSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myIsUsingGPUDrivenRendering", 32, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCopyPipeline", 40, reflectionSystem.GetOrCreateClass<FullscreenPipeline>("FullscreenPipeline"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDebugPipeline", 48, reflectionSystem.GetOrCreateClass<DebugPipeline>("DebugPipeline"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGDRPipeline", 56, reflectionSystem.GetOrCreateClass<GDRPipeline>("GDRPipeline"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkyboxPipeline", 64, reflectionSystem.GetOrCreateClass<SkyboxPipeline>("SkyboxPipeline"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderPass", 72, reflectionSystem.GetOrCreateClass<vk::RenderPass>("vk::RenderPass"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCopyToSwapchainRenderPass", 80, reflectionSystem.GetOrCreateClass<vk::RenderPass>("vk::RenderPass"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVkFrameBuffer", 88, reflectionSystem.GetOrCreateClass<vk::Framebuffer>("vk::Framebuffer"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCopyToSwapchainFrameBuffers", 96, reflectionSystem.GetOrCreateClass<List<vk::Framebuffer>>("List<vk::Framebuffer>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myClearValues", 120, reflectionSystem.GetOrCreateClass<vk::ClearValue[2]>("vk::ClearValue[2]"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDepthBuffer", 152, reflectionSystem.GetOrCreateClass<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderTexture", 160, reflectionSystem.GetOrCreateClass<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myResolvedRenderTexture", 168, reflectionSystem.GetOrCreateClass<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirectionalLightShadowMap", 176, reflectionSystem.GetOrCreateClass<VulkanImage>("VulkanImage"), true, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<EventObserver>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
instance->Init();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Init", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
vk::RenderPass & result = instance->GetRenderPass();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRenderPass", reflectionSystem.GetOrCreateClass<vk::RenderPass &>("vk::RenderPass &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
vk::RenderPass & result = instance->GetImGuiRenderPass();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImGuiRenderPass", reflectionSystem.GetOrCreateClass<vk::RenderPass &>("vk::RenderPass &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
VulkanImage * result = instance->GetRenderTexture();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRenderTexture", reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
VulkanImage * result = instance->GetResolvedRenderTexture();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetResolvedRenderTexture", reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
VulkanImage * result = instance->GetDepthTexture();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDepthTexture", reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
VulkanImage * result = instance->GetDirectionalLightShadowMap();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDirectionalLightShadowMap", reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
instance->OnSwapChainResize();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnSwapChainResize", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
static thread_local vk::CommandBuffer result = instance->CreateUploadCommandBuffer_TS();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateUploadCommandBuffer_TS", reflectionSystem.GetOrCreateClass<vk::CommandBuffer>("vk::CommandBuffer"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
vk::CommandBuffer arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->QueueCommandBufferForUpload_TS(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("commandBuffer", reflectionSystem.GetOrCreateClass<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("QueueCommandBufferForUpload_TS", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
const GDRPipeline & result = instance->GetGDRPipeline();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGDRPipeline", reflectionSystem.GetOrCreateClass<const GDRPipeline &>("const GDRPipeline &"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MulticastDelegate<void ()>>();
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<void ()>("void ()"), false, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MulticastDelegate<void (Component *)>>();
	currentClass->AddTemplateArgument(reflectionSystem.GetOrCreateClass<void (Component *)>("void (Component *)"), false, false);
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TestClass>();
	{
		Field& currentField = currentClass->AddField(Field("SomeFloat", 0, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("SomeField", 4, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Engine>();
	{
		Field& currentField = currentClass->AddField(Field("myFrameIndex", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsRunning", 4, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myEngineProperties", 8, reflectionSystem.GetOrCreateClass<EngineProperties>("EngineProperties"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySystemManager", 120, reflectionSystem.GetOrCreateClass<SystemManager<System>>("SystemManager<System>"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPostMaster", 128, reflectionSystem.GetOrCreateClass<EventHandler>("EventHandler"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myThreadPool", 136, reflectionSystem.GetOrCreateClass<ThreadPool>("ThreadPool"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFilewatcher", 144, reflectionSystem.GetOrCreateClass<Filewatcher>("Filewatcher"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWindowHandler", 152, reflectionSystem.GetOrCreateClass<WindowHandler>("WindowHandler"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVulkanContext", 160, reflectionSystem.GetOrCreateClass<VulkanContext>("VulkanContext"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAssetRegistry", 168, reflectionSystem.GetOrCreateClass<AssetRegistry>("AssetRegistry"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWorld", 176, reflectionSystem.GetOrCreateClass<World>("World"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myExternalTickFunction", 184, reflectionSystem.GetOrCreateClass<std::function<void ()>>("std::function<void ()>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
static thread_local bool result = instance->ShouldRun();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ShouldRun", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
static thread_local unsigned int result = instance->GetFrameIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFrameIndex", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
const bool arg0 = *(const bool*)inArguments[0];
instance->SetIsRunning(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIsRunning", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetIsRunning", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
ReflectionSystem & result = instance->GetReflectionSystem();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetReflectionSystem", reflectionSystem.GetOrCreateClass<ReflectionSystem &>("ReflectionSystem &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
const EngineProperties & result = instance->GetEngineProperties();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetEngineProperties", reflectionSystem.GetOrCreateClass<const EngineProperties &>("const EngineProperties &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
const WindowHandler & result = instance->GetWindowHandler();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWindowHandler", reflectionSystem.GetOrCreateClass<const WindowHandler &>("const WindowHandler &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
EventHandler & result = instance->GetEventHandler();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetEventHandler", reflectionSystem.GetOrCreateClass<EventHandler &>("EventHandler &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
AssetRegistry & result = instance->GetAssetRegistry();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetRegistry", reflectionSystem.GetOrCreateClass<AssetRegistry &>("AssetRegistry &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
ThreadPool & result = instance->GetThreadPool();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetThreadPool", reflectionSystem.GetOrCreateClass<ThreadPool &>("ThreadPool &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
Filewatcher & result = instance->GetFilewatcher();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFilewatcher", reflectionSystem.GetOrCreateClass<Filewatcher &>("Filewatcher &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
World & result = instance->GetWorld();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWorld", reflectionSystem.GetOrCreateClass<World &>("World &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
World * arg0 = (World*)inArguments[0];
instance->SetWorld(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inWorld", reflectionSystem.GetOrCreateClass<World *>("World *")));
Method& currentMethod = currentClass->AddMethod(Method("SetWorld", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
static thread_local glm::vec<2, float> result = instance->GetRenderResolution();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRenderResolution", reflectionSystem.GetOrCreateClass<glm::vec<2, float>>("glm::vec<2, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
const std::function<void ()> arg0 = *(const std::function<void ()>*)inArguments[0];
instance->SetExternalTickFunction(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inExternalTickFunction", reflectionSystem.GetOrCreateClass<const std::function<void ()>>("const std::function<void ()>")));
Method& currentMethod = currentClass->AddMethod(Method("SetExternalTickFunction", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EngineProperties>();
	{
		Field& currentField = currentClass->AddField(Field("Title", 0, reflectionSystem.GetOrCreateClass<std::basic_string<wchar_t>>("std::basic_string<wchar_t>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowWidth", 32, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowHeight", 36, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowX", 40, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowY", 44, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("StartupArguments", 48, reflectionSystem.GetOrCreateClass<std::unordered_set<std::basic_string<char>>>("std::unordered_set<std::basic_string<char>>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EngineProperties* instance = static_cast<EngineProperties*>(inInstance);
int arg0 = *(int*)inArguments[0];
char ** arg1 = (char **)inArguments[1];
instance->AddStartupArguments(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inArgC", reflectionSystem.GetOrCreateClass<int>("int")));
arguments.Add(MethodArgument("inArgv", reflectionSystem.GetOrCreateClass<char **>("char **")));
Method& currentMethod = currentClass->AddMethod(Method("AddStartupArguments", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EngineProperties* instance = static_cast<EngineProperties*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
instance->AddStartupArgument(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inArg", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("AddStartupArgument", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EngineProperties* instance = static_cast<EngineProperties*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local bool result = instance->HasStartupArgument(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inArg", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("HasStartupArgument", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventHandler>();
	{
		Field& currentField = currentClass->AddField(Field("myObservers", 0, reflectionSystem.GetOrCreateClass<List<EventObserver *>>("List<EventObserver *>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EventHandler* instance = static_cast<EventHandler*>(inInstance);
const EventType arg0 = *(const EventType*)inArguments[0];
instance->FireEvent(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", reflectionSystem.GetOrCreateClass<const EventType>("const EventType")));
Method& currentMethod = currentClass->AddMethod(Method("FireEvent", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventObserver>();
	{
		Field& currentField = currentClass->AddField(Field("myEvents", 8, reflectionSystem.GetOrCreateClass<std::map<EventType, std::function<void ()>>>("std::map<EventType, std::function<void ()>>"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Color>();
	{
		Field& currentField = currentClass->AddField(Field("myR", 0, reflectionSystem.GetOrCreateClass<unsigned char>("unsigned char"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myG", 1, reflectionSystem.GetOrCreateClass<unsigned char>("unsigned char"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myB", 2, reflectionSystem.GetOrCreateClass<unsigned char>("unsigned char"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myA", 3, reflectionSystem.GetOrCreateClass<unsigned char>("unsigned char"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local LinearColor result = instance->ToLinearColor();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ToLinearColor", reflectionSystem.GetOrCreateClass<LinearColor>("LinearColor"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local std::basic_string<char> result = instance->ToString();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ToString", reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local Color result = instance->White();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("White", reflectionSystem.GetOrCreateClass<Color>("Color"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local Color result = instance->Black();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Black", reflectionSystem.GetOrCreateClass<Color>("Color"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local Color result = instance->Red();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Red", reflectionSystem.GetOrCreateClass<Color>("Color"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local Color result = instance->Green();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Green", reflectionSystem.GetOrCreateClass<Color>("Color"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local Color result = instance->Blue();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Blue", reflectionSystem.GetOrCreateClass<Color>("Color"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myPhysicsCommands", 16, reflectionSystem.GetOrCreateClass<List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>>("List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDefaultErrorCallback", 40, reflectionSystem.GetOrCreateClass<physx::PxErrorCallback>("physx::PxErrorCallback"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDefaultAllocator", 48, reflectionSystem.GetOrCreateClass<physx::PxDefaultAllocator>("physx::PxDefaultAllocator"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myToleranceScale", 56, reflectionSystem.GetOrCreateClass<physx::PxTolerancesScale>("physx::PxTolerancesScale"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFoundation", 64, reflectionSystem.GetOrCreateClass<physx::PxFoundation>("physx::PxFoundation"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPvd", 72, reflectionSystem.GetOrCreateClass<physx::PxPvd>("physx::PxPvd"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPvdTransport", 80, reflectionSystem.GetOrCreateClass<physx::PxPvdTransport>("physx::PxPvdTransport"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDefaultMaterial", 88, reflectionSystem.GetOrCreateClass<physx::PxMaterial>("physx::PxMaterial"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myListener", 96, reflectionSystem.GetOrCreateClass<PhysicsListener>("PhysicsListener"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPhysics", 104, reflectionSystem.GetOrCreateClass<physx::PxPhysics>("physx::PxPhysics"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myScene", 112, reflectionSystem.GetOrCreateClass<physx::PxScene>("physx::PxScene"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myControllerManager", 120, reflectionSystem.GetOrCreateClass<physx::PxControllerManager>("physx::PxControllerManager"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasActiveSimulation", 128, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<WorldSystem>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
Delegate<void (physx::PxPhysics *, physx::PxScene *)> arg0 = *(Delegate<void (physx::PxPhysics *, physx::PxScene *)>*)inArguments[0];
instance->QueuePhysicsCommand(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inDelegate", reflectionSystem.GetOrCreateClass<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>("Delegate<void (physx::PxPhysics *, physx::PxScene *)>")));
Method& currentMethod = currentClass->AddMethod(Method("QueuePhysicsCommand", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
physx::PxTolerancesScale * result = instance->GetToleranceScale();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetToleranceScale", reflectionSystem.GetOrCreateClass<physx::PxTolerancesScale *>("physx::PxTolerancesScale *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
physx::PxMaterial * result = instance->GetDefaultMaterial();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDefaultMaterial", reflectionSystem.GetOrCreateClass<physx::PxMaterial *>("physx::PxMaterial *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
physx::PxScene * result = instance->GetScene();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetScene", reflectionSystem.GetOrCreateClass<physx::PxScene *>("physx::PxScene *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
physx::PxControllerManager * result = instance->GetControllerManager();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetControllerManager", reflectionSystem.GetOrCreateClass<physx::PxControllerManager *>("physx::PxControllerManager *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
PhysicsListener * result = instance->GetPhysicsListener();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPhysicsListener", reflectionSystem.GetOrCreateClass<PhysicsListener *>("PhysicsListener *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Heightfield>();
	{
		Field& currentField = currentClass->AddField(Field("mySeed", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPerlinNoise", 4, reflectionSystem.GetOrCreateClass<siv::BasicPerlinNoise<float>>("siv::BasicPerlinNoise<float>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Heightfield* instance = static_cast<Heightfield*>(inInstance);
const glm::vec<2, float> arg0 = *(const glm::vec<2, float>*)inArguments[0];
static thread_local float result = instance->GetHeight(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCoordinates", reflectionSystem.GetOrCreateClass<const glm::vec<2, float>>("const glm::vec<2, float>")));
Method& currentMethod = currentClass->AddMethod(Method("GetHeight", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LinearColor>();
	{
		Field& currentField = currentClass->AddField(Field("myR", 0, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myG", 4, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myB", 8, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myA", 12, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LinearColor* instance = static_cast<LinearColor*>(inInstance);
static thread_local Color result = instance->ToColor();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ToColor", reflectionSystem.GetOrCreateClass<Color>("Color"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsErrorCallback>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxErrorCallback>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsErrorCallback* instance = static_cast<PhysicsErrorCallback*>(inInstance);
physx::PxErrorCode::Enum arg0 = *(physx::PxErrorCode::Enum*)inArguments[0];
const char * arg1 = (const char*)inArguments[1];
const char * arg2 = (const char*)inArguments[2];
int arg3 = *(int*)inArguments[3];
instance->reportError(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("code", reflectionSystem.GetOrCreateClass<physx::PxErrorCode::Enum>("physx::PxErrorCode::Enum")));
arguments.Add(MethodArgument("message", reflectionSystem.GetOrCreateClass<const char *>("const char *")));
arguments.Add(MethodArgument("file", reflectionSystem.GetOrCreateClass<const char *>("const char *")));
arguments.Add(MethodArgument("line", reflectionSystem.GetOrCreateClass<int>("int")));
Method& currentMethod = currentClass->AddMethod(Method("reportError", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsListener>();
	{
		Field& currentField = currentClass->AddField(Field("myDequeueCollisionsDelegate", 32, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxSimulationEventCallback>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxUserControllerHitReport>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxControllerBehaviorCallback>());
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<physx::PxControllerFilterCallback>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
physx::PxTriggerPair * arg0 = (physx::PxTriggerPair*)inArguments[0];
unsigned int arg1 = *(unsigned int*)inArguments[1];
instance->onTrigger(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("pairs", reflectionSystem.GetOrCreateClass<physx::PxTriggerPair *>("physx::PxTriggerPair *")));
arguments.Add(MethodArgument("count", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onTrigger", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxContactPairHeader & arg0 = *(const physx::PxContactPairHeader*)inArguments[0];
const physx::PxContactPair * arg1 = (const physx::PxContactPair*)inArguments[1];
unsigned int arg2 = *(unsigned int*)inArguments[2];
instance->onContact(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("pairHeader", reflectionSystem.GetOrCreateClass<const physx::PxContactPairHeader &>("const physx::PxContactPairHeader &")));
arguments.Add(MethodArgument("pairs", reflectionSystem.GetOrCreateClass<const physx::PxContactPair *>("const physx::PxContactPair *")));
arguments.Add(MethodArgument("nbPairs", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onContact", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
physx::PxConstraintInfo * arg0 = (physx::PxConstraintInfo*)inArguments[0];
unsigned int arg1 = *(unsigned int*)inArguments[1];
instance->onConstraintBreak(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("constraints", reflectionSystem.GetOrCreateClass<physx::PxConstraintInfo *>("physx::PxConstraintInfo *")));
arguments.Add(MethodArgument("count", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onConstraintBreak", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
physx::PxActor ** arg0 = (physx::PxActor **)inArguments[0];
unsigned int arg1 = *(unsigned int*)inArguments[1];
instance->onWake(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("actors", reflectionSystem.GetOrCreateClass<physx::PxActor **>("physx::PxActor **")));
arguments.Add(MethodArgument("count", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onWake", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
physx::PxActor ** arg0 = (physx::PxActor **)inArguments[0];
unsigned int arg1 = *(unsigned int*)inArguments[1];
instance->onSleep(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("actors", reflectionSystem.GetOrCreateClass<physx::PxActor **>("physx::PxActor **")));
arguments.Add(MethodArgument("count", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onSleep", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxRigidBody *const * arg0 = (const physx::PxRigidBody *const*)inArguments[0];
const physx::PxTransformT<float> * arg1 = (const physx::PxTransformT<float>*)inArguments[1];
const unsigned int arg2 = *(const unsigned int*)inArguments[2];
instance->onAdvance(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("bodyBuffer", reflectionSystem.GetOrCreateClass<const physx::PxRigidBody *const *>("const physx::PxRigidBody *const *")));
arguments.Add(MethodArgument("poseBuffer", reflectionSystem.GetOrCreateClass<const physx::PxTransformT<float> *>("const physx::PxTransformT<float> *")));
arguments.Add(MethodArgument("count", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onAdvance", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxControllerShapeHit & arg0 = *(const physx::PxControllerShapeHit*)inArguments[0];
instance->onShapeHit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("hit", reflectionSystem.GetOrCreateClass<const physx::PxControllerShapeHit &>("const physx::PxControllerShapeHit &")));
Method& currentMethod = currentClass->AddMethod(Method("onShapeHit", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxControllersHit & arg0 = *(const physx::PxControllersHit*)inArguments[0];
instance->onControllerHit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("hit", reflectionSystem.GetOrCreateClass<const physx::PxControllersHit &>("const physx::PxControllersHit &")));
Method& currentMethod = currentClass->AddMethod(Method("onControllerHit", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxControllerObstacleHit & arg0 = *(const physx::PxControllerObstacleHit*)inArguments[0];
instance->onObstacleHit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("hit", reflectionSystem.GetOrCreateClass<const physx::PxControllerObstacleHit &>("const physx::PxControllerObstacleHit &")));
Method& currentMethod = currentClass->AddMethod(Method("onObstacleHit", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxShape & arg0 = *(const physx::PxShape*)inArguments[0];
const physx::PxActor & arg1 = *(const physx::PxActor*)inArguments[1];
static thread_local physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char> result = instance->getBehaviorFlags(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("shape", reflectionSystem.GetOrCreateClass<const physx::PxShape &>("const physx::PxShape &")));
arguments.Add(MethodArgument("actor", reflectionSystem.GetOrCreateClass<const physx::PxActor &>("const physx::PxActor &")));
Method& currentMethod = currentClass->AddMethod(Method("getBehaviorFlags", reflectionSystem.GetOrCreateClass<physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>>("physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxController & arg0 = *(const physx::PxController*)inArguments[0];
static thread_local physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char> result = instance->getBehaviorFlags(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("controller", reflectionSystem.GetOrCreateClass<const physx::PxController &>("const physx::PxController &")));
Method& currentMethod = currentClass->AddMethod(Method("getBehaviorFlags", reflectionSystem.GetOrCreateClass<physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>>("physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxObstacle & arg0 = *(const physx::PxObstacle*)inArguments[0];
static thread_local physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char> result = instance->getBehaviorFlags(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("obstacle", reflectionSystem.GetOrCreateClass<const physx::PxObstacle &>("const physx::PxObstacle &")));
Method& currentMethod = currentClass->AddMethod(Method("getBehaviorFlags", reflectionSystem.GetOrCreateClass<physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>>("physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxController & arg0 = *(const physx::PxController*)inArguments[0];
const physx::PxController & arg1 = *(const physx::PxController*)inArguments[1];
static thread_local bool result = instance->filter(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("a", reflectionSystem.GetOrCreateClass<const physx::PxController &>("const physx::PxController &")));
arguments.Add(MethodArgument("b", reflectionSystem.GetOrCreateClass<const physx::PxController &>("const physx::PxController &")));
Method& currentMethod = currentClass->AddMethod(Method("filter", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RaycastHit>();
	{
		Field& currentField = currentClass->AddField(Field("myHitPosition", 0, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitNormal", 12, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitDistance", 24, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitGameObject", 32, reflectionSystem.GetOrCreateClass<GameObject>("GameObject"), true, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ClassTemplateArgument>();
	{
		Field& currentField = currentClass->AddField(Field("myType", 0, reflectionSystem.GetOrCreateClass<const Class>("const Class"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsPointer", 8, reflectionSystem.GetOrCreateClass<const bool>("const bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsReference", 9, reflectionSystem.GetOrCreateClass<const bool>("const bool"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Class>();
	{
		Field& currentField = currentClass->AddField(Field("myFullName", 0, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myClassName", 32, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myByteSize", 64, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsCopyable", 68, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFactoryFunction", 72, reflectionSystem.GetOrCreateClass<Delegate<void *()>>("Delegate<void *()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBaseClasses", 80, reflectionSystem.GetOrCreateClass<List<const Class *>>("List<const Class *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDerivedClasses", 104, reflectionSystem.GetOrCreateClass<List<const Class *>>("List<const Class *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFields", 128, reflectionSystem.GetOrCreateClass<List<Field>>("List<Field>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMethods", 152, reflectionSystem.GetOrCreateClass<List<Method>>("List<Method>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTemplateArguments", 176, reflectionSystem.GetOrCreateClass<List<ClassTemplateArgument>>("List<ClassTemplateArgument>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const std::basic_string<char> & result = instance->GetName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const std::basic_string<char> & result = instance->GetFullName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFullName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const List<const Class *> & result = instance->GetBaseClasses();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBaseClasses", reflectionSystem.GetOrCreateClass<const List<const Class *> &>("const List<const Class *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const List<const Class *> & result = instance->GetDerivedClasses();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDerivedClasses", reflectionSystem.GetOrCreateClass<const List<const Class *> &>("const List<const Class *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const List<Field> & result = instance->GetFields();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFields", reflectionSystem.GetOrCreateClass<const List<Field> &>("const List<Field> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const List<Method> & result = instance->GetMethods();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMethods", reflectionSystem.GetOrCreateClass<const List<Method> &>("const List<Method> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
static thread_local unsigned int result = instance->GetSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSize", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Field * result = instance->FindFieldByName(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFieldName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("FindFieldByName", reflectionSystem.GetOrCreateClass<const Field *>("const Field *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
static thread_local bool result = instance->IsCopyable();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsCopyable", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local List<Field> result = instance->GetFieldsWithMetadata(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMetadata", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetFieldsWithMetadata", reflectionSystem.GetOrCreateClass<List<Field>>("List<Field>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local List<const Method *> result = instance->GetMethodsWithMetadata(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMetadata", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetMethodsWithMetadata", reflectionSystem.GetOrCreateClass<List<const Method *>>("List<const Method *>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Method * result = instance->GetMethod(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMethodName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetMethod", reflectionSystem.GetOrCreateClass<const Method *>("const Method *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
static thread_local bool result = instance->IsTemplateSpecialization();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsTemplateSpecialization", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const List<ClassTemplateArgument> & result = instance->GetTemplateArguments();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTemplateArguments", reflectionSystem.GetOrCreateClass<const List<ClassTemplateArgument> &>("const List<ClassTemplateArgument> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local std::basic_string<char> result = instance->GetClassNameWithoutForwardDeclares(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClassName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetClassNameWithoutForwardDeclares", reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Class* instance = static_cast<Class*>(inInstance);
const Class * arg0 = (const Class*)inArguments[0];
const bool arg1 = *(const bool*)inArguments[1];
const bool arg2 = *(const bool*)inArguments[2];
instance->AddTemplateArgument(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTemplateArgumentClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
arguments.Add(MethodArgument("inIsPointer", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
arguments.Add(MethodArgument("inIsReference", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("AddTemplateArgument", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ReflectionSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myClasses", 8, reflectionSystem.GetOrCreateClass<List<Class *>>("List<Class *>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ReflectionSystem* instance = static_cast<ReflectionSystem*>(inInstance);
const List<Class *> & result = instance->GetAllClasses();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAllClasses", reflectionSystem.GetOrCreateClass<const List<Class *> &>("const List<Class *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ReflectionSystem* instance = static_cast<ReflectionSystem*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Class * result = instance->GetClassByName(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetClassByName", reflectionSystem.GetOrCreateClass<const Class *>("const Class *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ReflectionSystem* instance = static_cast<ReflectionSystem*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Class * result = instance->GetClassByFullName(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFullName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetClassByFullName", reflectionSystem.GetOrCreateClass<const Class *>("const Class *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<DebugPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", 0, reflectionSystem.GetOrCreateClass<Shader>("Shader"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", 8, reflectionSystem.GetOrCreateClass<Shader>("Shader"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", 16, reflectionSystem.GetOrCreateClass<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", 24, reflectionSystem.GetOrCreateClass<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDescriptorSet", 32, reflectionSystem.GetOrCreateClass<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", 128, reflectionSystem.GetOrCreateClass<VulkanBuffer>("VulkanBuffer"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DebugPipeline* instance = static_cast<DebugPipeline*>(inInstance);
const vk::CommandBuffer arg0 = *(const vk::CommandBuffer*)inArguments[0];
instance->AddDrawCommands(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", reflectionSystem.GetOrCreateClass<const vk::CommandBuffer>("const vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("AddDrawCommands", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<FullscreenPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", 0, reflectionSystem.GetOrCreateClass<Shader>("Shader"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", 8, reflectionSystem.GetOrCreateClass<Shader>("Shader"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", 16, reflectionSystem.GetOrCreateClass<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", 24, reflectionSystem.GetOrCreateClass<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderPass", 32, reflectionSystem.GetOrCreateClass<vk::RenderPass>("vk::RenderPass"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorSet", 40, reflectionSystem.GetOrCreateClass<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
FullscreenPipeline* instance = static_cast<FullscreenPipeline*>(inInstance);
const vk::CommandBuffer arg0 = *(const vk::CommandBuffer*)inArguments[0];
instance->AddFullscreenPass(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", reflectionSystem.GetOrCreateClass<const vk::CommandBuffer>("const vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("AddFullscreenPass", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myPrePassShader", 0, reflectionSystem.GetOrCreateClass<Shader>("Shader"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCullShader", 8, reflectionSystem.GetOrCreateClass<Shader>("Shader"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCubemap", 16, reflectionSystem.GetOrCreateClass<TextureCube>("TextureCube"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndirectCommandsBuffer", 88, reflectionSystem.GetOrCreateClass<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCountBuffer", 96, reflectionSystem.GetOrCreateClass<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPerDrawDataBuffer", 104, reflectionSystem.GetOrCreateClass<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDescriptorSet", 112, reflectionSystem.GetOrCreateClass<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", 208, reflectionSystem.GetOrCreateClass<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", 216, reflectionSystem.GetOrCreateClass<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", 224, reflectionSystem.GetOrCreateClass<Shader>("Shader"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", 232, reflectionSystem.GetOrCreateClass<Shader>("Shader"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", 240, reflectionSystem.GetOrCreateClass<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirectionalLightBuffer", 248, reflectionSystem.GetOrCreateClass<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirtyTransforms", 256, reflectionSystem.GetOrCreateClass<List<TransformComponent *>>("List<TransformComponent *>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GDRPipeline* instance = static_cast<GDRPipeline*>(inInstance);
vk::CommandBuffer arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->AddComputeCommands(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", reflectionSystem.GetOrCreateClass<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("AddComputeCommands", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GDRPipeline* instance = static_cast<GDRPipeline*>(inInstance);
vk::CommandBuffer arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->AddGraphicsCommands(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", reflectionSystem.GetOrCreateClass<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("AddGraphicsCommands", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GDRPipeline* instance = static_cast<GDRPipeline*>(inInstance);
VulkanBuffer * result = instance->GetCountBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetCountBuffer", reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GDRPipeline* instance = static_cast<GDRPipeline*>(inInstance);
VulkanBuffer * result = instance->GetIndirectBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIndirectBuffer", reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GDRPipeline* instance = static_cast<GDRPipeline*>(inInstance);
ResizableBuffer * result = instance->GetPerDrawDataBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPerDrawDataBuffer", reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferHandle>();
	{
		Field& currentField = currentClass->AddField(Field("myIndex", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferHandle* instance = static_cast<IndexBufferHandle*>(inInstance);
static thread_local unsigned int result = instance->GetIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIndex", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myUsedBufferSize", 8, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCurrentIndexOffset", 12, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexBuffers", 16, reflectionSystem.GetOrCreateClass<List<IndexBufferHandle *>>("List<IndexBufferHandle *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 40, reflectionSystem.GetOrCreateClass<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseIndexDataBuffer", 48, reflectionSystem.GetOrCreateClass<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseIndexData_CPURepresentation", 56, reflectionSystem.GetOrCreateClass<List<IndexBufferData>>("List<IndexBufferData>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFreeSparseIndices", 80, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferSystem* instance = static_cast<IndexBufferSystem*>(inInstance);
const List<unsigned int> & arg0 = *(const List<unsigned int>*)inArguments[0];
IndexBufferHandle * result = instance->UploadIndexBuffer(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIndices", reflectionSystem.GetOrCreateClass<const List<unsigned int> &>("const List<unsigned int> &")));
Method& currentMethod = currentClass->AddMethod(Method("UploadIndexBuffer", reflectionSystem.GetOrCreateClass<IndexBufferHandle *>("IndexBufferHandle *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferSystem* instance = static_cast<IndexBufferSystem*>(inInstance);
VulkanBuffer * arg0 = (VulkanBuffer*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
IndexBufferHandle * result = instance->UploadIndexBuffer(arg0, arg1);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inStagingBuffer", reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
arguments.Add(MethodArgument("inIndexCount", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("UploadIndexBuffer", reflectionSystem.GetOrCreateClass<IndexBufferHandle *>("IndexBufferHandle *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferSystem* instance = static_cast<IndexBufferSystem*>(inInstance);
IndexBufferHandle * arg0 = (IndexBufferHandle*)inArguments[0];
const IndexBufferData & result = instance->GetIndexBufferDataFromIndexHandle(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inHandle", reflectionSystem.GetOrCreateClass<IndexBufferHandle *>("IndexBufferHandle *")));
Method& currentMethod = currentClass->AddMethod(Method("GetIndexBufferDataFromIndexHandle", reflectionSystem.GetOrCreateClass<const IndexBufferData &>("const IndexBufferData &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferSystem* instance = static_cast<IndexBufferSystem*>(inInstance);
const IndexBufferHandle * arg0 = (const IndexBufferHandle*)inArguments[0];
instance->RemoveIndexBuffer(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBuffer", reflectionSystem.GetOrCreateClass<const IndexBufferHandle *>("const IndexBufferHandle *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveIndexBuffer", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferSystem* instance = static_cast<IndexBufferSystem*>(inInstance);
const ResizableBuffer * result = instance->GetGlobalIndexBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGlobalIndexBuffer", reflectionSystem.GetOrCreateClass<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferSystem* instance = static_cast<IndexBufferSystem*>(inInstance);
const ResizableBuffer * result = instance->GetGlobalSparseIndexDataBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGlobalSparseIndexDataBuffer", reflectionSystem.GetOrCreateClass<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Mesh>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexBuffer", 0, reflectionSystem.GetOrCreateClass<VertexBufferHandle>("VertexBufferHandle"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexBuffer", 8, reflectionSystem.GetOrCreateClass<IndexBufferHandle>("IndexBufferHandle"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySphereBounds", 16, reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHandle", 32, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoPath", 40, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalPath", 72, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialPath", 104, reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
VertexBufferHandle * result = instance->GetVertexBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetVertexBuffer", reflectionSystem.GetOrCreateClass<VertexBufferHandle *>("VertexBufferHandle *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
IndexBufferHandle * result = instance->GetIndexBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIndexBuffer", reflectionSystem.GetOrCreateClass<IndexBufferHandle *>("IndexBufferHandle *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
const glm::vec<4, float> & result = instance->GetSphereBounds();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSphereBounds", reflectionSystem.GetOrCreateClass<const glm::vec<4, float> &>("const glm::vec<4, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
static thread_local unsigned int result = instance->GetHandle();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHandle", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
const std::basic_string<char> & result = instance->GetAlbedoPath();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAlbedoPath", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
const std::basic_string<char> & result = instance->GetNormalPath();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNormalPath", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
const std::basic_string<char> & result = instance->GetMaterialPath();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMaterialPath", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myMeshes", 8, reflectionSystem.GetOrCreateClass<List<Mesh *>>("List<Mesh *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 32, reflectionSystem.GetOrCreateClass<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumObjects", 40, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshSystem* instance = static_cast<MeshSystem*>(inInstance);
VertexBufferHandle * arg0 = (VertexBufferHandle*)inArguments[0];
IndexBufferHandle * arg1 = (IndexBufferHandle*)inArguments[1];
const glm::vec<4, float> & arg2 = *(const glm::vec<4, float>*)inArguments[2];
Mesh * result = instance->UploadMesh(arg0, arg1, arg2);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inVertexBuffer", reflectionSystem.GetOrCreateClass<VertexBufferHandle *>("VertexBufferHandle *")));
arguments.Add(MethodArgument("inIndexBuffer", reflectionSystem.GetOrCreateClass<IndexBufferHandle *>("IndexBufferHandle *")));
arguments.Add(MethodArgument("inBoundingSphere", reflectionSystem.GetOrCreateClass<const glm::vec<4, float> &>("const glm::vec<4, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("UploadMesh", reflectionSystem.GetOrCreateClass<Mesh *>("Mesh *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshSystem* instance = static_cast<MeshSystem*>(inInstance);
ResizableBuffer * result = instance->GetBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBuffer", reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferHandle>();
	{
		Field& currentField = currentClass->AddField(Field("myIndex", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferHandle* instance = static_cast<VertexBufferHandle*>(inInstance);
static thread_local unsigned int result = instance->GetIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIndex", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshUtils>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshUtils* instance = static_cast<MeshUtils*>(inInstance);
const List<Vertex> & arg0 = *(const List<Vertex>*)inArguments[0];
static thread_local glm::vec<4, float> result = instance->CalculateSphereBounds(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inVertices", reflectionSystem.GetOrCreateClass<const List<Vertex> &>("const List<Vertex> &")));
Method& currentMethod = currentClass->AddMethod(Method("CalculateSphereBounds", reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TextureSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorPool", 8, reflectionSystem.GetOrCreateClass<vk::DescriptorPool>("vk::DescriptorPool"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureSystem* instance = static_cast<TextureSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureSystem* instance = static_cast<TextureSystem*>(inInstance);
Texture * arg0 = (Texture*)inArguments[0];
instance->RegisterTexture_TS(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTexture", reflectionSystem.GetOrCreateClass<Texture *>("Texture *")));
Method& currentMethod = currentClass->AddMethod(Method("RegisterTexture_TS", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureSystem* instance = static_cast<TextureSystem*>(inInstance);
TextureCube * arg0 = (TextureCube*)inArguments[0];
instance->RegisterTextureCube_TS(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTexture", reflectionSystem.GetOrCreateClass<TextureCube *>("TextureCube *")));
Method& currentMethod = currentClass->AddMethod(Method("RegisterTextureCube_TS", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureSystem* instance = static_cast<TextureSystem*>(inInstance);
static thread_local vk::DescriptorSet result = instance->GetDescriptorSet();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDescriptorSet", reflectionSystem.GetOrCreateClass<vk::DescriptorSet>("vk::DescriptorSet"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureSystem* instance = static_cast<TextureSystem*>(inInstance);
static thread_local vk::DescriptorSetLayout result = instance->GetDescriptorLayout();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDescriptorLayout", reflectionSystem.GetOrCreateClass<vk::DescriptorSetLayout>("vk::DescriptorSetLayout"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryWriter>();
	{
		Field& currentField = currentClass->AddField(Field("myStream", 0, reflectionSystem.GetOrCreateClass<std::basic_ofstream<char>>("std::basic_ofstream<char>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinaryWriter* instance = static_cast<BinaryWriter*>(inInstance);
instance->Save();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Save", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinaryWriter* instance = static_cast<BinaryWriter*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Class * arg1 = (const Class*)inArguments[1];
instance->WriteClass(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
arguments.Add(MethodArgument("inClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
Method& currentMethod = currentClass->AddMethod(Method("WriteClass", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinaryWriter* instance = static_cast<BinaryWriter*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Field & arg1 = *(const Field*)inArguments[1];
instance->WriteField(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
arguments.Add(MethodArgument("inField", reflectionSystem.GetOrCreateClass<const Field &>("const Field &")));
Method& currentMethod = currentClass->AddMethod(Method("WriteField", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinaryWriter* instance = static_cast<BinaryWriter*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
instance->Write(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("Write", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinaryWriter* instance = static_cast<BinaryWriter*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
instance->Write(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("Write", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinaryWriter* instance = static_cast<BinaryWriter*>(inInstance);
void * arg0 = (void*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
instance->Write(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("Write", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Vertex>();
	{
		Field& currentField = currentClass->AddField(Field("myPosition", 0, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myColor", 12, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormal", 16, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTangents", 28, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBinormals", 40, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTexCoords", 52, reflectionSystem.GetOrCreateClass<glm::vec<2, float>[2]>("glm::vec<2, float>[2]"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Vertex* instance = static_cast<Vertex*>(inInstance);
const glm::vec<4, float> & arg0 = *(const glm::vec<4, float>*)inArguments[0];
instance->PackColor(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inColor", reflectionSystem.GetOrCreateClass<const glm::vec<4, float> &>("const glm::vec<4, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("PackColor", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Vertex* instance = static_cast<Vertex*>(inInstance);
List<vk::VertexInputBindingDescription> & result = instance->GetBindingDescriptions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBindingDescriptions", reflectionSystem.GetOrCreateClass<List<vk::VertexInputBindingDescription> &>("List<vk::VertexInputBindingDescription> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Vertex* instance = static_cast<Vertex*>(inInstance);
List<vk::VertexInputAttributeDescription> & result = instance->GetAttributeDescriptions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAttributeDescriptions", reflectionSystem.GetOrCreateClass<List<vk::VertexInputAttributeDescription> &>("List<vk::VertexInputAttributeDescription> &"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myUsedBufferSize", 8, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCurrentVertexOffset", 12, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexBuffers", 16, reflectionSystem.GetOrCreateClass<List<VertexBufferHandle *>>("List<VertexBufferHandle *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 40, reflectionSystem.GetOrCreateClass<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseVertexDataBuffer", 48, reflectionSystem.GetOrCreateClass<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseVertexData_CPURepresentation", 56, reflectionSystem.GetOrCreateClass<List<VertexBufferData>>("List<VertexBufferData>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFreeSparseIndices", 80, reflectionSystem.GetOrCreateClass<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
VulkanBuffer * arg0 = (VulkanBuffer*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
VertexBufferHandle * result = instance->UploadVertexBuffer(arg0, arg1);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inStagingBuffer", reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
arguments.Add(MethodArgument("inVertexCount", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("UploadVertexBuffer", reflectionSystem.GetOrCreateClass<VertexBufferHandle *>("VertexBufferHandle *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
const List<Vertex> & arg0 = *(const List<Vertex>*)inArguments[0];
VertexBufferHandle * result = instance->UploadVertexBuffer(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inVertices", reflectionSystem.GetOrCreateClass<const List<Vertex> &>("const List<Vertex> &")));
Method& currentMethod = currentClass->AddMethod(Method("UploadVertexBuffer", reflectionSystem.GetOrCreateClass<VertexBufferHandle *>("VertexBufferHandle *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
const VertexBufferHandle * arg0 = (const VertexBufferHandle*)inArguments[0];
instance->RemoveVertexBuffer(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBuffer", reflectionSystem.GetOrCreateClass<const VertexBufferHandle *>("const VertexBufferHandle *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveVertexBuffer", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
const ResizableBuffer * result = instance->GetGlobalVertexBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGlobalVertexBuffer", reflectionSystem.GetOrCreateClass<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
const ResizableBuffer * result = instance->GetGlobalSparseVertexDataBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGlobalSparseVertexDataBuffer", reflectionSystem.GetOrCreateClass<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
static thread_local unsigned int result = instance->GetUsedBufferSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetUsedBufferSize", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
VertexBufferHandle * arg0 = (VertexBufferHandle*)inArguments[0];
static thread_local unsigned int result = instance->GetVertexOffsetFromVertexHandle(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBuffer", reflectionSystem.GetOrCreateClass<VertexBufferHandle *>("VertexBufferHandle *")));
Method& currentMethod = currentClass->AddMethod(Method("GetVertexOffsetFromVertexHandle", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryReader>();
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 0, reflectionSystem.GetOrCreateClass<List<unsigned char>>("List<unsigned char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myReadOffset", 24, reflectionSystem.GetOrCreateClass<unsigned long long>("unsigned long long"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinaryReader* instance = static_cast<BinaryReader*>(inInstance);
std::filesystem::path & arg0 = *(std::filesystem::path*)inArguments[0];
instance->Read(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("out", reflectionSystem.GetOrCreateClass<std::filesystem::path &>("std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("Read", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinaryReader* instance = static_cast<BinaryReader*>(inInstance);
void * arg0 = (void*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
instance->Read(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("outInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("Read", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinarySerializer>();
	{
		Field& currentField = currentClass->AddField(Field("myPath", 0, reflectionSystem.GetOrCreateClass<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myOutputStream", 32, reflectionSystem.GetOrCreateClass<std::basic_ostream<char>>("std::basic_ostream<char>"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsStreamOpen", 40, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsTemporary", 41, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myInBuffer", 48, reflectionSystem.GetOrCreateClass<List<unsigned char>>("List<unsigned char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myReadOffset", 72, reflectionSystem.GetOrCreateClass<unsigned long long>("unsigned long long"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMode", 80, reflectionSystem.GetOrCreateClass<BinarySerializer::Mode>("BinarySerializer::Mode"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
static thread_local bool result = instance->IsReading();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsReading", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
static thread_local bool result = instance->IsWriting();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsWriting", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
instance->Close();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Close", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Class * arg1 = (const Class*)inArguments[1];
const bool arg2 = *(const bool*)inArguments[2];
instance->SerializeClass(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOutInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
arguments.Add(MethodArgument("inClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
arguments.Add(MethodArgument("inIsPointer", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SerializeClass", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
std::basic_string<char> & arg0 = *(std::basic_string<char>*)inArguments[0];
instance->SerializeString(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOutString", reflectionSystem.GetOrCreateClass<std::basic_string<char> &>("std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("SerializeString", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const int arg1 = *(const int*)inArguments[1];
instance->SerializeBinaryData(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOutInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<const int>("const int")));
Method& currentMethod = currentClass->AddMethod(Method("SerializeBinaryData", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ListSerializer>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ListSerializer* instance = static_cast<ListSerializer*>(inInstance);
const Class * arg0 = (const Class*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ListSerializer* instance = static_cast<ListSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Class * arg1 = (const Class*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
arguments.Add(MethodArgument("inClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
arguments.Add(MethodArgument("inSerializer", reflectionSystem.GetOrCreateClass<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StringSerializer>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StringSerializer* instance = static_cast<StringSerializer*>(inInstance);
const Class * arg0 = (const Class*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StringSerializer* instance = static_cast<StringSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Class * arg1 = (const Class*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
arguments.Add(MethodArgument("inClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
arguments.Add(MethodArgument("inSerializer", reflectionSystem.GetOrCreateClass<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshData>();
	{
		Field& currentField = currentClass->AddField(Field("myBoundingSphereModelSpace", 0, reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexIndex", 16, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexDataIndex", 20, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialIndex", 24, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferData>();
	{
		Field& currentField = currentClass->AddField(Field("myOffset", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferData>();
	{
		Field& currentField = currentClass->AddField(Field("myOffset", 0, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCount", 4, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshInstanceData>();
	{
		Field& currentField = currentClass->AddField(Field("myToWorld", 0, reflectionSystem.GetOrCreateClass<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMeshIndex", 64, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoIndex", 68, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalIndex", 72, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialIndex", 76, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PerDrawData>();
	{
		Field& currentField = currentClass->AddField(Field("myToWorld", 0, reflectionSystem.GetOrCreateClass<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoIndex", 64, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalIndex", 68, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialIndex", 72, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("padding", 76, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightData>();
	{
		Field& currentField = currentClass->AddField(Field("myColor", 0, reflectionSystem.GetOrCreateClass<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPosition", 16, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRange", 28, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIntensity", 32, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<System>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
System* instance = static_cast<System*>(inInstance);
instance->Init();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Init", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeSystem>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<WorldSystem>());
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanUtils>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanUtils* instance = static_cast<VulkanUtils*>(inInstance);
instance->CreateSamplers();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateSamplers", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanUtils* instance = static_cast<VulkanUtils*>(inInstance);
const SamplerMode arg0 = *(const SamplerMode*)inArguments[0];
static thread_local vk::Sampler result = instance->GetSampler(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMode", reflectionSystem.GetOrCreateClass<const SamplerMode>("const SamplerMode")));
Method& currentMethod = currentClass->AddMethod(Method("GetSampler", reflectionSystem.GetOrCreateClass<vk::Sampler>("vk::Sampler"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanUtils* instance = static_cast<VulkanUtils*>(inInstance);
instance->DestroySamplers();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DestroySamplers", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 8, reflectionSystem.GetOrCreateClass<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumPointLights", 16, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightSystem* instance = static_cast<PointLightSystem*>(inInstance);
const ResizableBuffer * result = instance->GetBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBuffer", reflectionSystem.GetOrCreateClass<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightSystem* instance = static_cast<PointLightSystem*>(inInstance);
TransformComponent * arg0 = (TransformComponent*)inArguments[0];
PointLightComponent * arg1 = (PointLightComponent*)inArguments[1];
instance->AddLight(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTransform", reflectionSystem.GetOrCreateClass<TransformComponent *>("TransformComponent *")));
arguments.Add(MethodArgument("inLight", reflectionSystem.GetOrCreateClass<PointLightComponent *>("PointLightComponent *")));
Method& currentMethod = currentClass->AddMethod(Method("AddLight", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Debug>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Debug* instance = static_cast<Debug*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const glm::vec<3, float> & arg1 = *(const glm::vec<3, float>*)inArguments[1];
const Color & arg2 = *(const Color*)inArguments[2];
instance->DrawLine(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inStart", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inEnd", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inColor", reflectionSystem.GetOrCreateClass<const Color &>("const Color &")));
Method& currentMethod = currentClass->AddMethod(Method("DrawLine", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Debug* instance = static_cast<Debug*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const glm::vec<3, float> & arg1 = *(const glm::vec<3, float>*)inArguments[1];
const Color & arg2 = *(const Color*)inArguments[2];
instance->DrawArrow(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inStart", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inEnd", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inColor", reflectionSystem.GetOrCreateClass<const Color &>("const Color &")));
Method& currentMethod = currentClass->AddMethod(Method("DrawArrow", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Debug* instance = static_cast<Debug*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const float arg1 = *(const float*)inArguments[1];
const Color & arg2 = *(const Color*)inArguments[2];
const int arg3 = *(const int*)inArguments[3];
instance->DrawSphere(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPosition", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inRadius", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inColor", reflectionSystem.GetOrCreateClass<const Color &>("const Color &")));
arguments.Add(MethodArgument("inSphereSubdivisions", reflectionSystem.GetOrCreateClass<const int>("const int")));
Method& currentMethod = currentClass->AddMethod(Method("DrawSphere", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Debug* instance = static_cast<Debug*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const glm::qua<float> & arg1 = *(const glm::qua<float>*)inArguments[1];
const float arg2 = *(const float*)inArguments[2];
const float arg3 = *(const float*)inArguments[3];
const Color & arg4 = *(const Color*)inArguments[4];
instance->DrawCapsule(arg0, arg1, arg2, arg3, arg4);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPosition", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inRotation", reflectionSystem.GetOrCreateClass<const glm::qua<float> &>("const glm::qua<float> &")));
arguments.Add(MethodArgument("inRadius", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inHeight", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inColor", reflectionSystem.GetOrCreateClass<const Color &>("const Color &")));
Method& currentMethod = currentClass->AddMethod(Method("DrawCapsule", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Debug* instance = static_cast<Debug*>(inInstance);
const List<Debug::DrawLineInfos> & result = instance->GetDrawInfos();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDrawInfos", reflectionSystem.GetOrCreateClass<const List<Debug::DrawLineInfos> &>("const List<Debug::DrawLineInfos> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Debug* instance = static_cast<Debug*>(inInstance);
instance->ClearDrawInfos();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ClearDrawInfos", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Debug::DrawLineInfos>();
	{
		Field& currentField = currentClass->AddField(Field("myStart", 0, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myEnd", 12, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myColor", 24, reflectionSystem.GetOrCreateClass<Color>("Color"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MathUtils>();
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<OnScopeExit>();
	{
		Field& currentField = currentClass->AddField(Field("myOnScopeExit", 0, reflectionSystem.GetOrCreateClass<std::function<void ()>>("std::function<void ()>"), false, false));
	}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadUtils>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ThreadUtils* instance = static_cast<ThreadUtils*>(inInstance);
std::thread & arg0 = *(std::thread*)inArguments[0];
const std::basic_string<wchar_t> & arg1 = *(const std::basic_string<wchar_t>*)inArguments[1];
instance->NameThread(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inThread", reflectionSystem.GetOrCreateClass<std::thread &>("std::thread &")));
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<wchar_t> &>("const std::basic_string<wchar_t> &")));
Method& currentMethod = currentClass->AddMethod(Method("NameThread", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ThreadUtils* instance = static_cast<ThreadUtils*>(inInstance);
void * arg0 = (void*)inArguments[0];
const std::basic_string<wchar_t> & arg1 = *(const std::basic_string<wchar_t>*)inArguments[1];
instance->NameThread(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inThreadHandle", reflectionSystem.GetOrCreateClass<void *>("void *")));
arguments.Add(MethodArgument("inString", reflectionSystem.GetOrCreateClass<const std::basic_string<wchar_t> &>("const std::basic_string<wchar_t> &")));
Method& currentMethod = currentClass->AddMethod(Method("NameThread", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ThreadUtils* instance = static_cast<ThreadUtils*>(inInstance);
instance->RegisterThisThreadAsMainThread();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RegisterThisThreadAsMainThread", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ThreadUtils* instance = static_cast<ThreadUtils*>(inInstance);
static thread_local bool result = instance->IsOnMainThread();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsOnMainThread", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<NvidiaAftermathTracker>();
	{
		Field& currentField = currentClass->AddField(Field("m_initialized", 0, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_mutex", 8, reflectionSystem.GetOrCreateClass<std::mutex>("std::mutex"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_shaderDebugInfo", 88, reflectionSystem.GetOrCreateClass<std::map<GFSDK_Aftermath_ShaderDebugInfoIdentifier, std::vector<unsigned char>>>("std::map<GFSDK_Aftermath_ShaderDebugInfoIdentifier, std::vector<unsigned char>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_shaderDatabase", 104, reflectionSystem.GetOrCreateClass<ShaderDatabase>("ShaderDatabase"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_markerMap", 136, reflectionSystem.GetOrCreateClass<const std::array<std::map<unsigned long long, std::basic_string<char>>, 4>>("const std::array<std::map<unsigned long long, std::basic_string<char>>, 4>"), false, true));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_AftermathFlags", 144, reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
NvidiaAftermathTracker* instance = static_cast<NvidiaAftermathTracker*>(inInstance);
instance->Initialize();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Initialize", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
NvidiaAftermathTracker* instance = static_cast<NvidiaAftermathTracker*>(inInstance);
static thread_local unsigned int result = instance->GetAftermathFlags();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAftermathFlags", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ShaderDatabase>();
	{
		Field& currentField = currentClass->AddField(Field("m_shaderBinaries", 0, reflectionSystem.GetOrCreateClass<std::map<GFSDK_Aftermath_ShaderBinaryHash, std::vector<unsigned char>>>("std::map<GFSDK_Aftermath_ShaderBinaryHash, std::vector<unsigned char>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_shaderBinariesWithDebugInfo", 16, reflectionSystem.GetOrCreateClass<std::map<GFSDK_Aftermath_ShaderDebugName, std::vector<unsigned char>>>("std::map<GFSDK_Aftermath_ShaderDebugName, std::vector<unsigned char>>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ShaderDatabase* instance = static_cast<ShaderDatabase*>(inInstance);
const GFSDK_Aftermath_ShaderBinaryHash & arg0 = *(const GFSDK_Aftermath_ShaderBinaryHash*)inArguments[0];
std::vector<unsigned char> & arg1 = *(std::vector<unsigned char>*)inArguments[1];
static thread_local bool result = instance->FindShaderBinary(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("shaderHash", reflectionSystem.GetOrCreateClass<const GFSDK_Aftermath_ShaderBinaryHash &>("const GFSDK_Aftermath_ShaderBinaryHash &")));
arguments.Add(MethodArgument("shader", reflectionSystem.GetOrCreateClass<std::vector<unsigned char> &>("std::vector<unsigned char> &")));
Method& currentMethod = currentClass->AddMethod(Method("FindShaderBinary", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ShaderDatabase* instance = static_cast<ShaderDatabase*>(inInstance);
const GFSDK_Aftermath_ShaderDebugName & arg0 = *(const GFSDK_Aftermath_ShaderDebugName*)inArguments[0];
std::vector<unsigned char> & arg1 = *(std::vector<unsigned char>*)inArguments[1];
static thread_local bool result = instance->FindShaderBinaryWithDebugData(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("shaderDebugName", reflectionSystem.GetOrCreateClass<const GFSDK_Aftermath_ShaderDebugName &>("const GFSDK_Aftermath_ShaderDebugName &")));
arguments.Add(MethodArgument("shader", reflectionSystem.GetOrCreateClass<std::vector<unsigned char> &>("std::vector<unsigned char> &")));
Method& currentMethod = currentClass->AddMethod(Method("FindShaderBinaryWithDebugData", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ResizableBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("OnBufferResized", 0, reflectionSystem.GetOrCreateClass<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasActiveUpload", 104, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasRegisteredForTick", 105, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 112, reflectionSystem.GetOrCreateClass<VulkanBuffer>("VulkanBuffer"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
VulkanBuffer * result = instance->GetBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBuffer", reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
VulkanBuffer * arg0 = (VulkanBuffer*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
unsigned int arg2 = *(unsigned int*)inArguments[2];
instance->CopyDataFromBuffer(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inStagingBuffer", reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inOffset", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("CopyDataFromBuffer", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
const void * arg0 = (const void*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
unsigned int arg2 = *(unsigned int*)inArguments[2];
instance->SetData(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inData", reflectionSystem.GetOrCreateClass<const void *>("const void *")));
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inOffset", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetData", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
const unsigned int arg0 = *(const unsigned int*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
const unsigned int arg2 = *(const unsigned int*)inArguments[2];
instance->MoveData(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourceOffset", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inDstOffset", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("MoveData", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
const unsigned int arg0 = *(const unsigned int*)inArguments[0];
instance->Resize(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRequiredSize", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("Resize", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
const unsigned int arg0 = *(const unsigned int*)inArguments[0];
instance->CheckCapacity(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRequiredSize", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("CheckCapacity", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StagingBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("myUnderlyingBuffer", 0, reflectionSystem.GetOrCreateClass<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySize", 8, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myOffset", 12, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingBuffer* instance = static_cast<StagingBuffer*>(inInstance);
const void * arg0 = (const void*)inArguments[0];
unsigned int arg1 = *(unsigned int*)inArguments[1];
instance->SetData(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inData", reflectionSystem.GetOrCreateClass<const void *>("const void *")));
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetData", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingBuffer* instance = static_cast<StagingBuffer*>(inInstance);
static thread_local unsigned int result = instance->GetSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSize", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingBuffer* instance = static_cast<StagingBuffer*>(inInstance);
static thread_local unsigned int result = instance->GetOffset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetOffset", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingBuffer* instance = static_cast<StagingBuffer*>(inInstance);
VulkanBuffer * result = instance->GetUnderlyingBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetUnderlyingBuffer", reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StagingSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myLastFrameIndex", 8, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingSystem* instance = static_cast<StagingSystem*>(inInstance);
const unsigned int arg0 = *(const unsigned int*)inArguments[0];
static thread_local StagingBuffer result = instance->GetStagingBufferWithSize(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("GetStagingBufferWithSize", reflectionSystem.GetOrCreateClass<StagingBuffer>("StagingBuffer"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingSystem* instance = static_cast<StagingSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanAllocator>();
	{
		Field& currentField = currentClass->AddField(Field("myAllocator", 0, reflectionSystem.GetOrCreateClass<VmaAllocator>("VmaAllocator"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const vk::BufferCreateInfo & arg1 = *(const vk::BufferCreateInfo*)inArguments[1];
VmaMemoryUsage arg2 = *(VmaMemoryUsage*)inArguments[2];
bool arg3 = *(bool*)inArguments[3];
VulkanBuffer * result = instance->AllocateBuffer_TS(arg0, arg1, arg2, arg3);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inCreateInfo", reflectionSystem.GetOrCreateClass<const vk::BufferCreateInfo &>("const vk::BufferCreateInfo &")));
arguments.Add(MethodArgument("inUsage", reflectionSystem.GetOrCreateClass<VmaMemoryUsage>("VmaMemoryUsage")));
arguments.Add(MethodArgument("inMappable", reflectionSystem.GetOrCreateClass<bool>("bool")));
Method& currentMethod = currentClass->AddMethod(Method("AllocateBuffer_TS", reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
VulkanBuffer * arg0 = (VulkanBuffer*)inArguments[0];
instance->DestroyBuffer_TS(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBuffer", reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
Method& currentMethod = currentClass->AddMethod(Method("DestroyBuffer_TS", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
ResizableBuffer * arg0 = (ResizableBuffer*)inArguments[0];
instance->DestroyBuffer_TS(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBuffer", reflectionSystem.GetOrCreateClass<ResizableBuffer *>("ResizableBuffer *")));
Method& currentMethod = currentClass->AddMethod(Method("DestroyBuffer_TS", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
const Delegate<void ()> & arg0 = *(const Delegate<void ()>*)inArguments[0];
instance->QueueDestroyCommand(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommand", reflectionSystem.GetOrCreateClass<const Delegate<void ()> &>("const Delegate<void ()> &")));
Method& currentMethod = currentClass->AddMethod(Method("QueueDestroyCommand", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const vk::ImageCreateInfo & arg1 = *(const vk::ImageCreateInfo*)inArguments[1];
VmaMemoryUsage arg2 = *(VmaMemoryUsage*)inArguments[2];
VulkanImage * result = instance->AllocateImage_TS(arg0, arg1, arg2);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inName", reflectionSystem.GetOrCreateClass<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inCreateInfo", reflectionSystem.GetOrCreateClass<const vk::ImageCreateInfo &>("const vk::ImageCreateInfo &")));
arguments.Add(MethodArgument("inUsage", reflectionSystem.GetOrCreateClass<VmaMemoryUsage>("VmaMemoryUsage")));
Method& currentMethod = currentClass->AddMethod(Method("AllocateImage_TS", reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
VulkanImage * arg0 = (VulkanImage*)inArguments[0];
instance->DestroyImage_TS(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inImage", reflectionSystem.GetOrCreateClass<VulkanImage *>("VulkanImage *")));
Method& currentMethod = currentClass->AddMethod(Method("DestroyImage_TS", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
VmaAllocator_T * result = instance->GetVMAAllocator();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetVMAAllocator", reflectionSystem.GetOrCreateClass<VmaAllocator_T *>("VmaAllocator_T *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("myAllocation", 0, reflectionSystem.GetOrCreateClass<VmaAllocation>("VmaAllocation"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", 8, reflectionSystem.GetOrCreateClass<vk::Buffer>("vk::Buffer"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCreateInfo", 16, reflectionSystem.GetOrCreateClass<vk::BufferCreateInfo>("vk::BufferCreateInfo"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMemoryUsage", 72, reflectionSystem.GetOrCreateClass<VmaMemoryUsage>("VmaMemoryUsage"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsMappingAllowed", 76, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPtr", 80, reflectionSystem.GetOrCreateClass<void>("void"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
unsigned int arg0 = *(unsigned int*)inArguments[0];
static thread_local vk::BufferCreateInfo result = instance->StagingCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("StagingCreateInfo", reflectionSystem.GetOrCreateClass<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
const List<Vertex> & arg0 = *(const List<Vertex>*)inArguments[0];
static thread_local vk::BufferCreateInfo result = instance->VertexBufferCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inVertices", reflectionSystem.GetOrCreateClass<const List<Vertex> &>("const List<Vertex> &")));
Method& currentMethod = currentClass->AddMethod(Method("VertexBufferCreateInfo", reflectionSystem.GetOrCreateClass<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
const List<unsigned int> & arg0 = *(const List<unsigned int>*)inArguments[0];
static thread_local vk::BufferCreateInfo result = instance->IndexBufferCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIndices", reflectionSystem.GetOrCreateClass<const List<unsigned int> &>("const List<unsigned int> &")));
Method& currentMethod = currentClass->AddMethod(Method("IndexBufferCreateInfo", reflectionSystem.GetOrCreateClass<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
unsigned int arg0 = *(unsigned int*)inArguments[0];
static thread_local vk::BufferCreateInfo result = instance->UniformBufferCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("UniformBufferCreateInfo", reflectionSystem.GetOrCreateClass<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
unsigned int arg0 = *(unsigned int*)inArguments[0];
static thread_local vk::BufferCreateInfo result = instance->StorageBufferCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("StorageBufferCreateInfo", reflectionSystem.GetOrCreateClass<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
unsigned int arg0 = *(unsigned int*)inArguments[0];
static thread_local vk::BufferCreateInfo result = instance->ResizableStorageBufferCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("ResizableStorageBufferCreateInfo", reflectionSystem.GetOrCreateClass<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
static thread_local vk::Buffer result = instance->GetAPIResource();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAPIResource", reflectionSystem.GetOrCreateClass<vk::Buffer>("vk::Buffer"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
VulkanBuffer * arg0 = (VulkanBuffer*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
unsigned int arg2 = *(unsigned int*)inArguments[2];
instance->CopyDataFromBuffer(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inStagingBuffer", reflectionSystem.GetOrCreateClass<VulkanBuffer *>("VulkanBuffer *")));
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inOffset", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("CopyDataFromBuffer", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
const void * arg0 = (const void*)inArguments[0];
const unsigned int arg1 = *(const unsigned int*)inArguments[1];
unsigned int arg2 = *(unsigned int*)inArguments[2];
instance->SetData(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inData", reflectionSystem.GetOrCreateClass<const void *>("const void *")));
arguments.Add(MethodArgument("inSize", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inOffset", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetData", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
static thread_local unsigned int result = instance->GetSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSize", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
const vk::BufferCreateInfo & result = instance->GetCreateInfo();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetCreateInfo", reflectionSystem.GetOrCreateClass<const vk::BufferCreateInfo &>("const vk::BufferCreateInfo &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
static thread_local VmaMemoryUsage result = instance->GetVmaMemoryUsage();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetVmaMemoryUsage", reflectionSystem.GetOrCreateClass<VmaMemoryUsage>("VmaMemoryUsage"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
static thread_local bool result = instance->IsMappable();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsMappable", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
static thread_local std::basic_string<char> result = instance->GetName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetName", reflectionSystem.GetOrCreateClass<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
void * result = instance->GetPtr();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPtr", reflectionSystem.GetOrCreateClass<void *>("void *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanContext>();
	{
		Field& currentField = currentClass->AddField(Field("myVulkanInstance", 0, reflectionSystem.GetOrCreateClass<vk::Instance>("vk::Instance"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDebugMessenger", 8, reflectionSystem.GetOrCreateClass<vk::DebugUtilsMessengerEXT>("vk::DebugUtilsMessengerEXT"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineCache", 16, reflectionSystem.GetOrCreateClass<vk::PipelineCache>("vk::PipelineCache"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPhysicalDevice", 24, reflectionSystem.GetOrCreateClass<VulkanPhysicalDevice>("VulkanPhysicalDevice"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDevice", 32, reflectionSystem.GetOrCreateClass<VulkanDevice>("VulkanDevice"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAllocator", 40, reflectionSystem.GetOrCreateClass<VulkanAllocator>("VulkanAllocator"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChain", 48, reflectionSystem.GetOrCreateClass<VulkanSwapChain>("VulkanSwapChain"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("markerMap", 56, reflectionSystem.GetOrCreateClass<std::array<std::map<unsigned long long, std::basic_string<char>>, 4>>("std::array<std::map<unsigned long long, std::basic_string<char>>, 4>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNvidiaAftermathDebugger", 120, reflectionSystem.GetOrCreateClass<NvidiaAftermathTracker>("NvidiaAftermathTracker"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorPool", 128, reflectionSystem.GetOrCreateClass<vk::DescriptorPool>("vk::DescriptorPool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myExtensions", 136, reflectionSystem.GetOrCreateClass<const List<const char *>>("const List<const char *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLayers", 160, reflectionSystem.GetOrCreateClass<const List<const char *>>("const List<const char *>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
vk::Instance & result = instance->GetInstance();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetInstance", reflectionSystem.GetOrCreateClass<vk::Instance &>("vk::Instance &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
VulkanPhysicalDevice & result = instance->GetPhysicalDevice();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPhysicalDevice", reflectionSystem.GetOrCreateClass<VulkanPhysicalDevice &>("VulkanPhysicalDevice &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
VulkanDevice & result = instance->GetDevice();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDevice", reflectionSystem.GetOrCreateClass<VulkanDevice &>("VulkanDevice &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
VulkanSwapChain & result = instance->GetSwapChain();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSwapChain", reflectionSystem.GetOrCreateClass<VulkanSwapChain &>("VulkanSwapChain &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
vk::PipelineCache & result = instance->GetPipelineCache();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPipelineCache", reflectionSystem.GetOrCreateClass<vk::PipelineCache &>("vk::PipelineCache &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
VulkanAllocator & result = instance->GetAllocator();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAllocator", reflectionSystem.GetOrCreateClass<VulkanAllocator &>("VulkanAllocator &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
NvidiaAftermathTracker * result = instance->GetAftermathTracker();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAftermathTracker", reflectionSystem.GetOrCreateClass<NvidiaAftermathTracker *>("NvidiaAftermathTracker *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
static thread_local glm::vec<2, float> result = instance->GetRenderResolution();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRenderResolution", reflectionSystem.GetOrCreateClass<glm::vec<2, float>>("glm::vec<2, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
instance->BeginFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("BeginFrame", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
instance->EndFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("EndFrame", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
static thread_local vk::DescriptorPool result = instance->GetDescriptorPool();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDescriptorPool", reflectionSystem.GetOrCreateClass<vk::DescriptorPool>("vk::DescriptorPool"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanDescriptorSet>();
	{
		Field& currentField = currentClass->AddField(Field("myUsesSharedLayout", 72, reflectionSystem.GetOrCreateClass<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLayout", 80, reflectionSystem.GetOrCreateClass<vk::DescriptorSetLayout>("vk::DescriptorSetLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySet", 88, reflectionSystem.GetOrCreateClass<vk::DescriptorSet>("vk::DescriptorSet"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
static thread_local vk::DescriptorSetLayout result = instance->GetLayout();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetLayout", reflectionSystem.GetOrCreateClass<vk::DescriptorSetLayout>("vk::DescriptorSetLayout"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
static thread_local vk::DescriptorSet result = instance->GetSet();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSet", reflectionSystem.GetOrCreateClass<vk::DescriptorSet>("vk::DescriptorSet"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
const VulkanBuffer * arg0 = (const VulkanBuffer*)inArguments[0];
vk::Flags<vk::ShaderStageFlagBits> arg1 = *(vk::Flags<vk::ShaderStageFlagBits>*)inArguments[1];
unsigned int arg2 = *(unsigned int*)inArguments[2];
vk::DescriptorType arg3 = *(vk::DescriptorType*)inArguments[3];
instance->BindBuffer(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBuffer", reflectionSystem.GetOrCreateClass<const VulkanBuffer *>("const VulkanBuffer *")));
arguments.Add(MethodArgument("inShaderStages", reflectionSystem.GetOrCreateClass<vk::Flags<vk::ShaderStageFlagBits>>("vk::Flags<vk::ShaderStageFlagBits>")));
arguments.Add(MethodArgument("inBindingIndex", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
arguments.Add(MethodArgument("inDescriptorType", reflectionSystem.GetOrCreateClass<vk::DescriptorType>("vk::DescriptorType")));
Method& currentMethod = currentClass->AddMethod(Method("BindBuffer", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
const ResizableBuffer * arg0 = (const ResizableBuffer*)inArguments[0];
vk::Flags<vk::ShaderStageFlagBits> arg1 = *(vk::Flags<vk::ShaderStageFlagBits>*)inArguments[1];
unsigned int arg2 = *(unsigned int*)inArguments[2];
vk::DescriptorType arg3 = *(vk::DescriptorType*)inArguments[3];
instance->BindBuffer(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBuffer", reflectionSystem.GetOrCreateClass<const ResizableBuffer *>("const ResizableBuffer *")));
arguments.Add(MethodArgument("inShaderStages", reflectionSystem.GetOrCreateClass<vk::Flags<vk::ShaderStageFlagBits>>("vk::Flags<vk::ShaderStageFlagBits>")));
arguments.Add(MethodArgument("inBindingIndex", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int")));
arguments.Add(MethodArgument("inDescriptorType", reflectionSystem.GetOrCreateClass<vk::DescriptorType>("vk::DescriptorType")));
Method& currentMethod = currentClass->AddMethod(Method("BindBuffer", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
const VulkanImage * arg0 = (const VulkanImage*)inArguments[0];
const vk::Sampler arg1 = *(const vk::Sampler*)inArguments[1];
const unsigned int arg2 = *(const unsigned int*)inArguments[2];
const vk::Flags<vk::ShaderStageFlagBits> arg3 = *(const vk::Flags<vk::ShaderStageFlagBits>*)inArguments[3];
const vk::ImageLayout arg4 = *(const vk::ImageLayout*)inArguments[4];
instance->BindImage(arg0, arg1, arg2, arg3, arg4);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inImage", reflectionSystem.GetOrCreateClass<const VulkanImage *>("const VulkanImage *")));
arguments.Add(MethodArgument("inSampler", reflectionSystem.GetOrCreateClass<const vk::Sampler>("const vk::Sampler")));
arguments.Add(MethodArgument("inBinding", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inShaderFlags", reflectionSystem.GetOrCreateClass<const vk::Flags<vk::ShaderStageFlagBits>>("const vk::Flags<vk::ShaderStageFlagBits>")));
arguments.Add(MethodArgument("inImageLayout", reflectionSystem.GetOrCreateClass<const vk::ImageLayout>("const vk::ImageLayout")));
Method& currentMethod = currentClass->AddMethod(Method("BindImage", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
instance->Build();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Build", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
instance->Rebuild();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Rebuild", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanDevice>();
	{
		Field& currentField = currentClass->AddField(Field("myPhysicalDevice", 0, reflectionSystem.GetOrCreateClass<const VulkanPhysicalDevice>("const VulkanPhysicalDevice"), false, true));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDevice", 8, reflectionSystem.GetOrCreateClass<vk::Device>("vk::Device"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGraphicsQueue", 16, reflectionSystem.GetOrCreateClass<vk::Queue>("vk::Queue"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComputeQueue", 24, reflectionSystem.GetOrCreateClass<vk::Queue>("vk::Queue"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTransferQueue", 32, reflectionSystem.GetOrCreateClass<vk::Queue>("vk::Queue"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandPoolsMutex", 40, reflectionSystem.GetOrCreateClass<std::recursive_mutex>("std::recursive_mutex"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandPools", 120, reflectionSystem.GetOrCreateClass<std::map<std::thread::id, vk::CommandPool>>("std::map<std::thread::id, vk::CommandPool>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComputeCommandPools", 136, reflectionSystem.GetOrCreateClass<std::map<std::thread::id, vk::CommandPool>>("std::map<std::thread::id, vk::CommandPool>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
vk::Device * result = instance->operator->();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("operator->", reflectionSystem.GetOrCreateClass<vk::Device *>("vk::Device *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const vk::Device * result = instance->operator->();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("operator->", reflectionSystem.GetOrCreateClass<const vk::Device *>("const vk::Device *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const vk::Queue & result = instance->GetGraphicsQueue();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGraphicsQueue", reflectionSystem.GetOrCreateClass<const vk::Queue &>("const vk::Queue &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const vk::Queue & result = instance->GetComputeQueue();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetComputeQueue", reflectionSystem.GetOrCreateClass<const vk::Queue &>("const vk::Queue &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const vk::Queue & result = instance->GetTransferQueue();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTransferQueue", reflectionSystem.GetOrCreateClass<const vk::Queue &>("const vk::Queue &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const vk::Queue & result = instance->GetPresentQueue();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPresentQueue", reflectionSystem.GetOrCreateClass<const vk::Queue &>("const vk::Queue &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
static thread_local vk::Device result = instance->GetDevice();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDevice", reflectionSystem.GetOrCreateClass<vk::Device>("vk::Device"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const std::thread::id arg0 = *(const std::thread::id*)inArguments[0];
static thread_local vk::CommandPool result = instance->GetCommandPoolForThread(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inThreadId", reflectionSystem.GetOrCreateClass<const std::thread::id>("const std::thread::id")));
Method& currentMethod = currentClass->AddMethod(Method("GetCommandPoolForThread", reflectionSystem.GetOrCreateClass<vk::CommandPool>("vk::CommandPool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const std::thread::id arg0 = *(const std::thread::id*)inArguments[0];
const bool arg1 = *(const bool*)inArguments[1];
const bool arg2 = *(const bool*)inArguments[2];
static thread_local vk::CommandBuffer result = instance->CreateCommandBufferForThread(arg0, arg1, arg2);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inThreadId", reflectionSystem.GetOrCreateClass<const std::thread::id>("const std::thread::id")));
arguments.Add(MethodArgument("inBegin", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
arguments.Add(MethodArgument("isSecondaryBuffer", reflectionSystem.GetOrCreateClass<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("CreateCommandBufferForThread", reflectionSystem.GetOrCreateClass<vk::CommandBuffer>("vk::CommandBuffer"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const std::thread::id arg0 = *(const std::thread::id*)inArguments[0];
vk::CommandBuffer arg1 = *(vk::CommandBuffer*)inArguments[1];
instance->FlushCommandBufferFromThread(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inThreadId", reflectionSystem.GetOrCreateClass<const std::thread::id>("const std::thread::id")));
arguments.Add(MethodArgument("inCommandBuffer", reflectionSystem.GetOrCreateClass<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("FlushCommandBufferFromThread", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanImGui>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImGui* instance = static_cast<VulkanImGui*>(inInstance);
instance->Start();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Start", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImGui* instance = static_cast<VulkanImGui*>(inInstance);
instance->Destroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Destroy", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImGui* instance = static_cast<VulkanImGui*>(inInstance);
instance->BeginFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("BeginFrame", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImGui* instance = static_cast<VulkanImGui*>(inInstance);
vk::CommandBuffer arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->Render(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", reflectionSystem.GetOrCreateClass<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("Render", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanImage>();
	{
		Field& currentField = currentClass->AddField(Field("myAllocation", 0, reflectionSystem.GetOrCreateClass<VmaAllocation>("VmaAllocation"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImage", 8, reflectionSystem.GetOrCreateClass<vk::Image>("vk::Image"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myView", 16, reflectionSystem.GetOrCreateClass<vk::ImageView>("vk::ImageView"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFormat", 24, reflectionSystem.GetOrCreateClass<vk::Format>("vk::Format"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySize", 28, reflectionSystem.GetOrCreateClass<glm::vec<2, float>>("glm::vec<2, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumMipLevels", 36, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumMSAASamples", 40, reflectionSystem.GetOrCreateClass<vk::SampleCountFlagBits>("vk::SampleCountFlagBits"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
vk::SampleCountFlagBits arg1 = *(vk::SampleCountFlagBits*)inArguments[1];
static thread_local vk::ImageCreateInfo result = instance->DepthCreateInfo(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inResolution", reflectionSystem.GetOrCreateClass<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
arguments.Add(MethodArgument("inSampleCount", reflectionSystem.GetOrCreateClass<vk::SampleCountFlagBits>("vk::SampleCountFlagBits")));
Method& currentMethod = currentClass->AddMethod(Method("DepthCreateInfo", reflectionSystem.GetOrCreateClass<vk::ImageCreateInfo>("vk::ImageCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
static thread_local vk::ImageCreateInfo result = instance->ShadowMapCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inResolution", reflectionSystem.GetOrCreateClass<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("ShadowMapCreateInfo", reflectionSystem.GetOrCreateClass<vk::ImageCreateInfo>("vk::ImageCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local vk::Image result = instance->GetAPIResource();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAPIResource", reflectionSystem.GetOrCreateClass<vk::Image>("vk::Image"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local vk::ImageView result = instance->GetImageView();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImageView", reflectionSystem.GetOrCreateClass<vk::ImageView>("vk::ImageView"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local vk::Format result = instance->GetFormat();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFormat", reflectionSystem.GetOrCreateClass<vk::Format>("vk::Format"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local bool result = instance->HasImageView();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("HasImageView", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
vk::ImageViewType arg0 = *(vk::ImageViewType*)inArguments[0];
instance->CreateView(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inViewType", reflectionSystem.GetOrCreateClass<vk::ImageViewType>("vk::ImageViewType")));
Method& currentMethod = currentClass->AddMethod(Method("CreateView", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
vk::ImageViewType arg0 = *(vk::ImageViewType*)inArguments[0];
vk::ImageSubresourceRange arg1 = *(vk::ImageSubresourceRange*)inArguments[1];
instance->CreateView(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inViewType", reflectionSystem.GetOrCreateClass<vk::ImageViewType>("vk::ImageViewType")));
arguments.Add(MethodArgument("inRange", reflectionSystem.GetOrCreateClass<vk::ImageSubresourceRange>("vk::ImageSubresourceRange")));
Method& currentMethod = currentClass->AddMethod(Method("CreateView", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
instance->CreateDepthView();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateDepthView", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
const glm::vec<2, float> & result = instance->GetSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSize", reflectionSystem.GetOrCreateClass<const glm::vec<2, float> &>("const glm::vec<2, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local unsigned int result = instance->GetNumMipLevels();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNumMipLevels", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local vk::SampleCountFlagBits result = instance->GetNumMSAASamples();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNumMSAASamples", reflectionSystem.GetOrCreateClass<vk::SampleCountFlagBits>("vk::SampleCountFlagBits"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanPhysicalDevice>();
	{
		Field& currentField = currentClass->AddField(Field("myDevice", 0, reflectionSystem.GetOrCreateClass<vk::PhysicalDevice>("vk::PhysicalDevice"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFeatures", 8, reflectionSystem.GetOrCreateClass<vk::PhysicalDeviceFeatures2>("vk::PhysicalDeviceFeatures2"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMemoryProperties", 248, reflectionSystem.GetOrCreateClass<vk::PhysicalDeviceMemoryProperties>("vk::PhysicalDeviceMemoryProperties"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myQueueFamilyProperties", 768, reflectionSystem.GetOrCreateClass<List<vk::QueueFamilyProperties>>("List<vk::QueueFamilyProperties>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMSAASampleCount", 792, reflectionSystem.GetOrCreateClass<vk::SampleCountFlagBits>("vk::SampleCountFlagBits"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGraphicsQueueIndex", 796, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComputeQueueIndex", 800, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTransferQueueIndex", 804, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPresentQueueIndex", 808, reflectionSystem.GetOrCreateClass<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDeviceExtensions", 816, reflectionSystem.GetOrCreateClass<List<const char *>>("List<const char *>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
const List<const char *> & result = instance->GetExtensions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetExtensions", reflectionSystem.GetOrCreateClass<const List<const char *> &>("const List<const char *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local int result = instance->GetGraphicsQueueIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGraphicsQueueIndex", reflectionSystem.GetOrCreateClass<int>("int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local int result = instance->GetComputeQueueIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetComputeQueueIndex", reflectionSystem.GetOrCreateClass<int>("int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local int result = instance->GetTransferQueueIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTransferQueueIndex", reflectionSystem.GetOrCreateClass<int>("int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local int result = instance->GetPresentQueueIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPresentQueueIndex", reflectionSystem.GetOrCreateClass<int>("int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
vk::PhysicalDevice * result = instance->operator->();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("operator->", reflectionSystem.GetOrCreateClass<vk::PhysicalDevice *>("vk::PhysicalDevice *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
const vk::PhysicalDevice * result = instance->operator->();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("operator->", reflectionSystem.GetOrCreateClass<const vk::PhysicalDevice *>("const vk::PhysicalDevice *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local const List<vk::QueueFamilyProperties> result = instance->GetQueueFamilyProperties();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetQueueFamilyProperties", reflectionSystem.GetOrCreateClass<const List<vk::QueueFamilyProperties>>("const List<vk::QueueFamilyProperties>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
const vk::PhysicalDeviceMemoryProperties & result = instance->GetMemoryProperties();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMemoryProperties", reflectionSystem.GetOrCreateClass<const vk::PhysicalDeviceMemoryProperties &>("const vk::PhysicalDeviceMemoryProperties &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local vk::SampleCountFlagBits result = instance->GetMaxMSAASamples();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMaxMSAASamples", reflectionSystem.GetOrCreateClass<vk::SampleCountFlagBits>("vk::SampleCountFlagBits"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local vk::PhysicalDevice result = instance->GetPhysicalDevice();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPhysicalDevice", reflectionSystem.GetOrCreateClass<vk::PhysicalDevice>("vk::PhysicalDevice"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanShaderIncluder>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<shaderc::CompileOptions::IncluderInterface>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanShaderIncluder* instance = static_cast<VulkanShaderIncluder*>(inInstance);
const char * arg0 = (const char*)inArguments[0];
shaderc_include_type arg1 = *(shaderc_include_type*)inArguments[1];
const char * arg2 = (const char*)inArguments[2];
unsigned long long arg3 = *(unsigned long long*)inArguments[3];
shaderc_include_result * result = instance->GetInclude(arg0, arg1, arg2, arg3);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRequestedSource", reflectionSystem.GetOrCreateClass<const char *>("const char *")));
arguments.Add(MethodArgument("inType", reflectionSystem.GetOrCreateClass<shaderc_include_type>("shaderc_include_type")));
arguments.Add(MethodArgument("inRequrestingSource", reflectionSystem.GetOrCreateClass<const char *>("const char *")));
arguments.Add(MethodArgument("inIncludeDepth", reflectionSystem.GetOrCreateClass<unsigned long long>("unsigned long long")));
Method& currentMethod = currentClass->AddMethod(Method("GetInclude", reflectionSystem.GetOrCreateClass<shaderc_include_result *>("shaderc_include_result *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanShaderIncluder* instance = static_cast<VulkanShaderIncluder*>(inInstance);
shaderc_include_result * arg0 = (shaderc_include_result*)inArguments[0];
instance->ReleaseInclude(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inData", reflectionSystem.GetOrCreateClass<shaderc_include_result *>("shaderc_include_result *")));
Method& currentMethod = currentClass->AddMethod(Method("ReleaseInclude", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanShaderIncluder* instance = static_cast<VulkanShaderIncluder*>(inInstance);
static thread_local List<std::filesystem::path> result = instance->GetIncludedFiles();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIncludedFiles", reflectionSystem.GetOrCreateClass<List<std::filesystem::path>>("List<std::filesystem::path>"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanSwapChain>();
	{
		Field& currentField = currentClass->AddField(Field("myDevice", 0, reflectionSystem.GetOrCreateClass<const VulkanDevice>("const VulkanDevice"), false, true));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWindowSurface", 8, reflectionSystem.GetOrCreateClass<vk::SurfaceKHR>("vk::SurfaceKHR"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFormat", 16, reflectionSystem.GetOrCreateClass<vk::Format>("vk::Format"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFences", 24, reflectionSystem.GetOrCreateClass<List<vk::Fence>>("List<vk::Fence>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImageAcquiredSemaphores", 48, reflectionSystem.GetOrCreateClass<List<vk::Semaphore>>("List<vk::Semaphore>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDrawCompleteSemaphores", 72, reflectionSystem.GetOrCreateClass<List<vk::Semaphore>>("List<vk::Semaphore>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameIndex", 96, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySyncIndex", 100, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChain", 104, reflectionSystem.GetOrCreateClass<vk::SwapchainKHR>("vk::SwapchainKHR"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChainWidth", 112, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChainHeight", 116, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMinImageCount", 120, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImages", 128, reflectionSystem.GetOrCreateClass<List<vk::Image>>("List<vk::Image>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImageViews", 152, reflectionSystem.GetOrCreateClass<List<vk::ImageView>>("List<vk::ImageView>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandPool", 176, reflectionSystem.GetOrCreateClass<vk::CommandPool>("vk::CommandPool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandBuffers", 184, reflectionSystem.GetOrCreateClass<List<vk::CommandBuffer>>("List<vk::CommandBuffer>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
instance->BeginFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("BeginFrame", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
instance->EndFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("EndFrame", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
const vk::CommandBuffer & result = instance->GetCommandBuffer();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetCommandBuffer", reflectionSystem.GetOrCreateClass<const vk::CommandBuffer &>("const vk::CommandBuffer &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
const vk::Image & result = instance->GetImage();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImage", reflectionSystem.GetOrCreateClass<const vk::Image &>("const vk::Image &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
const vk::SurfaceKHR & result = instance->GetSurface();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSurface", reflectionSystem.GetOrCreateClass<const vk::SurfaceKHR &>("const vk::SurfaceKHR &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
const vk::Format & result = instance->GetFormat();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFormat", reflectionSystem.GetOrCreateClass<const vk::Format &>("const vk::Format &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
static thread_local unsigned int result = instance->GetFrameIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFrameIndex", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
static thread_local unsigned int result = instance->GetSyncIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSyncIndex", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
static thread_local unsigned int result = instance->GetWidth();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWidth", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
static thread_local unsigned int result = instance->GetHeight();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHeight", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
static thread_local unsigned int result = instance->GetMinImageCount();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMinImageCount", reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
const unsigned int arg0 = *(const unsigned int*)inArguments[0];
const vk::ImageView & result = instance->GetImageView(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIndex", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("GetImageView", reflectionSystem.GetOrCreateClass<const vk::ImageView &>("const vk::ImageView &"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<WindowHandler>();
	{
		Field& currentField = currentClass->AddField(Field("myHWND", 0, reflectionSystem.GetOrCreateClass<HWND__>("HWND__"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHInstance", 8, reflectionSystem.GetOrCreateClass<HINSTANCE__>("HINSTANCE__"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WindowHandler* instance = static_cast<WindowHandler*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WindowHandler* instance = static_cast<WindowHandler*>(inInstance);
HWND__ * result = instance->GetHWND();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHWND", reflectionSystem.GetOrCreateClass<HWND__ *>("HWND__ *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WindowHandler* instance = static_cast<WindowHandler*>(inInstance);
HINSTANCE__ * result = instance->GetHInstance();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHInstance", reflectionSystem.GetOrCreateClass<HINSTANCE__ *>("HINSTANCE__ *"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<World>();
	{
		Field& currentField = currentClass->AddField(Field("myAssetRegistry", 8, reflectionSystem.GetOrCreateClass<AssetRegistry>("AssetRegistry"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySystemManager", 16, reflectionSystem.GetOrCreateClass<SystemManager<WorldSystem>>("SystemManager<WorldSystem>"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCactus", 24, reflectionSystem.GetOrCreateClass<GameObject>("GameObject"), true, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<AutoInit>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->Init();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Init", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->Update();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Update", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->Destroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Destroy", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
instance->SaveToFile(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("SaveToFile", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
instance->LoadFromFile(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", reflectionSystem.GetOrCreateClass<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("LoadFromFile", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const glm::vec<3, float> & arg1 = *(const glm::vec<3, float>*)inArguments[1];
RaycastHit & arg2 = *(RaycastHit*)inArguments[2];
const float arg3 = *(const float*)inArguments[3];
const unsigned int arg4 = *(const unsigned int*)inArguments[4];
bool arg5 = *(bool*)inArguments[5];
static thread_local bool result = instance->Raycast(arg0, arg1, arg2, arg3, arg4, arg5);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOrigin", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inDirection", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("outHit", reflectionSystem.GetOrCreateClass<RaycastHit &>("RaycastHit &")));
arguments.Add(MethodArgument("inMaxDistance", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inExcludedTags", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inIgnoreTriggers", reflectionSystem.GetOrCreateClass<bool>("bool")));
Method& currentMethod = currentClass->AddMethod(Method("Raycast", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const glm::vec<3, float> & arg1 = *(const glm::vec<3, float>*)inArguments[1];
List<RaycastHit> & arg2 = *(List<RaycastHit>*)inArguments[2];
const float arg3 = *(const float*)inArguments[3];
const unsigned int arg4 = *(const unsigned int*)inArguments[4];
bool arg5 = *(bool*)inArguments[5];
static thread_local bool result = instance->RaycastAll(arg0, arg1, arg2, arg3, arg4, arg5);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOrigin", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inDirection", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("outHits", reflectionSystem.GetOrCreateClass<List<RaycastHit> &>("List<RaycastHit> &")));
arguments.Add(MethodArgument("inMaxDistance", reflectionSystem.GetOrCreateClass<const float>("const float")));
arguments.Add(MethodArgument("inExcludedTags", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inIgnoreTriggers", reflectionSystem.GetOrCreateClass<bool>("bool")));
Method& currentMethod = currentClass->AddMethod(Method("RaycastAll", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
AssetRegistry & result = instance->GetAssetRegistry();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetRegistry", reflectionSystem.GetOrCreateClass<AssetRegistry &>("AssetRegistry &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
DirectionalLightComponent * result = instance->GetDirectionalLight();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDirectionalLight", reflectionSystem.GetOrCreateClass<DirectionalLightComponent *>("DirectionalLightComponent *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
ComponentSystem & result = instance->GetComponentSystem();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetComponentSystem", reflectionSystem.GetOrCreateClass<ComponentSystem &>("ComponentSystem &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->ToggleCactus();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ToggleCactus", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PlayerCameraControllerComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myMouseSensitivity", 16, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySpringArmChangeAmount", 20, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMinSpringArmLength", 24, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaxSpringArmLength", 28, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myPitch", 32, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myYaw", 36, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStoredPitch", 40, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStoredYaw", 44, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStoredSpringArmLength", 48, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PlayerCameraControllerComponent* instance = static_cast<PlayerCameraControllerComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PlayerComponent>();
	{
		Field& currentField = currentClass->AddField(Field("mySpeed", 16, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySprintSpeed", 20, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myJumpForce", 24, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PlayerComponent* instance = static_cast<PlayerComponent*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SpringArmComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myLength", 16, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myOffset", 20, reflectionSystem.GetOrCreateClass<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myExclusionTags", 32, reflectionSystem.GetOrCreateClass<unsigned int>("unsigned int"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitOffset", 36, reflectionSystem.GetOrCreateClass<float>("float"), false, false));
		currentField.AddMetadata(R"delim(ExposeToEditor)delim");
	}
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SpringArmComponent* instance = static_cast<SpringArmComponent*>(inInstance);
const float arg0 = *(const float*)inArguments[0];
instance->SetLength(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inLength", reflectionSystem.GetOrCreateClass<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetLength", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SpringArmComponent* instance = static_cast<SpringArmComponent*>(inInstance);
const unsigned int arg0 = *(const unsigned int*)inArguments[0];
instance->SetExclusionTags(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTags", reflectionSystem.GetOrCreateClass<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetExclusionTags", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SpringArmComponent* instance = static_cast<SpringArmComponent*>(inInstance);
static thread_local float result = instance->GetLength();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetLength", reflectionSystem.GetOrCreateClass<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SpringArmComponent* instance = static_cast<SpringArmComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
instance->SetOffset(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOffset", reflectionSystem.GetOrCreateClass<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetOffset", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SpringArmComponent* instance = static_cast<SpringArmComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Game>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Game* instance = static_cast<Game*>(inInstance);
instance->StaticTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("StaticTick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Game* instance = static_cast<Game*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PrimitiveSerializer>();
	currentClass->AddBaseClass(reflectionSystem.GetMutableClass<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PrimitiveSerializer* instance = static_cast<PrimitiveSerializer*>(inInstance);
const Class * arg0 = (const Class*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", reflectionSystem.GetOrCreateClass<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PrimitiveSerializer* instance = static_cast<PrimitiveSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Class * arg1 = (const Class*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", reflectionSystem.GetOrCreateClass<void *>("void *")));
arguments.Add(MethodArgument("inClass", reflectionSystem.GetOrCreateClass<const Class *>("const Class *")));
arguments.Add(MethodArgument("inSerializer", reflectionSystem.GetOrCreateClass<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", reflectionSystem.GetOrCreateClass<void>("void"), invoker, arguments));
}
}

        }
    }
};