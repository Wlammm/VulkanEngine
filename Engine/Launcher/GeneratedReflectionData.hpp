#pragma once
#include "Engine/Utils/StdIncludes.hpp"
#include "Engine/Reflection/Type.h"
#include "Engine/Reflection/Field.h"
#include "Engine/Reflection/ReflectionSystem.h"

// BEGIN INCLUDES FOR REFLECTED TYPES

#include "../Engine/EnginePch.h"
#include "../Engine/Math/Color.h"
#include "../Engine/Core/Random.h"
#include "../Engine/Assets/Texture.h"
#include "../Engine/Reflection/Method.h"
#include "../Engine/Math/LinearColor.h"
#include "../Engine/AssetRegistry/AssetUtils.h"
#include "../Engine/Vulkan/VulkanSwapChain.h"
#include "../Engine/Core/Console.h"
#include "../Engine/Core/UniqueID.h"
#include "../Engine/Utils/OnScopeExit.h"
#include "../Engine/Rendering/Vertex.hpp"
#include "../Engine/Core/UniquePtr.h"
#include "../Engine/EngineProperties.hpp"
#include "../Engine/Containers/List.hpp"
#include "../Engine/Containers/ContainerTypes.hpp"
#include "../Editor/Toolbar/File/SaveLoadToolbar.h"
#include "../Engine/Animation/Skeleton.h"
#include "../Engine/AssetRegistry/Asset.h"
#include "../Engine/Core/Filewatcher.h"
#include "../Engine/Utils/String.hpp"
#include "../Engine/Components/SphereColliderComponent.h"
#include "../Engine/Engine.h"
#include "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/include/memory"
#include "../Engine/Reflection/Field.h"
#include "../Engine/Delegates/Delegate.hpp"
#include "../Engine/ComponentSystem/Actor.h"
#include "../Engine/Reflection/Type.h"
#include "../Engine/ComponentSystem/Component.h"
#include "../Engine/Components/TransformComponent.h"
#include "../Engine/Core/Input.h"
#include "../Engine/Reflection/ReflectionSystem.h"
#include "../Engine/Serialization/BinarySerializer.h"
#include "../Engine/Assets/TextureCube.h"
#include "../Engine/System/System.h"
#include "../Engine/Components/StaticMeshComponent.h"
#include "../Engine/Serialization/TypeSerializers/GlmSerializer.h"
#include "../Engine/Utils/ThreadUtils.hpp"
#include "../Engine/AssetRegistry/AssetRegistry.h"
#include "../Engine/Rendering/SkyboxPipeline.h"
#include "../Engine/Components/ConvexColliderComponent.h"
#include "../Engine/ComponentSystem/Actors/PointLightActor.h"
#include "../Engine/Containers/MutexList.hpp"
#include "../Engine/Assets/JsonAsset.h"
#include "../Editor/EditorSystem/PreviousWorldsSystem.h"
#include "../Engine/Assets/Material.h"
#include "../Engine/Assets/Model.h"
#include "../Engine/Assets/Shader.h"
#include "../Engine/Delegates/MulticastDelegate.hpp"
#include "../Engine/Components/CharacterControllerComponent.h"
#include "../Engine/Rendering/RenderingPasses/IRenderPass.h"
#include "../Engine/Components/RigidbodyComponent.h"
#include "../Engine/Vulkan/GPUSceneSystem.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Components/BoxColliderComponent.h"
#include "../Engine/Components/LandscapeRenderComponent.h"
#include "../Engine/Components/CameraComponent.h"
#include "../Editor/Toolbar/Themes/EditorThemes.h"
#include "../Engine/Components/CapsuleColliderComponent.h"
#include "../Engine/Components/DirectionalLightComponent.h"
#include "../Editor/EditorPch.h"
#include "../Engine/Components/EditorCameraMovementComponent.h"
#include "../Engine/Containers/SegmentedList.h"
#include "../Engine/System/WorldSystem.h"
#include "../Engine/Components/LandscapeColliderComponent.h"
#include "../Engine/Physics/PhysicsSystem.h"
#include "../Engine/Math/Heightfield.h"
#include "../Engine/Components/MeshColliderComponent.h"
#include "../Engine/Serialization/TypeSerializers/TypeSerializer.h"
#include "../Engine/Components/PointLightComponent.h"
#include "../Engine/Coroutines/Coroutine.h"
#include "../Engine/Components/SinWaveMovementComponent.h"
#include "../Engine/ComponentSystem/Actors/DirectionalLightActor.h"
#include "../Engine/ComponentSystem/Actors/LandscapeActor.h"
#include "../Engine/ComponentSystem/Actors/PhysicsCubeActor.h"
#include "../Engine/ComponentSystem/Actors/StaticMeshActor.h"
#include "../Engine/Core/AutoInit.h"
#include "../Engine/Core/AutoInitManager.h"
#include "../Engine/Core/ThreadPool.h"
#include "../Engine/Core/Time.h"
#include "../Engine/Coroutines/Awaitable.h"
#include "../Engine/Coroutines/CoroutineManager.h"
#include "../Engine/Physics/PhysicsErrorCallback.h"
#include "../Engine/Physics/PhysicsListener.h"
#include "../Engine/Rendering/RenderingPasses/GraphicsPasses/NoDepthPass.h"
#include "../Engine/Physics/PhysicsQueryStructs.h"
#include "../Engine/Vulkan/VulkanAllocator.h"
#include "../Engine/Vulkan/VulkanBuffer.h"
#include "../Engine/Delegates/Internal/LambdaFuncCtor.hpp"
#include "../Engine/Rendering/RenderSystem.h"
#include "../Engine/Utils/MathUtils.hpp"
#include "../Engine/Vulkan/VulkanDynamicBuffer.hpp"
#include "../Engine/Vulkan/VulkanDescriptorSet.h"
#include "../Engine/Rendering/DebugPipeline.h"
#include "../Engine/Rendering/FullscreenPipeline.h"
#include "../Engine/Rendering/GDRPipeline.h"
#include "../Engine/Rendering/IndexBufferHandle.h"
#include "../Engine/Rendering/SharedWithShaders/MeshStructs.hpp"
#include "../Engine/Rendering/IndexBufferSystem.h"
#include "../Engine/Rendering/Mesh.h"
#include "../Engine/Rendering/MeshSystem.h"
#include "../Engine/Rendering/MeshUtils.h"
#include "../Engine/Rendering/VertexBufferHandle.h"
#include "../Engine/Rendering/RenderingPasses/ComputePass.h"
#include "../Engine/Rendering/RenderingPasses/ComputePasses/IndirectCullPass.h"
#include "../Engine/World/World.h"
#include "../Engine/Rendering/RenderingPasses/ComputePasses/IndirectPrePass.h"
#include "../Engine/Vulkan/Aftermath/ShaderDatabase.h"
#include "../Engine/Vulkan/Aftermath/NvidiaAftermathTracker.h"
#include "../Engine/Vulkan/VulkanContext.h"
#include "../Engine/Vulkan/VulkanDevice.h"
#include "../Engine/Vulkan/VulkanUtils.hpp"
#include "../Engine/Systems/PointLightSystem.h"
#include "../Engine/Vulkan/VulkanImage.h"
#include "../Engine/Rendering/RenderingPasses/GraphicsPass.h"
#include "../Engine/Rendering/RenderingPasses/GraphicsPasses/CopyTexturePass.h"
#include "../Engine/Rendering/RenderingPasses/GraphicsPasses/CopyToSwapchainPass.h"
#include "../Engine/Rendering/RenderingPasses/GraphicsPasses/DebugPass.h"
#include "../Engine/Rendering/RenderingPasses/GraphicsPasses/ImGuiPass.h"
#include "../Engine/Rendering/RenderingPasses/GraphicsPasses/MainPass.h"
#include "../Engine/Rendering/RenderingPasses/GraphicsPasses/SkyboxPass.h"
#include "../Engine/Rendering/RenderingPasses/TransitionPasses/TransitionImagePass.h"
#include "../Engine/Rendering/RenderingPasses/TransitionPasses/TransitionSwapchainImagePass.h"
#include "../Engine/Rendering/TextureSystem.h"
#include "../Engine/Vulkan/ResizableBuffer.h"
#include "../Engine/Rendering/VertexBufferSystem.h"
#include "../Engine/Serialization/TypeSerializers/AssetSerializer.h"
#include "../Engine/Serialization/TypeSerializers/FileTimeSerializer.h"
#include "../Engine/Serialization/TypeSerializers/ListSerializer.h"
#include "../Engine/Serialization/TypeSerializers/MapSerializer.h"
#include "../Engine/Serialization/TypeSerializers/PathSerializer.h"
#include "../Editor/ImGui/AdvancedDrawers/AssetPropertyDrawer.h"
#include "../Engine/Serialization/TypeSerializers/PrimitiveSerializer.h"
#include "../Engine/Serialization/TypeSerializers/StringSerializer.h"
#include "../Engine/Serialization/TypeSerializers/UniquePtrSerializer.h"
#include "../Game/Components/Player/PlayerCameraControllerComponent.h"
#include "../Engine/Systems/LandscapeSystem.h"
#include "../Engine/Vulkan/VulkanCommandBuffer.h"
#include "../Editor/ImGui/AdvancedDrawers/ListPropertyDrawer.h"
#include "../Engine/Delegates/Internal/FuncCtor.hpp"
#include "../Engine/Vulkan/Containers/GPUList.h"
#include "../Engine/Vulkan/Containers/GPUSparseDenseBuffer.h"
#include "../Editor/Windows/InspectorWindow.h"
#include "../Engine/Utils/ActorUtils.h"
#include "../Engine/Utils/BinaryUtils.hpp"
#include "../Engine/Utils/Debug.h"
#include "../Engine/Vulkan/Staging/StagingBuffer.h"
#include "../Engine/Vulkan/Staging/StagingSystem.h"
#include "../Game/Actors/PlayerCameraActor.h"
#include "../Engine/AssetRegistry/AssetDefines.h"
#include "../Engine/Vulkan/VulkanImGui.h"
#include "../Engine/Vulkan/VulkanPhysicalDevice.h"
#include "../Engine/Vulkan/VulkanShaderIncluder.h"
#include "../Engine/Windows/WindowHandler.h"
#include "../Engine/World/GameWorld.h"
#include "../Editor/Actors/EditorCameraActor.h"
#include "../Editor/AssetEditors/AssetEditor.h"
#include "../Editor/EditorSystem/EditorSystem.h"
#include "../Editor/Windows/HierarchyWindow.h"
#include "../Editor/AssetEditors/AssetEditorSystem.h"
#include "../Editor/AssetEditors/DefaultAssetEditor.h"
#include "../Editor/Windows/ContentBrowserWindow.h"
#include "../Editor/Editor.h"
#include "../Editor/EditorSystem/EditorToolbar.h"
#include "../Editor/EditorSystem/GuizmoSystem.h"
#include "../Editor/EditorSystem/ImGuiDemoSystem.h"
#include "../Editor/EditorSystem/SelectionSystem.h"
#include "../Editor/ImGui/ImGuiPropertyDrawer.h"
#include "../Editor/World/EditorWorld.h"
#include "../Editor/Utils/FileDialog.h"
#include "../Editor/Utils/EditorConfirmPrompt.h"
#include "../Editor/Utils/ImGuiTextureUtils.h"
#include "../Editor/Windows/EditorWindow.h"
#include "../Editor/Windows/Viewport.h"
#include "../Game/Components/Player/PlayerComponent.h"
#include "../Game/Actors/PlayerActor.h"
#include "../Game/Components/SpringArmComponent.h"
#include "../Game/Game.h"
#include "../Engine/ComponentSystem/GameObjectTag.hpp"
#include "../Engine/Core/CheckDefine.hpp"
#include "../Engine/Core/EngineDefines.hpp"
#include "../Engine/Core/SharedPtr.h"
#include "../Engine/Delegates/Internal/ConstMemberFuncCtor.hpp"
#include "../Engine/Delegates/Internal/FreeFuncCtor.hpp"
#include "../Engine/Delegates/Internal/MemberFuncCtor.hpp"
#include "../Engine/EnginePch.h"
#include "../Engine/Math/GlmUtils.hpp"
#include "../Engine/Physics/PhysXInclude.h"
#include "../Engine/System/SystemManager.hpp"
#include "../Engine/Utils/CommonMacros.h"
#include "../Engine/Utils/HashUtils.hpp"
#include "../Engine/Utils/StdIncludes.hpp"
#include "../Editor/ImGui/AssetPicker.h"
#include "../Game/GamePch.h"
#include "../Game/GameTags.h"


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
	static void RegisterReflectionData()
	{
		// Create all classes.
		{
		    ReflectionSystem::AddType<Color>("Color", typeid(Color).name());
ReflectionSystem::AddType<Random>("Random", typeid(Random).name());
ReflectionSystem::AddType<ImageData>("ImageData", typeid(ImageData).name());
ReflectionSystem::AddType<Texture>("Texture", typeid(Texture).name());
ReflectionSystem::AddType<MethodArgument>("MethodArgument", typeid(MethodArgument).name());
ReflectionSystem::AddType<Method>("Method", typeid(Method).name());
ReflectionSystem::AddType<LinearColor>("LinearColor", typeid(LinearColor).name());
ReflectionSystem::AddType<AssetUtils>("AssetUtils", typeid(AssetUtils).name());
ReflectionSystem::AddType<VulkanSwapChain>("VulkanSwapChain", typeid(VulkanSwapChain).name());
ReflectionSystem::AddType<Console>("Console", typeid(Console).name());
ReflectionSystem::AddType<UniqueID>("UniqueID", typeid(UniqueID).name());
ReflectionSystem::AddType<OnScopeExit>("OnScopeExit", typeid(OnScopeExit).name());
ReflectionSystem::AddType<Vertex>("Vertex", typeid(Vertex).name());
ReflectionSystem::AddType<IUniquePtr>("IUniquePtr", typeid(IUniquePtr).name());
ReflectionSystem::AddType<UniquePtr<SystemManager<System>>>("UniquePtr<SystemManager<System>>", typeid(UniquePtr<SystemManager<System>>).name());
ReflectionSystem::AddType<UniquePtr<ThreadPool>>("UniquePtr<ThreadPool>", typeid(UniquePtr<ThreadPool>).name());
ReflectionSystem::AddType<UniquePtr<Filewatcher>>("UniquePtr<Filewatcher>", typeid(UniquePtr<Filewatcher>).name());
ReflectionSystem::AddType<UniquePtr<WindowHandler>>("UniquePtr<WindowHandler>", typeid(UniquePtr<WindowHandler>).name());
ReflectionSystem::AddType<UniquePtr<VulkanContext>>("UniquePtr<VulkanContext>", typeid(UniquePtr<VulkanContext>).name());
ReflectionSystem::AddType<UniquePtr<VulkanPhysicalDevice>>("UniquePtr<VulkanPhysicalDevice>", typeid(UniquePtr<VulkanPhysicalDevice>).name());
ReflectionSystem::AddType<UniquePtr<VulkanDevice>>("UniquePtr<VulkanDevice>", typeid(UniquePtr<VulkanDevice>).name());
ReflectionSystem::AddType<UniquePtr<VulkanAllocator>>("UniquePtr<VulkanAllocator>", typeid(UniquePtr<VulkanAllocator>).name());
ReflectionSystem::AddType<UniquePtr<VulkanSwapChain>>("UniquePtr<VulkanSwapChain>", typeid(UniquePtr<VulkanSwapChain>).name());
ReflectionSystem::AddType<UniquePtr<NvidiaAftermathTracker>>("UniquePtr<NvidiaAftermathTracker>", typeid(UniquePtr<NvidiaAftermathTracker>).name());
ReflectionSystem::AddType<UniquePtr<SystemManager<WorldSystem>>>("UniquePtr<SystemManager<WorldSystem>>", typeid(UniquePtr<SystemManager<WorldSystem>>).name());
ReflectionSystem::AddType<UniquePtr<Actor>>("UniquePtr<Actor>", typeid(UniquePtr<Actor>).name());
ReflectionSystem::AddType<EngineProperties>("EngineProperties", typeid(EngineProperties).name());
ReflectionSystem::AddType<IList>("IList", typeid(IList).name());
ReflectionSystem::AddType<List<Skeleton::Bone>>("List<Skeleton::Bone>", typeid(List<Skeleton::Bone>).name());
ReflectionSystem::AddType<List<std::basic_string<char>>>("List<std::basic_string<char>>", typeid(List<std::basic_string<char>>).name());
ReflectionSystem::AddType<List<MethodArgument>>("List<MethodArgument>", typeid(List<MethodArgument>).name());
ReflectionSystem::AddType<List<const Type *>>("List<const Type *>", typeid(List<const Type *>).name());
ReflectionSystem::AddType<List<Field>>("List<Field>", typeid(List<Field>).name());
ReflectionSystem::AddType<List<Method>>("List<Method>", typeid(List<Method>).name());
ReflectionSystem::AddType<List<TypeTemplateArgument>>("List<TypeTemplateArgument>", typeid(List<TypeTemplateArgument>).name());
ReflectionSystem::AddType<List<unsigned char>>("List<unsigned char>", typeid(List<unsigned char>).name());
ReflectionSystem::AddType<List<Vertex>>("List<Vertex>", typeid(List<Vertex>).name());
ReflectionSystem::AddType<List<unsigned int>>("List<unsigned int>", typeid(List<unsigned int>).name());
ReflectionSystem::AddType<List<Mesh *>>("List<Mesh *>", typeid(List<Mesh *>).name());
ReflectionSystem::AddType<List<SerializationMeshData>>("List<SerializationMeshData>", typeid(List<SerializationMeshData>).name());
ReflectionSystem::AddType<List<Filewatcher::CallbackHandle>>("List<Filewatcher::CallbackHandle>", typeid(List<Filewatcher::CallbackHandle>).name());
ReflectionSystem::AddType<List<std::function<void ()>>>("List<std::function<void ()>>", typeid(List<std::function<void ()>>).name());
ReflectionSystem::AddType<List<IncludeData>>("List<IncludeData>", typeid(List<IncludeData>).name());
ReflectionSystem::AddType<List<float>>("List<float>", typeid(List<float>).name());
ReflectionSystem::AddType<List<std::shared_ptr<Material>>>("List<std::shared_ptr<Material>>", typeid(List<std::shared_ptr<Material>>).name());
ReflectionSystem::AddType<List<TransformComponent *>>("List<TransformComponent *>", typeid(List<TransformComponent *>).name());
ReflectionSystem::AddType<List<Component *>>("List<Component *>", typeid(List<Component *>).name());
ReflectionSystem::AddType<List<std::thread>>("List<std::thread>", typeid(List<std::thread>).name());
ReflectionSystem::AddType<List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>>("List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>", typeid(List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>).name());
ReflectionSystem::AddType<List<vk::Framebuffer>>("List<vk::Framebuffer>", typeid(List<vk::Framebuffer>).name());
ReflectionSystem::AddType<List<IRenderPass *>>("List<IRenderPass *>", typeid(List<IRenderPass *>).name());
ReflectionSystem::AddType<List<IndexBufferHandle *>>("List<IndexBufferHandle *>", typeid(List<IndexBufferHandle *>).name());
ReflectionSystem::AddType<List<IndexBufferData>>("List<IndexBufferData>", typeid(List<IndexBufferData>).name());
ReflectionSystem::AddType<List<const char *>>("List<const char *>", typeid(List<const char *>).name());
ReflectionSystem::AddType<List<vk::RenderingAttachmentInfo>>("List<vk::RenderingAttachmentInfo>", typeid(List<vk::RenderingAttachmentInfo>).name());
ReflectionSystem::AddType<List<vk::Format>>("List<vk::Format>", typeid(List<vk::Format>).name());
ReflectionSystem::AddType<List<VertexBufferHandle *>>("List<VertexBufferHandle *>", typeid(List<VertexBufferHandle *>).name());
ReflectionSystem::AddType<List<VertexBufferData>>("List<VertexBufferData>", typeid(List<VertexBufferData>).name());
ReflectionSystem::AddType<List<vk::QueueFamilyProperties>>("List<vk::QueueFamilyProperties>", typeid(List<vk::QueueFamilyProperties>).name());
ReflectionSystem::AddType<List<std::filesystem::path>>("List<std::filesystem::path>", typeid(List<std::filesystem::path>).name());
ReflectionSystem::AddType<List<vk::Fence>>("List<vk::Fence>", typeid(List<vk::Fence>).name());
ReflectionSystem::AddType<List<vk::Semaphore>>("List<vk::Semaphore>", typeid(List<vk::Semaphore>).name());
ReflectionSystem::AddType<List<vk::Image>>("List<vk::Image>", typeid(List<vk::Image>).name());
ReflectionSystem::AddType<List<vk::ImageView>>("List<vk::ImageView>", typeid(List<vk::ImageView>).name());
ReflectionSystem::AddType<List<vk::CommandBuffer>>("List<vk::CommandBuffer>", typeid(List<vk::CommandBuffer>).name());
ReflectionSystem::AddType<List<UniquePtr<Actor>>>("List<UniquePtr<Actor>>", typeid(List<UniquePtr<Actor>>).name());
ReflectionSystem::AddType<List<Actor *>>("List<Actor *>", typeid(List<Actor *>).name());
ReflectionSystem::AddType<List<AssetEditor *>>("List<AssetEditor *>", typeid(List<AssetEditor *>).name());
ReflectionSystem::AddType<List<EditorWindow *>>("List<EditorWindow *>", typeid(List<EditorWindow *>).name());
ReflectionSystem::AddType<List<EditorSystem *>>("List<EditorSystem *>", typeid(List<EditorSystem *>).name());
ReflectionSystem::AddType<List<const Method *>>("List<const Method *>", typeid(List<const Method *>).name());
ReflectionSystem::AddType<List<ContentBrowserItem>>("List<ContentBrowserItem>", typeid(List<ContentBrowserItem>).name());
ReflectionSystem::AddType<List<vk::DescriptorSet>>("List<vk::DescriptorSet>", typeid(List<vk::DescriptorSet>).name());
ReflectionSystem::AddType<IsCopyable<int>>("IsCopyable<int>", typeid(IsCopyable<int>).name());
ReflectionSystem::AddType<SaveLoadToolbar>("SaveLoadToolbar", typeid(SaveLoadToolbar).name());
ReflectionSystem::AddType<Skeleton>("Skeleton", typeid(Skeleton).name());
ReflectionSystem::AddType<Skeleton::Bone>("Skeleton::Bone", typeid(Skeleton::Bone).name());
ReflectionSystem::AddType<Asset>("Asset", typeid(Asset).name());
ReflectionSystem::AddType<Filewatcher>("Filewatcher", typeid(Filewatcher).name());
ReflectionSystem::AddType<Filewatcher::CallbackHandle>("Filewatcher::CallbackHandle", typeid(Filewatcher::CallbackHandle).name());
ReflectionSystem::AddType<Filewatcher::FileData>("Filewatcher::FileData", typeid(Filewatcher::FileData).name());
ReflectionSystem::AddType<String>("String", typeid(String).name());
ReflectionSystem::AddType<SphereColliderComponent>("SphereColliderComponent", typeid(SphereColliderComponent).name());
ReflectionSystem::AddType<TestClass>("TestClass", typeid(TestClass).name());
ReflectionSystem::AddType<Engine>("Engine", typeid(Engine).name());
ReflectionSystem::AddType<std::shared_ptr<World>>("std::shared_ptr<World>", typeid(std::shared_ptr<World>).name());
ReflectionSystem::AddType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>", typeid(std::shared_ptr<Texture>).name());
ReflectionSystem::AddType<std::shared_ptr<Material>>("std::shared_ptr<Material>", typeid(std::shared_ptr<Material>).name());
ReflectionSystem::AddType<std::shared_ptr<Model>>("std::shared_ptr<Model>", typeid(std::shared_ptr<Model>).name());
ReflectionSystem::AddType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>", typeid(std::shared_ptr<Shader>).name());
ReflectionSystem::AddType<std::shared_ptr<Asset>>("std::shared_ptr<Asset>", typeid(std::shared_ptr<Asset>).name());
ReflectionSystem::AddType<Field>("Field", typeid(Field).name());
ReflectionSystem::AddType<Delegate<void *(void *, const List<void *> &)>>("Delegate<void *(void *, const List<void *> &)>", typeid(Delegate<void *(void *, const List<void *> &)>).name());
ReflectionSystem::AddType<Delegate<void *()>>("Delegate<void *()>", typeid(Delegate<void *()>).name());
ReflectionSystem::AddType<Delegate<void (void *)>>("Delegate<void (void *)>", typeid(Delegate<void (void *)>).name());
ReflectionSystem::AddType<Delegate<void (IUniquePtr *)>>("Delegate<void (IUniquePtr *)>", typeid(Delegate<void (IUniquePtr *)>).name());
ReflectionSystem::AddType<Delegate<std::shared_ptr<void> ()>>("Delegate<std::shared_ptr<void> ()>", typeid(Delegate<std::shared_ptr<void> ()>).name());
ReflectionSystem::AddType<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>("Delegate<void (physx::PxPhysics *, physx::PxScene *)>", typeid(Delegate<void (physx::PxPhysics *, physx::PxScene *)>).name());
ReflectionSystem::AddType<Delegate<void ()>>("Delegate<void ()>", typeid(Delegate<void ()>).name());
ReflectionSystem::AddType<Actor>("Actor", typeid(Actor).name());
ReflectionSystem::AddType<TypeTemplateArgument>("TypeTemplateArgument", typeid(TypeTemplateArgument).name());
ReflectionSystem::AddType<Type>("Type", typeid(Type).name());
ReflectionSystem::AddType<Component>("Component", typeid(Component).name());
ReflectionSystem::AddType<TransformComponent>("TransformComponent", typeid(TransformComponent).name());
ReflectionSystem::AddType<Input>("Input", typeid(Input).name());
ReflectionSystem::AddType<ReflectionSystem>("ReflectionSystem", typeid(ReflectionSystem).name());
ReflectionSystem::AddType<BinarySerializer>("BinarySerializer", typeid(BinarySerializer).name());
ReflectionSystem::AddType<ImageCubeData>("ImageCubeData", typeid(ImageCubeData).name());
ReflectionSystem::AddType<TextureCube>("TextureCube", typeid(TextureCube).name());
ReflectionSystem::AddType<System>("System", typeid(System).name());
ReflectionSystem::AddType<StaticMeshComponent>("StaticMeshComponent", typeid(StaticMeshComponent).name());
ReflectionSystem::AddType<GlmSerializer>("GlmSerializer", typeid(GlmSerializer).name());
ReflectionSystem::AddType<ThreadUtils>("ThreadUtils", typeid(ThreadUtils).name());
ReflectionSystem::AddType<AssetRegistry>("AssetRegistry", typeid(AssetRegistry).name());
ReflectionSystem::AddType<SkyboxPipeline>("SkyboxPipeline", typeid(SkyboxPipeline).name());
ReflectionSystem::AddType<ConvexColliderComponent>("ConvexColliderComponent", typeid(ConvexColliderComponent).name());
ReflectionSystem::AddType<PointLightActor>("PointLightActor", typeid(PointLightActor).name());
ReflectionSystem::AddType<MutexList<std::filesystem::path>>("MutexList<std::filesystem::path>", typeid(MutexList<std::filesystem::path>).name());
ReflectionSystem::AddType<JsonAsset>("JsonAsset", typeid(JsonAsset).name());
ReflectionSystem::AddType<PreviousWorldsSystem>("PreviousWorldsSystem", typeid(PreviousWorldsSystem).name());
ReflectionSystem::AddType<Material>("Material", typeid(Material).name());
ReflectionSystem::AddType<SerializationMeshData>("SerializationMeshData", typeid(SerializationMeshData).name());
ReflectionSystem::AddType<Model>("Model", typeid(Model).name());
ReflectionSystem::AddType<IncludeData>("IncludeData", typeid(IncludeData).name());
ReflectionSystem::AddType<Shader>("Shader", typeid(Shader).name());
ReflectionSystem::AddType<MulticastDelegate<void ()>>("MulticastDelegate<void ()>", typeid(MulticastDelegate<void ()>).name());
ReflectionSystem::AddType<CharacterControllerComponent>("CharacterControllerComponent", typeid(CharacterControllerComponent).name());
ReflectionSystem::AddType<IRenderPass>("IRenderPass", typeid(IRenderPass).name());
ReflectionSystem::AddType<RigidbodyComponent>("RigidbodyComponent", typeid(RigidbodyComponent).name());
ReflectionSystem::AddType<GPUSceneSystem>("GPUSceneSystem", typeid(GPUSceneSystem).name());
ReflectionSystem::AddType<ColliderComponent>("ColliderComponent", typeid(ColliderComponent).name());
ReflectionSystem::AddType<BoxColliderComponent>("BoxColliderComponent", typeid(BoxColliderComponent).name());
ReflectionSystem::AddType<LandscapeRenderComponent>("LandscapeRenderComponent", typeid(LandscapeRenderComponent).name());
ReflectionSystem::AddType<CameraComponent>("CameraComponent", typeid(CameraComponent).name());
ReflectionSystem::AddType<EditorThemes>("EditorThemes", typeid(EditorThemes).name());
ReflectionSystem::AddType<CapsuleColliderComponent>("CapsuleColliderComponent", typeid(CapsuleColliderComponent).name());
ReflectionSystem::AddType<DirectionalLightComponent>("DirectionalLightComponent", typeid(DirectionalLightComponent).name());
ReflectionSystem::AddType<EditorCameraMovementComponent>("EditorCameraMovementComponent", typeid(EditorCameraMovementComponent).name());
ReflectionSystem::AddType<WorldSystem>("WorldSystem", typeid(WorldSystem).name());
ReflectionSystem::AddType<LandscapeColliderComponent>("LandscapeColliderComponent", typeid(LandscapeColliderComponent).name());
ReflectionSystem::AddType<PhysicsSystem>("PhysicsSystem", typeid(PhysicsSystem).name());
ReflectionSystem::AddType<Heightfield>("Heightfield", typeid(Heightfield).name());
ReflectionSystem::AddType<MeshColliderComponent>("MeshColliderComponent", typeid(MeshColliderComponent).name());
ReflectionSystem::AddType<TypeSerializer>("TypeSerializer", typeid(TypeSerializer).name());
ReflectionSystem::AddType<PointLightComponent>("PointLightComponent", typeid(PointLightComponent).name());
ReflectionSystem::AddType<PromiseReturnTypeImplementation<void>>("PromiseReturnTypeImplementation<void>", typeid(PromiseReturnTypeImplementation<void>).name());
ReflectionSystem::AddType<SinWaveMovementComponent>("SinWaveMovementComponent", typeid(SinWaveMovementComponent).name());
ReflectionSystem::AddType<DirectionalLightActor>("DirectionalLightActor", typeid(DirectionalLightActor).name());
ReflectionSystem::AddType<LandscapeActor>("LandscapeActor", typeid(LandscapeActor).name());
ReflectionSystem::AddType<PhysicsCubeActor>("PhysicsCubeActor", typeid(PhysicsCubeActor).name());
ReflectionSystem::AddType<StaticMeshActor>("StaticMeshActor", typeid(StaticMeshActor).name());
ReflectionSystem::AddType<AutoInit>("AutoInit", typeid(AutoInit).name());
ReflectionSystem::AddType<AutoInitManager>("AutoInitManager", typeid(AutoInitManager).name());
ReflectionSystem::AddType<ThreadPool>("ThreadPool", typeid(ThreadPool).name());
ReflectionSystem::AddType<Time>("Time", typeid(Time).name());
ReflectionSystem::AddType<Awaitable>("Awaitable", typeid(Awaitable).name());
ReflectionSystem::AddType<CoroutineManager>("CoroutineManager", typeid(CoroutineManager).name());
ReflectionSystem::AddType<PhysicsErrorCallback>("PhysicsErrorCallback", typeid(PhysicsErrorCallback).name());
ReflectionSystem::AddType<PhysicsListener>("PhysicsListener", typeid(PhysicsListener).name());
ReflectionSystem::AddType<NoDepthPass>("NoDepthPass", typeid(NoDepthPass).name());
ReflectionSystem::AddType<RaycastHit>("RaycastHit", typeid(RaycastHit).name());
ReflectionSystem::AddType<VulkanAllocator>("VulkanAllocator", typeid(VulkanAllocator).name());
ReflectionSystem::AddType<VulkanBuffer>("VulkanBuffer", typeid(VulkanBuffer).name());
ReflectionSystem::AddType<RenderSystem>("RenderSystem", typeid(RenderSystem).name());
ReflectionSystem::AddType<MathUtils>("MathUtils", typeid(MathUtils).name());
ReflectionSystem::AddType<IVulkanDynamicBuffer>("IVulkanDynamicBuffer", typeid(IVulkanDynamicBuffer).name());
ReflectionSystem::AddType<VulkanDescriptorSet>("VulkanDescriptorSet", typeid(VulkanDescriptorSet).name());
ReflectionSystem::AddType<DebugPipeline>("DebugPipeline", typeid(DebugPipeline).name());
ReflectionSystem::AddType<FullscreenPipeline>("FullscreenPipeline", typeid(FullscreenPipeline).name());
ReflectionSystem::AddType<GDRPipeline>("GDRPipeline", typeid(GDRPipeline).name());
ReflectionSystem::AddType<GDRPipeline::DirectionalLightBuffer>("GDRPipeline::DirectionalLightBuffer", typeid(GDRPipeline::DirectionalLightBuffer).name());
ReflectionSystem::AddType<GDRPipeline::FrameData>("GDRPipeline::FrameData", typeid(GDRPipeline::FrameData).name());
ReflectionSystem::AddType<IndexBufferHandle>("IndexBufferHandle", typeid(IndexBufferHandle).name());
ReflectionSystem::AddType<MeshData>("MeshData", typeid(MeshData).name());
ReflectionSystem::AddType<VertexBufferData>("VertexBufferData", typeid(VertexBufferData).name());
ReflectionSystem::AddType<IndexBufferData>("IndexBufferData", typeid(IndexBufferData).name());
ReflectionSystem::AddType<MeshInstanceData>("MeshInstanceData", typeid(MeshInstanceData).name());
ReflectionSystem::AddType<PerDrawData>("PerDrawData", typeid(PerDrawData).name());
ReflectionSystem::AddType<PointLightData>("PointLightData", typeid(PointLightData).name());
ReflectionSystem::AddType<IndexBufferSystem>("IndexBufferSystem", typeid(IndexBufferSystem).name());
ReflectionSystem::AddType<Mesh>("Mesh", typeid(Mesh).name());
ReflectionSystem::AddType<MeshSystem>("MeshSystem", typeid(MeshSystem).name());
ReflectionSystem::AddType<MeshUtils>("MeshUtils", typeid(MeshUtils).name());
ReflectionSystem::AddType<VertexBufferHandle>("VertexBufferHandle", typeid(VertexBufferHandle).name());
ReflectionSystem::AddType<ComputePass>("ComputePass", typeid(ComputePass).name());
ReflectionSystem::AddType<IndirectCullPass>("IndirectCullPass", typeid(IndirectCullPass).name());
ReflectionSystem::AddType<World>("World", typeid(World).name());
ReflectionSystem::AddType<IndirectPrePass>("IndirectPrePass", typeid(IndirectPrePass).name());
ReflectionSystem::AddType<ShaderDatabase>("ShaderDatabase", typeid(ShaderDatabase).name());
ReflectionSystem::AddType<NvidiaAftermathTracker>("NvidiaAftermathTracker", typeid(NvidiaAftermathTracker).name());
ReflectionSystem::AddType<VulkanContext>("VulkanContext", typeid(VulkanContext).name());
ReflectionSystem::AddType<VulkanDevice>("VulkanDevice", typeid(VulkanDevice).name());
ReflectionSystem::AddType<VulkanUtils>("VulkanUtils", typeid(VulkanUtils).name());
ReflectionSystem::AddType<PointLightSystem>("PointLightSystem", typeid(PointLightSystem).name());
ReflectionSystem::AddType<VulkanImage>("VulkanImage", typeid(VulkanImage).name());
ReflectionSystem::AddType<TextureAttachmentDesc>("TextureAttachmentDesc", typeid(TextureAttachmentDesc).name());
ReflectionSystem::AddType<GraphicsPass>("GraphicsPass", typeid(GraphicsPass).name());
ReflectionSystem::AddType<CopyTexturePass>("CopyTexturePass", typeid(CopyTexturePass).name());
ReflectionSystem::AddType<CopyToSwapchainPass>("CopyToSwapchainPass", typeid(CopyToSwapchainPass).name());
ReflectionSystem::AddType<DebugPass>("DebugPass", typeid(DebugPass).name());
ReflectionSystem::AddType<ImGuiPass>("ImGuiPass", typeid(ImGuiPass).name());
ReflectionSystem::AddType<MainPass>("MainPass", typeid(MainPass).name());
ReflectionSystem::AddType<SkyboxPass>("SkyboxPass", typeid(SkyboxPass).name());
ReflectionSystem::AddType<TransitionImagePass>("TransitionImagePass", typeid(TransitionImagePass).name());
ReflectionSystem::AddType<TransitionSwapchainImagePass>("TransitionSwapchainImagePass", typeid(TransitionSwapchainImagePass).name());
ReflectionSystem::AddType<TextureSystem>("TextureSystem", typeid(TextureSystem).name());
ReflectionSystem::AddType<ResizableBuffer>("ResizableBuffer", typeid(ResizableBuffer).name());
ReflectionSystem::AddType<VertexBufferSystem>("VertexBufferSystem", typeid(VertexBufferSystem).name());
ReflectionSystem::AddType<AssetSerializer>("AssetSerializer", typeid(AssetSerializer).name());
ReflectionSystem::AddType<FileTimeSerializer>("FileTimeSerializer", typeid(FileTimeSerializer).name());
ReflectionSystem::AddType<ListSerializer>("ListSerializer", typeid(ListSerializer).name());
ReflectionSystem::AddType<MapSerializer>("MapSerializer", typeid(MapSerializer).name());
ReflectionSystem::AddType<PathSerializer>("PathSerializer", typeid(PathSerializer).name());
ReflectionSystem::AddType<AssetPropertyDrawer>("AssetPropertyDrawer", typeid(AssetPropertyDrawer).name());
ReflectionSystem::AddType<PrimitiveSerializer>("PrimitiveSerializer", typeid(PrimitiveSerializer).name());
ReflectionSystem::AddType<StringSerializer>("StringSerializer", typeid(StringSerializer).name());
ReflectionSystem::AddType<WStringSerializer>("WStringSerializer", typeid(WStringSerializer).name());
ReflectionSystem::AddType<UniquePtrSerializer>("UniquePtrSerializer", typeid(UniquePtrSerializer).name());
ReflectionSystem::AddType<PlayerCameraControllerComponent>("PlayerCameraControllerComponent", typeid(PlayerCameraControllerComponent).name());
ReflectionSystem::AddType<LandscapeSystem>("LandscapeSystem", typeid(LandscapeSystem).name());
ReflectionSystem::AddType<VulkanCommandBuffer>("VulkanCommandBuffer", typeid(VulkanCommandBuffer).name());
ReflectionSystem::AddType<ListPropertyDrawer>("ListPropertyDrawer", typeid(ListPropertyDrawer).name());
ReflectionSystem::AddType<IGPUList>("IGPUList", typeid(IGPUList).name());
ReflectionSystem::AddType<GPUSparseDenseBuffer<PointLightData>>("GPUSparseDenseBuffer<PointLightData>", typeid(GPUSparseDenseBuffer<PointLightData>).name());
ReflectionSystem::AddType<InspectorWindow>("InspectorWindow", typeid(InspectorWindow).name());
ReflectionSystem::AddType<ActorUtils>("ActorUtils", typeid(ActorUtils).name());
ReflectionSystem::AddType<BinaryUtils>("BinaryUtils", typeid(BinaryUtils).name());
ReflectionSystem::AddType<Debug>("Debug", typeid(Debug).name());
ReflectionSystem::AddType<Debug::DrawLineInfos>("Debug::DrawLineInfos", typeid(Debug::DrawLineInfos).name());
ReflectionSystem::AddType<StagingBuffer>("StagingBuffer", typeid(StagingBuffer).name());
ReflectionSystem::AddType<StagingSystem>("StagingSystem", typeid(StagingSystem).name());
ReflectionSystem::AddType<PlayerCameraActor>("PlayerCameraActor", typeid(PlayerCameraActor).name());
ReflectionSystem::AddType<VulkanImGui>("VulkanImGui", typeid(VulkanImGui).name());
ReflectionSystem::AddType<VulkanPhysicalDevice>("VulkanPhysicalDevice", typeid(VulkanPhysicalDevice).name());
ReflectionSystem::AddType<VulkanShaderIncluder>("VulkanShaderIncluder", typeid(VulkanShaderIncluder).name());
ReflectionSystem::AddType<WindowHandler>("WindowHandler", typeid(WindowHandler).name());
ReflectionSystem::AddType<GameWorld>("GameWorld", typeid(GameWorld).name());
ReflectionSystem::AddType<EditorCameraActor>("EditorCameraActor", typeid(EditorCameraActor).name());
ReflectionSystem::AddType<AssetEditor>("AssetEditor", typeid(AssetEditor).name());
ReflectionSystem::AddType<EditorSystem>("EditorSystem", typeid(EditorSystem).name());
ReflectionSystem::AddType<HierarchyWindow>("HierarchyWindow", typeid(HierarchyWindow).name());
ReflectionSystem::AddType<AssetEditorSystem>("AssetEditorSystem", typeid(AssetEditorSystem).name());
ReflectionSystem::AddType<DefaultAssetEditor>("DefaultAssetEditor", typeid(DefaultAssetEditor).name());
ReflectionSystem::AddType<ContentBrowserItem>("ContentBrowserItem", typeid(ContentBrowserItem).name());
ReflectionSystem::AddType<ContentBrowserWindow>("ContentBrowserWindow", typeid(ContentBrowserWindow).name());
ReflectionSystem::AddType<Editor>("Editor", typeid(Editor).name());
ReflectionSystem::AddType<EditorToolbar>("EditorToolbar", typeid(EditorToolbar).name());
ReflectionSystem::AddType<GuizmoSystem>("GuizmoSystem", typeid(GuizmoSystem).name());
ReflectionSystem::AddType<ImGuiDemoSystem>("ImGuiDemoSystem", typeid(ImGuiDemoSystem).name());
ReflectionSystem::AddType<SelectionSystem>("SelectionSystem", typeid(SelectionSystem).name());
ReflectionSystem::AddType<AdvancedPropertyDrawer>("AdvancedPropertyDrawer", typeid(AdvancedPropertyDrawer).name());
ReflectionSystem::AddType<ImGuiPropertyDrawer>("ImGuiPropertyDrawer", typeid(ImGuiPropertyDrawer).name());
ReflectionSystem::AddType<EditorWorld>("EditorWorld", typeid(EditorWorld).name());
ReflectionSystem::AddType<FileDialog>("FileDialog", typeid(FileDialog).name());
ReflectionSystem::AddType<EditorConfirmPrompt>("EditorConfirmPrompt", typeid(EditorConfirmPrompt).name());
ReflectionSystem::AddType<ImGuiTextureUtils>("ImGuiTextureUtils", typeid(ImGuiTextureUtils).name());
ReflectionSystem::AddType<EditorWindow>("EditorWindow", typeid(EditorWindow).name());
ReflectionSystem::AddType<Viewport>("Viewport", typeid(Viewport).name());
ReflectionSystem::AddType<PlayerComponent>("PlayerComponent", typeid(PlayerComponent).name());
ReflectionSystem::AddType<PlayerActor>("PlayerActor", typeid(PlayerActor).name());
ReflectionSystem::AddType<SpringArmComponent>("SpringArmComponent", typeid(SpringArmComponent).name());
ReflectionSystem::AddType<Game>("Game", typeid(Game).name());

        }
        
        // Add all fields & add base classes.
        {
            { 
	Type* currentClass = ReflectionSystem::GetMutableType<Color>();
	{
		Field& currentField = currentClass->AddField(Field("myR", -1, ReflectionSystem::GetOrCreateType<unsigned char>("unsigned char"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myG", -1, ReflectionSystem::GetOrCreateType<unsigned char>("unsigned char"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myB", -1, ReflectionSystem::GetOrCreateType<unsigned char>("unsigned char"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myA", -1, ReflectionSystem::GetOrCreateType<unsigned char>("unsigned char"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local LinearColor result = instance->ToLinearColor();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ToLinearColor", ReflectionSystem::GetOrCreateType<LinearColor>("LinearColor"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local std::basic_string<char> result = instance->ToString();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ToString", ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local Color result = instance->White();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("White", ReflectionSystem::GetOrCreateType<Color>("Color"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local Color result = instance->Black();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Black", ReflectionSystem::GetOrCreateType<Color>("Color"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local Color result = instance->Red();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Red", ReflectionSystem::GetOrCreateType<Color>("Color"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local Color result = instance->Green();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Green", ReflectionSystem::GetOrCreateType<Color>("Color"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Color* instance = static_cast<Color*>(inInstance);
static thread_local Color result = instance->Blue();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Blue", ReflectionSystem::GetOrCreateType<Color>("Color"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Random>();
	{
		Field& currentField = currentClass->AddField(Field("myEngine", -1, ReflectionSystem::GetOrCreateType<std::mersenne_twister_engine<unsigned int, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>>("std::mersenne_twister_engine<unsigned int, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ImageData>();
	{
		Field& currentField = currentClass->AddField(Field("myWidth", offsetof(ImageData, myWidth), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myHeight", offsetof(ImageData, myHeight), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myChannels", offsetof(ImageData, myChannels), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumMipLevels", offsetof(ImageData, myNumMipLevels), ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myPixelData", offsetof(ImageData, myPixelData), ReflectionSystem::GetOrCreateType<List<unsigned char>>("List<unsigned char>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Texture>();
	{
		Field& currentField = currentClass->AddField(Field("myImage", offsetof(Texture, myImage), ReflectionSystem::GetOrCreateType<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindlessIndex", offsetof(Texture, myBindlessIndex), ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImageData", offsetof(Texture, myImageData), ReflectionSystem::GetOrCreateType<ImageData>("ImageData"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
static thread_local List<std::basic_string<char>> result = instance->GetAssetExtensions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetExtensions", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
instance->LoadPropertiesFromSource();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LoadPropertiesFromSource", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
instance->PostPropertiesSerialized();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("PostPropertiesSerialized", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
static thread_local bool result = instance->IsExternalAsset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsExternalAsset", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
VulkanImage * result = instance->GetImage();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImage", ReflectionSystem::GetOrCreateType<VulkanImage *>("VulkanImage *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
static thread_local vk::ImageView result = instance->GetImageView();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImageView", ReflectionSystem::GetOrCreateType<vk::ImageView>("vk::ImageView"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
static thread_local unsigned int result = instance->GetBindlessIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBindlessIndex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Texture* instance = static_cast<Texture*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->GenerateMipLevels(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("GenerateMipLevels", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MethodArgument>();
	{
		Field& currentField = currentClass->AddField(Field("myArgumentName", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myArgumentType", -1, ReflectionSystem::GetOrCreateType<const Type>("const Type"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MethodArgument* instance = static_cast<MethodArgument*>(inInstance);
const std::basic_string<char> & result = instance->GetArgumentName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetArgumentName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MethodArgument* instance = static_cast<MethodArgument*>(inInstance);
const Type * result = instance->GetArgumentType();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetArgumentType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Method>();
	{
		Field& currentField = currentClass->AddField(Field("myMethodName", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myReturnType", -1, ReflectionSystem::GetOrCreateType<const Type>("const Type"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myArguments", -1, ReflectionSystem::GetOrCreateType<List<MethodArgument>>("List<MethodArgument>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMetadata", -1, ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myInvoker", -1, ReflectionSystem::GetOrCreateType<Delegate<void *(void *, const List<void *> &)>>("Delegate<void *(void *, const List<void *> &)>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Method* instance = static_cast<Method*>(inInstance);
const std::basic_string<char> & result = instance->GetMethodName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMethodName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Method* instance = static_cast<Method*>(inInstance);
const List<std::basic_string<char>> & result = instance->GetMetadata();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMetadata", ReflectionSystem::GetOrCreateType<const List<std::basic_string<char>> &>("const List<std::basic_string<char>> &"), invoker, arguments));
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
arguments.Add(MethodArgument("inMetadata", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("HasMetadata", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
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
arguments.Add(MethodArgument("inMetadata", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetMetadataArgs", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<LinearColor>();
	{
		Field& currentField = currentClass->AddField(Field("myR", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myG", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myB", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myA", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LinearColor* instance = static_cast<LinearColor*>(inInstance);
static thread_local Color result = instance->ToColor();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ToColor", ReflectionSystem::GetOrCreateType<Color>("Color"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<AssetUtils>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetUtils* instance = static_cast<AssetUtils*>(inInstance);
instance->Start();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Start", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetUtils* instance = static_cast<AssetUtils*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local std::filesystem::path result = instance->GetSourcePathFromAssetName(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inAssetName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetSourcePathFromAssetName", ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetUtils* instance = static_cast<AssetUtils*>(inInstance);
const List<std::basic_string<char>> & arg0 = *(const List<std::basic_string<char>>*)inArguments[0];
static thread_local List<std::filesystem::path> result = instance->GetAllSourcePathsWithExtensions(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inExtensions", ReflectionSystem::GetOrCreateType<const List<std::basic_string<char>> &>("const List<std::basic_string<char>> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetAllSourcePathsWithExtensions", ReflectionSystem::GetOrCreateType<List<std::filesystem::path>>("List<std::filesystem::path>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetUtils* instance = static_cast<AssetUtils*>(inInstance);
const List<const Type *> & result = instance->GetAllInternalAssetTypes();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAllInternalAssetTypes", ReflectionSystem::GetOrCreateType<const List<const Type *> &>("const List<const Type *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetUtils* instance = static_cast<AssetUtils*>(inInstance);
const List<const Type *> & result = instance->GetAllExternalAssetTypes();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAllExternalAssetTypes", ReflectionSystem::GetOrCreateType<const List<const Type *> &>("const List<const Type *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetUtils* instance = static_cast<AssetUtils*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Type * result = instance->GetAssetTypeFromFileExtension(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFileExtension", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetAssetTypeFromFileExtension", ReflectionSystem::GetOrCreateType<const Type *>("const Type *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetUtils* instance = static_cast<AssetUtils*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local std::basic_string<char> result = instance->GetExtensionForInternalType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("GetExtensionForInternalType", ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetUtils* instance = static_cast<AssetUtils*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const std::filesystem::path & arg1 = *(const std::filesystem::path*)inArguments[1];
instance->RenameAsset(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCurrentPath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inNewPath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("RenameAsset", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanSwapChain>();
	{
		Field& currentField = currentClass->AddField(Field("myDevice", -1, ReflectionSystem::GetOrCreateType<const VulkanDevice>("const VulkanDevice"), false, true));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWindowSurface", -1, ReflectionSystem::GetOrCreateType<vk::SurfaceKHR>("vk::SurfaceKHR"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFormat", -1, ReflectionSystem::GetOrCreateType<vk::Format>("vk::Format"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFences", -1, ReflectionSystem::GetOrCreateType<List<vk::Fence>>("List<vk::Fence>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImageAcquiredSemaphores", -1, ReflectionSystem::GetOrCreateType<List<vk::Semaphore>>("List<vk::Semaphore>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDrawCompleteSemaphores", -1, ReflectionSystem::GetOrCreateType<List<vk::Semaphore>>("List<vk::Semaphore>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySyncIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChain", -1, ReflectionSystem::GetOrCreateType<vk::SwapchainKHR>("vk::SwapchainKHR"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChainWidth", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChainHeight", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMinImageCount", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImages", -1, ReflectionSystem::GetOrCreateType<List<vk::Image>>("List<vk::Image>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImageViews", -1, ReflectionSystem::GetOrCreateType<List<vk::ImageView>>("List<vk::ImageView>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandPool", -1, ReflectionSystem::GetOrCreateType<vk::CommandPool>("vk::CommandPool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandBuffers", -1, ReflectionSystem::GetOrCreateType<List<vk::CommandBuffer>>("List<vk::CommandBuffer>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
instance->BeginFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("BeginFrame", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
instance->EndFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("EndFrame", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
const vk::CommandBuffer & result = instance->GetCommandBuffer();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetCommandBuffer", ReflectionSystem::GetOrCreateType<const vk::CommandBuffer &>("const vk::CommandBuffer &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
const vk::Image & result = instance->GetImage();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImage", ReflectionSystem::GetOrCreateType<const vk::Image &>("const vk::Image &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
const vk::SurfaceKHR & result = instance->GetSurface();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSurface", ReflectionSystem::GetOrCreateType<const vk::SurfaceKHR &>("const vk::SurfaceKHR &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
const vk::Format & result = instance->GetFormat();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFormat", ReflectionSystem::GetOrCreateType<const vk::Format &>("const vk::Format &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
static thread_local unsigned int result = instance->GetFrameIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFrameIndex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
static thread_local unsigned int result = instance->GetSyncIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSyncIndex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
static thread_local unsigned int result = instance->GetWidth();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWidth", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
static thread_local unsigned int result = instance->GetHeight();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHeight", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
static thread_local unsigned int result = instance->GetMinImageCount();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMinImageCount", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanSwapChain* instance = static_cast<VulkanSwapChain*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
const vk::ImageView & result = instance->GetImageView(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIndex", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("GetImageView", ReflectionSystem::GetOrCreateType<const vk::ImageView &>("const vk::ImageView &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Console>();
	{
		Field& currentField = currentClass->AddField(Field("myConsoleHandle", -1, ReflectionSystem::GetOrCreateType<void>("void"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLogToFileEnabled", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLogToFileStream", -1, ReflectionSystem::GetOrCreateType<std::basic_ofstream<char>>("std::basic_ofstream<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCoutBuffer", -1, ReflectionSystem::GetOrCreateType<std::basic_streambuf<char>>("std::basic_streambuf<char>"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Console* instance = static_cast<Console*>(inInstance);
Console & result = instance->Get();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Get", ReflectionSystem::GetOrCreateType<Console &>("Console &"), invoker, arguments));
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
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("Log", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("LogWarning", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("LogError", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniqueID>();
	{
		Field& currentField = currentClass->AddField(Field("myA", offsetof(UniqueID, myA), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myB", offsetof(UniqueID, myB), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myC", offsetof(UniqueID, myC), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myD", offsetof(UniqueID, myD), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
UniqueID* instance = static_cast<UniqueID*>(inInstance);
static thread_local UniqueID result = instance->GenerateUniqueID();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GenerateUniqueID", ReflectionSystem::GetOrCreateType<UniqueID>("UniqueID"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
UniqueID* instance = static_cast<UniqueID*>(inInstance);
const UniqueID & arg0 = *(const UniqueID*)inArguments[0];
static thread_local bool result = instance->operator==(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<const UniqueID &>("const UniqueID &")));
Method& currentMethod = currentClass->AddMethod(Method("operator==", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<OnScopeExit>();
	{
		Field& currentField = currentClass->AddField(Field("myOnScopeExit", -1, ReflectionSystem::GetOrCreateType<std::function<void ()>>("std::function<void ()>"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Vertex>();
	{
		Field& currentField = currentClass->AddField(Field("myPosition", offsetof(Vertex, myPosition), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myColor", offsetof(Vertex, myColor), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormal", offsetof(Vertex, myNormal), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myTangents", offsetof(Vertex, myTangents), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myBinormals", offsetof(Vertex, myBinormals), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myTexCoords", offsetof(Vertex, myTexCoords), ReflectionSystem::GetOrCreateType<glm::vec<2, float>[2]>("glm::vec<2, float>[2]"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
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
arguments.Add(MethodArgument("inColor", ReflectionSystem::GetOrCreateType<const glm::vec<4, float> &>("const glm::vec<4, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("PackColor", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Vertex* instance = static_cast<Vertex*>(inInstance);
List<vk::VertexInputBindingDescription> & result = instance->GetBindingDescriptions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBindingDescriptions", ReflectionSystem::GetOrCreateType<List<vk::VertexInputBindingDescription> &>("List<vk::VertexInputBindingDescription> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Vertex* instance = static_cast<Vertex*>(inInstance);
List<vk::VertexInputAttributeDescription> & result = instance->GetAttributeDescriptions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAttributeDescriptions", ReflectionSystem::GetOrCreateType<List<vk::VertexInputAttributeDescription> &>("List<vk::VertexInputAttributeDescription> &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IUniquePtr>();
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<SystemManager<System>>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<SystemManager<System>>("SystemManager<System>"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<ThreadPool>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<ThreadPool>("ThreadPool"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<Filewatcher>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Filewatcher>("Filewatcher"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<WindowHandler>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<WindowHandler>("WindowHandler"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<VulkanContext>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<VulkanContext>("VulkanContext"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<VulkanPhysicalDevice>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<VulkanPhysicalDevice>("VulkanPhysicalDevice"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<VulkanDevice>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<VulkanDevice>("VulkanDevice"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<VulkanAllocator>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<VulkanAllocator>("VulkanAllocator"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<VulkanSwapChain>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<VulkanSwapChain>("VulkanSwapChain"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<NvidiaAftermathTracker>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<NvidiaAftermathTracker>("NvidiaAftermathTracker"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<SystemManager<WorldSystem>>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<SystemManager<WorldSystem>>("SystemManager<WorldSystem>"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtr<Actor>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IUniquePtr>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Actor>("Actor"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<EngineProperties>();
	{
		Field& currentField = currentClass->AddField(Field("Title", -1, ReflectionSystem::GetOrCreateType<std::basic_string<wchar_t>>("std::basic_string<wchar_t>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowWidth", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowHeight", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowX", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("WindowY", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("StartupArguments", -1, ReflectionSystem::GetOrCreateType<std::unordered_set<std::basic_string<char>>>("std::unordered_set<std::basic_string<char>>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EngineProperties* instance = static_cast<EngineProperties*>(inInstance);
int& arg0 = *(int*)inArguments[0];
char ** arg1 = (char **)inArguments[1];
instance->AddStartupArguments(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inArgC", ReflectionSystem::GetOrCreateType<int>("int")));
arguments.Add(MethodArgument("inArgv", ReflectionSystem::GetOrCreateType<char **>("char **")));
Method& currentMethod = currentClass->AddMethod(Method("AddStartupArguments", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inArg", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("AddStartupArgument", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inArg", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("HasStartupArgument", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IList>();
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<Skeleton::Bone>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Skeleton::Bone>("Skeleton::Bone"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<std::basic_string<char>>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<MethodArgument>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<MethodArgument>("MethodArgument"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<const Type *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<const Type>("const Type"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<Field>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Field>("Field"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<Method>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Method>("Method"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<TypeTemplateArgument>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<TypeTemplateArgument>("TypeTemplateArgument"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<unsigned char>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<unsigned char>("unsigned char"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<Vertex>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Vertex>("Vertex"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<unsigned int>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<Mesh *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Mesh>("Mesh"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<SerializationMeshData>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<SerializationMeshData>("SerializationMeshData"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<Filewatcher::CallbackHandle>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Filewatcher::CallbackHandle>("Filewatcher::CallbackHandle"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<std::function<void ()>>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<std::function<void ()>>("std::function<void ()>"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<IncludeData>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<IncludeData>("IncludeData"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<float>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<float>("float"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<std::shared_ptr<Material>>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<std::shared_ptr<Material>>("std::shared_ptr<Material>"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<TransformComponent *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<TransformComponent>("TransformComponent"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<Component *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Component>("Component"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<std::thread>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<std::thread>("std::thread"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>("Delegate<void (physx::PxPhysics *, physx::PxScene *)>"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<vk::Framebuffer>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<vk::Framebuffer>("vk::Framebuffer"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<IRenderPass *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<IRenderPass>("IRenderPass"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<IndexBufferHandle *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<IndexBufferHandle>("IndexBufferHandle"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<IndexBufferData>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<IndexBufferData>("IndexBufferData"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<const char *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<const char>("const char"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<vk::RenderingAttachmentInfo>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<vk::RenderingAttachmentInfo>("vk::RenderingAttachmentInfo"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<vk::Format>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<vk::Format>("vk::Format"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<VertexBufferHandle *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<VertexBufferHandle>("VertexBufferHandle"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<VertexBufferData>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<VertexBufferData>("VertexBufferData"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<vk::QueueFamilyProperties>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<vk::QueueFamilyProperties>("vk::QueueFamilyProperties"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<std::filesystem::path>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<vk::Fence>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<vk::Fence>("vk::Fence"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<vk::Semaphore>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<vk::Semaphore>("vk::Semaphore"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<vk::Image>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<vk::Image>("vk::Image"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<vk::ImageView>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<vk::ImageView>("vk::ImageView"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<vk::CommandBuffer>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<UniquePtr<Actor>>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<UniquePtr<Actor>>("UniquePtr<Actor>"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<Actor *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Actor>("Actor"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<AssetEditor *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<AssetEditor>("AssetEditor"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<EditorWindow *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<EditorWindow>("EditorWindow"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<EditorSystem *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<EditorSystem>("EditorSystem"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<const Method *>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<const Method>("const Method"), true, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<ContentBrowserItem>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<ContentBrowserItem>("ContentBrowserItem"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<List<vk::DescriptorSet>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<vk::DescriptorSet>("vk::DescriptorSet"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IsCopyable<int>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<int>("int"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<SaveLoadToolbar>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SaveLoadToolbar* instance = static_cast<SaveLoadToolbar*>(inInstance);
instance->SaveWorld();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SaveWorld", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
currentMethod.AddMetadata(R"delim(EditorMenuItem("File/Save World"))delim");
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SaveLoadToolbar* instance = static_cast<SaveLoadToolbar*>(inInstance);
instance->LoadWorld();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LoadWorld", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
currentMethod.AddMetadata(R"delim(EditorMenuItem("File/Load World"))delim");
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Skeleton>();
	{
		Field& currentField = currentClass->AddField(Field("myJoints", -1, ReflectionSystem::GetOrCreateType<List<Skeleton::Bone>>("List<Skeleton::Bone>"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Skeleton::Bone>();
	{
		Field& currentField = currentClass->AddField(Field("myBindPosition", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindRotation", -1, ReflectionSystem::GetOrCreateType<glm::qua<float>>("glm::qua<float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindScale", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexToParent", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Asset>();
	{
		Field& currentField = currentClass->AddField(Field("myAssetPath", offsetof(Asset, myAssetPath), ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(HideInEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySourceLastModifiedTime", offsetof(Asset, mySourceLastModifiedTime), ReflectionSystem::GetOrCreateType<std::chrono::time_point<std::filesystem::_File_time_clock>>("std::chrono::time_point<std::filesystem::_File_time_clock>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(HideInEditor)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myAssetRegistry", offsetof(Asset, myAssetRegistry), ReflectionSystem::GetOrCreateType<AssetRegistry>("AssetRegistry"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myType", offsetof(Asset, myType), ReflectionSystem::GetOrCreateType<const Type>("const Type"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsValid", offsetof(Asset, myIsValid), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<std::enable_shared_from_this<Asset>>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
static thread_local List<std::basic_string<char>> result = instance->GetAssetExtensions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetExtensions", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
static thread_local bool result = instance->IsExternalAsset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsExternalAsset", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
instance->PostPropertiesSerialized();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("PostPropertiesSerialized", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
instance->LoadPropertiesFromSource();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LoadPropertiesFromSource", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
static thread_local bool result = instance->IsCacheValid();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsCacheValid", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
instance->DoFirstTimeAssetInitialization(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inAssetPath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("DoFirstTimeAssetInitialization", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
instance->SetAssetPath(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("SetAssetPath", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
const std::filesystem::path & result = instance->GetSourcePath();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
static thread_local std::basic_string<char> result = instance->GetName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetName", ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
const std::chrono::time_point<std::filesystem::_File_time_clock> & result = instance->GetSourceLastModifiedTime();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSourceLastModifiedTime", ReflectionSystem::GetOrCreateType<const std::chrono::time_point<std::filesystem::_File_time_clock> &>("const std::chrono::time_point<std::filesystem::_File_time_clock> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
static thread_local bool result = instance->IsValid();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsValid", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
const bool& arg0 = *(const bool*)inArguments[0];
instance->SetIsValid(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIsValid", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetIsValid", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
AssetRegistry * arg0 = (AssetRegistry*)inArguments[0];
instance->SetAssetRegistry(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inAssetRegistry", ReflectionSystem::GetOrCreateType<AssetRegistry *>("AssetRegistry *")));
Method& currentMethod = currentClass->AddMethod(Method("SetAssetRegistry", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
instance->SetType(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SetType", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
const Type * result = instance->GetType();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Asset* instance = static_cast<Asset*>(inInstance);
instance->ResaveAsset();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ResaveAsset", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Filewatcher>();
	{
		Field& currentField = currentClass->AddField(Field("myThread", -1, ReflectionSystem::GetOrCreateType<std::thread>("std::thread"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLock", -1, ReflectionSystem::GetOrCreateType<std::mutex>("std::mutex"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShouldRun", -1, ReflectionSystem::GetOrCreateType<std::atomic<bool>>("std::atomic<bool>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFilesToWatch", -1, ReflectionSystem::GetOrCreateType<std::unordered_map<std::filesystem::path, Filewatcher::FileData>>("std::unordered_map<std::filesystem::path, Filewatcher::FileData>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myModifiedPaths", -1, ReflectionSystem::GetOrCreateType<std::set<Filewatcher::FileData *>>("std::set<Filewatcher::FileData *>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Filewatcher* instance = static_cast<Filewatcher*>(inInstance);
const bool& arg0 = *(const bool*)inArguments[0];
instance->SetShouldRun(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inState", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetShouldRun", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Filewatcher* instance = static_cast<Filewatcher*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const std::function<void ()>& arg1 = *(const std::function<void ()>*)inArguments[1];
static thread_local Filewatcher::CallbackHandle result = instance->InsertWatch_TS(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inCallback", ReflectionSystem::GetOrCreateType<const std::function<void ()>>("const std::function<void ()>")));
Method& currentMethod = currentClass->AddMethod(Method("InsertWatch_TS", ReflectionSystem::GetOrCreateType<Filewatcher::CallbackHandle>("Filewatcher::CallbackHandle"), invoker, arguments));
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
arguments.Add(MethodArgument("inPath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inHandle", ReflectionSystem::GetOrCreateType<const Filewatcher::CallbackHandle &>("const Filewatcher::CallbackHandle &")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveWatch_TS", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Filewatcher* instance = static_cast<Filewatcher*>(inInstance);
instance->FlushChanges();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("FlushChanges", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Filewatcher::CallbackHandle>();
	{
		Field& currentField = currentClass->AddField(Field("myID", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Filewatcher::CallbackHandle* instance = static_cast<Filewatcher::CallbackHandle*>(inInstance);
static thread_local bool result = instance->IsValid();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsValid", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
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
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<const Filewatcher::CallbackHandle &>("const Filewatcher::CallbackHandle &")));
Method& currentMethod = currentClass->AddMethod(Method("operator==", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Filewatcher::FileData>();
	{
		Field& currentField = currentClass->AddField(Field("myCallbackIDIndexList", -1, ReflectionSystem::GetOrCreateType<List<Filewatcher::CallbackHandle>>("List<Filewatcher::CallbackHandle>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCallbacks", -1, ReflectionSystem::GetOrCreateType<List<std::function<void ()>>>("List<std::function<void ()>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLastModifiedTime", -1, ReflectionSystem::GetOrCreateType<std::chrono::time_point<std::filesystem::_File_time_clock>>("std::chrono::time_point<std::filesystem::_File_time_clock>"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<String>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
const std::basic_string<wchar_t> & arg0 = *(const std::basic_string<wchar_t>*)inArguments[0];
static thread_local std::basic_string<char> result = instance->ToString(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<const std::basic_string<wchar_t> &>("const std::basic_string<wchar_t> &")));
Method& currentMethod = currentClass->AddMethod(Method("ToString", ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
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
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inToSearchFor", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("Contains", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
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
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inToSearchFor", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inNewPart", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("Replace", ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
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
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("ToWString", ReflectionSystem::GetOrCreateType<std::basic_string<wchar_t>>("std::basic_string<wchar_t>"), invoker, arguments));
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
arguments.Add(MethodArgument("inA", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inB", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("EqualsIgnoreCase", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
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
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<std::basic_string<char> &>("std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("ToLower", ReflectionSystem::GetOrCreateType<std::basic_string<char> &>("std::basic_string<char> &"), invoker, arguments));
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
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("ToLowerCopy", ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
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
arguments.Add(MethodArgument("string", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("delimiters", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("SplitString", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
String* instance = static_cast<String*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const char& arg1 = *(const char*)inArguments[1];
static thread_local List<std::basic_string<char>> result = instance->SplitString(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("string", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("delimiter", ReflectionSystem::GetOrCreateType<const char>("const char")));
Method& currentMethod = currentClass->AddMethod(Method("SplitString", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<SphereColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myRadius", offsetof(SphereColliderComponent, myRadius), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SphereColliderComponent* instance = static_cast<SphereColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SphereColliderComponent* instance = static_cast<SphereColliderComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetRadius(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRadius", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetRadius", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SphereColliderComponent* instance = static_cast<SphereColliderComponent*>(inInstance);
instance->OnScaleChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnScaleChanged", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<TestClass>();
	{
		Field& currentField = currentClass->AddField(Field("SomeFloat", offsetof(TestClass, SomeFloat), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("SomeField", offsetof(TestClass, SomeField), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Engine>();
	{
		Field& currentField = currentClass->AddField(Field("myFrameIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsRunning", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myEngineProperties", -1, ReflectionSystem::GetOrCreateType<EngineProperties>("EngineProperties"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySystemManager", -1, ReflectionSystem::GetOrCreateType<UniquePtr<SystemManager<System>>>("UniquePtr<SystemManager<System>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myThreadPool", -1, ReflectionSystem::GetOrCreateType<UniquePtr<ThreadPool>>("UniquePtr<ThreadPool>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFilewatcher", -1, ReflectionSystem::GetOrCreateType<UniquePtr<Filewatcher>>("UniquePtr<Filewatcher>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWindowHandler", -1, ReflectionSystem::GetOrCreateType<UniquePtr<WindowHandler>>("UniquePtr<WindowHandler>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVulkanContext", -1, ReflectionSystem::GetOrCreateType<UniquePtr<VulkanContext>>("UniquePtr<VulkanContext>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWorld", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<World>>("std::shared_ptr<World>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myExternalTickFunction", -1, ReflectionSystem::GetOrCreateType<std::function<void ()>>("std::function<void ()>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
static thread_local bool result = instance->ShouldRun();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ShouldRun", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
static thread_local unsigned int result = instance->GetFrameIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFrameIndex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
const bool& arg0 = *(const bool*)inArguments[0];
instance->SetIsRunning(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIsRunning", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetIsRunning", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
const EngineProperties & result = instance->GetEngineProperties();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetEngineProperties", ReflectionSystem::GetOrCreateType<const EngineProperties &>("const EngineProperties &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
const WindowHandler & result = instance->GetWindowHandler();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWindowHandler", ReflectionSystem::GetOrCreateType<const WindowHandler &>("const WindowHandler &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
ThreadPool & result = instance->GetThreadPool();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetThreadPool", ReflectionSystem::GetOrCreateType<ThreadPool &>("ThreadPool &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
Filewatcher & result = instance->GetFilewatcher();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFilewatcher", ReflectionSystem::GetOrCreateType<Filewatcher &>("Filewatcher &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
static thread_local std::shared_ptr<World> result = instance->GetWorld();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWorld", ReflectionSystem::GetOrCreateType<std::shared_ptr<World>>("std::shared_ptr<World>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
static thread_local std::weak_ptr<World> result = instance->GetWeakWorld();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWeakWorld", ReflectionSystem::GetOrCreateType<std::weak_ptr<World>>("std::weak_ptr<World>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
std::shared_ptr<World>& arg0 = *(std::shared_ptr<World>*)inArguments[0];
instance->SetWorld(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inWorld", ReflectionSystem::GetOrCreateType<std::shared_ptr<World>>("std::shared_ptr<World>")));
Method& currentMethod = currentClass->AddMethod(Method("SetWorld", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
static thread_local glm::vec<2, float> result = instance->GetRenderResolution();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRenderResolution", ReflectionSystem::GetOrCreateType<glm::vec<2, float>>("glm::vec<2, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Engine* instance = static_cast<Engine*>(inInstance);
const std::function<void ()>& arg0 = *(const std::function<void ()>*)inArguments[0];
instance->SetExternalTickFunction(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inExternalTickFunction", ReflectionSystem::GetOrCreateType<const std::function<void ()>>("const std::function<void ()>")));
Method& currentMethod = currentClass->AddMethod(Method("SetExternalTickFunction", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<std::shared_ptr<World>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<World>("World"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<std::shared_ptr<Texture>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Texture>("Texture"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<std::shared_ptr<Material>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Material>("Material"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<std::shared_ptr<Model>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Model>("Model"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<std::shared_ptr<Shader>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Shader>("Shader"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<std::shared_ptr<Asset>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<Asset>("Asset"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Field>();
	{
		Field& currentField = currentClass->AddField(Field("myName", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myOffset", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myType", -1, ReflectionSystem::GetOrCreateType<const Type>("const Type"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsPointer", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsReference", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMetadata", -1, ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
const std::basic_string<char> & result = instance->GetName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
static thread_local unsigned int result = instance->GetOffset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetOffset", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
const Type * result = instance->GetType();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
const List<std::basic_string<char>> & result = instance->GetMetadata();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMetadata", ReflectionSystem::GetOrCreateType<const List<std::basic_string<char>> &>("const List<std::basic_string<char>> &"), invoker, arguments));
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
arguments.Add(MethodArgument("inMetadata", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("HasMetadata", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
static thread_local bool result = instance->IsPointer();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsPointer", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Field* instance = static_cast<Field*>(inInstance);
static thread_local bool result = instance->IsReference();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsReference", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
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
arguments.Add(MethodArgument("inMetadata", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetMetadataArgs", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
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
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
Method& currentMethod = currentClass->AddMethod(Method("GetPointerToValue", ReflectionSystem::GetOrCreateType<void *>("void *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Delegate<void *(void *, const List<void *> &)>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<void *(void *, const List<void *> &)>("void *(void *, const List<void *> &)"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Delegate<void *()>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<void *()>("void *()"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Delegate<void (void *)>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<void (void *)>("void (void *)"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Delegate<void (IUniquePtr *)>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<void (IUniquePtr *)>("void (IUniquePtr *)"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Delegate<std::shared_ptr<void> ()>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<std::shared_ptr<void> ()>("std::shared_ptr<void> ()"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<void (physx::PxPhysics *, physx::PxScene *)>("void (physx::PxPhysics *, physx::PxScene *)"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Delegate<void ()>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<void ()>("void ()"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Actor>();
	{
		Field& currentField = currentClass->AddField(Field("myTransform", offsetof(Actor, myTransform), ReflectionSystem::GetOrCreateType<TransformComponent>("TransformComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myName", offsetof(Actor, myName), ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myComponents", offsetof(Actor, myComponents), ReflectionSystem::GetOrCreateType<List<Component *>>("List<Component *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWorld", offsetof(Actor, myWorld), ReflectionSystem::GetOrCreateType<World>("World"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTagMask", offsetof(Actor, myTagMask), ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderStateDirty", offsetof(Actor, myRenderStateDirty), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->DoOnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DoOnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->DoOnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DoOnDestroy", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->OnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnDestroy", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->DoTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DoTick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->DoEditorTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DoEditorTick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->TickRenderState();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickRenderState", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->DoTickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DoTickPhysics", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
static thread_local bool result = instance->IsTransientActor();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsTransientActor", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->DoOnTriggerEnter(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("DoOnTriggerEnter", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnTriggerEnter(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerEnter", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->DoOnTrigger(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("DoOnTrigger", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnTrigger(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTrigger", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->DoOnTriggerExit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("DoOnTriggerExit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnTriggerExit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerExit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->DoOnCollisionEnter(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("DoOnCollisionEnter", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnCollisionEnter(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionEnter", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->DoOnCollision(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("DoOnCollision", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnCollision(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollision", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->DoOnCollisionExit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("DoOnCollisionExit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnCollisionExit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionExit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
TransformComponent & result = instance->GetTransform();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTransform", ReflectionSystem::GetOrCreateType<TransformComponent &>("TransformComponent &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
World * result = instance->GetWorld();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWorld", ReflectionSystem::GetOrCreateType<World *>("World *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
instance->AddTags(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTagMask", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("AddTags", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
unsigned int& arg0 = *(unsigned int*)inArguments[0];
instance->RemoveTags(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTags", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveTags", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->ClearTags();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ClearTags", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
unsigned int& arg0 = *(unsigned int*)inArguments[0];
static thread_local bool result = instance->HasAllTags(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTags", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("HasAllTags", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
static thread_local unsigned int result = instance->GetTags();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTags", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
static thread_local bool result = instance->HasAnyTag(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTags", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("HasAnyTag", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->MarkRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("MarkRenderStateDirty", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
instance->SetName(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetName", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
const std::basic_string<char> & result = instance->GetName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->Destroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Destroy", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
static thread_local List<Component *> result = instance->GetAllComponents();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAllComponents", ReflectionSystem::GetOrCreateType<List<Component *>>("List<Component *>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
instance->RegisterComponents();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RegisterComponents", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Actor* instance = static_cast<Actor*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
instance->RegisterComponentsForType(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("RegisterComponentsForType", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<TypeTemplateArgument>();
	{
		Field& currentField = currentClass->AddField(Field("myType", -1, ReflectionSystem::GetOrCreateType<const Type>("const Type"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsPointer", -1, ReflectionSystem::GetOrCreateType<const bool>("const bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsReference", -1, ReflectionSystem::GetOrCreateType<const bool>("const bool"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Type>();
	{
		Field& currentField = currentClass->AddField(Field("myFullName", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTypeName", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myByteSize", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsCopyable", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFactoryFunction", -1, ReflectionSystem::GetOrCreateType<Delegate<void *()>>("Delegate<void *()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPlacementFactoryFunction", -1, ReflectionSystem::GetOrCreateType<Delegate<void (void *)>>("Delegate<void (void *)>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myUniquePtrFactoryFunction", -1, ReflectionSystem::GetOrCreateType<Delegate<void (IUniquePtr *)>>("Delegate<void (IUniquePtr *)>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySharedPtrFactoryFunction", -1, ReflectionSystem::GetOrCreateType<Delegate<std::shared_ptr<void> ()>>("Delegate<std::shared_ptr<void> ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBaseTypes", -1, ReflectionSystem::GetOrCreateType<List<const Type *>>("List<const Type *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDerivedTypes", -1, ReflectionSystem::GetOrCreateType<List<const Type *>>("List<const Type *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFields", -1, ReflectionSystem::GetOrCreateType<List<Field>>("List<Field>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMethods", -1, ReflectionSystem::GetOrCreateType<List<Method>>("List<Method>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTemplateArguments", -1, ReflectionSystem::GetOrCreateType<List<TypeTemplateArgument>>("List<TypeTemplateArgument>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & result = instance->GetName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & result = instance->GetFullName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFullName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const List<const Type *> & result = instance->GetBaseTypes();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBaseTypes", ReflectionSystem::GetOrCreateType<const List<const Type *> &>("const List<const Type *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const List<const Type *> & result = instance->GetDerivedTypes();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDerivedTypes", ReflectionSystem::GetOrCreateType<const List<const Type *> &>("const List<const Type *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const List<Field> & result = instance->GetFields();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFields", ReflectionSystem::GetOrCreateType<const List<Field> &>("const List<Field> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const List<Method> & result = instance->GetMethods();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMethods", ReflectionSystem::GetOrCreateType<const List<Method> &>("const List<Method> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Field * result = instance->FindField(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFieldName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("FindField", ReflectionSystem::GetOrCreateType<const Field *>("const Field *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
static thread_local unsigned int result = instance->GetSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSize", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Field * result = instance->FindFieldByName(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFieldName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("FindFieldByName", ReflectionSystem::GetOrCreateType<const Field *>("const Field *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Field * result = instance->FindFieldByNameRecursive(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFieldName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("FindFieldByNameRecursive", ReflectionSystem::GetOrCreateType<const Field *>("const Field *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
static thread_local bool result = instance->IsCopyable();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsCopyable", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local List<Field> result = instance->GetFieldsWithMetadata(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMetadata", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetFieldsWithMetadata", ReflectionSystem::GetOrCreateType<List<Field>>("List<Field>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local List<Field> result = instance->GetFieldsWithMetadataRecursive(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMetadata", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetFieldsWithMetadataRecursive", ReflectionSystem::GetOrCreateType<List<Field>>("List<Field>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local List<const Method *> result = instance->GetMethodsWithMetadata(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMetadata", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetMethodsWithMetadata", ReflectionSystem::GetOrCreateType<List<const Method *>>("List<const Method *>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Method * result = instance->GetMethod(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMethodName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetMethod", ReflectionSystem::GetOrCreateType<const Method *>("const Method *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Method * result = instance->GetMethodRecursive(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMethodName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetMethodRecursive", ReflectionSystem::GetOrCreateType<const Method *>("const Method *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
static thread_local bool result = instance->IsTemplateSpecialization();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsTemplateSpecialization", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const List<TypeTemplateArgument> & result = instance->GetTemplateArguments();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTemplateArguments", ReflectionSystem::GetOrCreateType<const List<TypeTemplateArgument> &>("const List<TypeTemplateArgument> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
static thread_local std::basic_string<char> result = instance->GetTypeNameWithoutForwardDeclares(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTypeName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetTypeNameWithoutForwardDeclares", ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
void * arg0 = (void*)inArguments[0];
instance->PlacementNew(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("destination", ReflectionSystem::GetOrCreateType<void *>("void *")));
Method& currentMethod = currentClass->AddMethod(Method("PlacementNew", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
IUniquePtr * arg0 = (IUniquePtr*)inArguments[0];
instance->CreateUniquePtr(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inUniquePtr", ReflectionSystem::GetOrCreateType<IUniquePtr *>("IUniquePtr *")));
Method& currentMethod = currentClass->AddMethod(Method("CreateUniquePtr", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Type* instance = static_cast<Type*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
const bool& arg1 = *(const bool*)inArguments[1];
const bool& arg2 = *(const bool*)inArguments[2];
instance->AddTemplateArgument(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTemplateArgumentType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inIsPointer", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
arguments.Add(MethodArgument("inIsReference", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("AddTemplateArgument", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Component>();
	{
		Field& currentField = currentClass->AddField(Field("myActor", -1, ReflectionSystem::GetOrCreateType<Actor>("Actor"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->OnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnDestroy", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->EditorTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("EditorTick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->MarkRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("MarkRenderStateDirty", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
instance->OnRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnRenderStateDirty", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnTriggerEnter(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerEnter", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnTrigger(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTrigger", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnTriggerExit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerExit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnCollisionEnter(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionEnter", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnCollision(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollision", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->OnCollisionExit(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionExit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
TransformComponent & result = instance->GetTransform();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTransform", ReflectionSystem::GetOrCreateType<TransformComponent &>("TransformComponent &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
Actor * result = instance->GetActor();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetActor", ReflectionSystem::GetOrCreateType<Actor *>("Actor *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Component* instance = static_cast<Component*>(inInstance);
World * result = instance->GetWorld();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWorld", ReflectionSystem::GetOrCreateType<World *>("World *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<TransformComponent>();
	{
		Field& currentField = currentClass->AddField(Field("OnPositionChanged", offsetof(TransformComponent, OnPositionChanged), ReflectionSystem::GetOrCreateType<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("OnRotationChanged", offsetof(TransformComponent, OnRotationChanged), ReflectionSystem::GetOrCreateType<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("OnScaleChanged", offsetof(TransformComponent, OnScaleChanged), ReflectionSystem::GetOrCreateType<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPositionDirty", offsetof(TransformComponent, myPositionDirty), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRotationDirty", offsetof(TransformComponent, myRotationDirty), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myScaleDirty", offsetof(TransformComponent, myScaleDirty), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPosition", offsetof(TransformComponent, myPosition), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(OnInspectorChangedEvent(MarkDirtyFromInspector))delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myRotation", offsetof(TransformComponent, myRotation), ReflectionSystem::GetOrCreateType<glm::qua<float>>("glm::qua<float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(OnInspectorChangedEvent(MarkDirtyFromInspector))delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myScale", offsetof(TransformComponent, myScale), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(OnInspectorChangedEvent(MarkDirtyFromInspector))delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkipPhysicsUpdate", offsetof(TransformComponent, mySkipPhysicsUpdate), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myParent", offsetof(TransformComponent, myParent), ReflectionSystem::GetOrCreateType<TransformComponent>("TransformComponent"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myChildren", offsetof(TransformComponent, myChildren), ReflectionSystem::GetOrCreateType<List<TransformComponent *>>("List<TransformComponent *>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local physx::PxTransformT<float> result = instance->AsPxTransform();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("AsPxTransform", ReflectionSystem::GetOrCreateType<physx::PxTransformT<float>>("physx::PxTransformT<float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
TransformComponent & arg0 = *(TransformComponent*)inArguments[0];
instance->SetParent(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inParent", ReflectionSystem::GetOrCreateType<TransformComponent &>("TransformComponent &")));
Method& currentMethod = currentClass->AddMethod(Method("SetParent", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
instance->RemoveParent();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RemoveParent", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
TransformComponent & arg0 = *(TransformComponent*)inArguments[0];
instance->AddChild(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inChild", ReflectionSystem::GetOrCreateType<TransformComponent &>("TransformComponent &")));
Method& currentMethod = currentClass->AddMethod(Method("AddChild", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
TransformComponent & arg0 = *(TransformComponent*)inArguments[0];
instance->RemoveChild(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inChild", ReflectionSystem::GetOrCreateType<TransformComponent &>("TransformComponent &")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveChild", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const List<TransformComponent *> & result = instance->GetChildren();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetChildren", ReflectionSystem::GetOrCreateType<const List<TransformComponent *> &>("const List<TransformComponent *> &"), invoker, arguments));
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
arguments.Add(MethodArgument("inPosition", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetPositionLocal", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inQuaternion", ReflectionSystem::GetOrCreateType<const glm::qua<float> &>("const glm::qua<float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotationLocal", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inScale", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetScaleLocal", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetScaleLocal(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inScale", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetScaleLocal", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inPosition", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetPosition", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
const float& arg1 = *(const float*)inArguments[1];
const float& arg2 = *(const float*)inArguments[2];
instance->SetPosition(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inY", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inZ", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetPosition", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inPosition", ReflectionSystem::GetOrCreateType<const physx::PxVec3T<float> &>("const physx::PxVec3T<float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetPosition", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetPositionX(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetPositionX", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetPositionY(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inY", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetPositionY", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetPositionZ(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inZ", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetPositionZ", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inScale", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetScale", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
const float& arg1 = *(const float*)inArguments[1];
const float& arg2 = *(const float*)inArguments[2];
instance->SetScale(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inY", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inZ", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetScale", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetScale(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inScalar", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetScale", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inQuat", ReflectionSystem::GetOrCreateType<const glm::qua<float> &>("const glm::qua<float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotation", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inQuat", ReflectionSystem::GetOrCreateType<const physx::PxQuatT<float> &>("const physx::PxQuatT<float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotation", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inRotation", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotationRad", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inRotation", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotationDeg", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
const float& arg1 = *(const float*)inArguments[1];
const float& arg2 = *(const float*)inArguments[2];
instance->SetRotationDeg(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inY", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inZ", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotationDeg", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & result = instance->GetPositionLocal();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPositionLocal", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::qua<float> & result = instance->GetRotationLocal();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRotationLocal", ReflectionSystem::GetOrCreateType<const glm::qua<float> &>("const glm::qua<float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const glm::vec<3, float> & result = instance->GetScaleLocal();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetScaleLocal", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local const glm::vec<3, float> result = instance->LocalForward();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LocalForward", ReflectionSystem::GetOrCreateType<const glm::vec<3, float>>("const glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local const glm::vec<3, float> result = instance->LocalRight();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LocalRight", ReflectionSystem::GetOrCreateType<const glm::vec<3, float>>("const glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local const glm::vec<3, float> result = instance->LocalUp();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LocalUp", ReflectionSystem::GetOrCreateType<const glm::vec<3, float>>("const glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::mat<4, 4, float> result = instance->GetMatrix();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMatrix", ReflectionSystem::GetOrCreateType<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::mat<4, 4, float> result = instance->GetMatrixLocal();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMatrixLocal", ReflectionSystem::GetOrCreateType<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetPosition();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPosition", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::qua<float> result = instance->GetRotation();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRotation", ReflectionSystem::GetOrCreateType<glm::qua<float>>("glm::qua<float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetRotationRad();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRotationRad", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetRotationDeg();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRotationDeg", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetScale();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetScale", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
TransformComponent * result = instance->GetParent();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetParent", ReflectionSystem::GetOrCreateType<TransformComponent *>("TransformComponent *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local bool result = instance->HasParent();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("HasParent", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetForward();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetForward", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetUp();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetUp", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetRight();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRight", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
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
arguments.Add(MethodArgument("inDisplacement", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("Move", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
const float& arg1 = *(const float*)inArguments[1];
const float& arg2 = *(const float*)inArguments[2];
instance->Move(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inY", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inZ", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("Move", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inRotation", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("Rotate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
const float& arg1 = *(const float*)inArguments[1];
const float& arg2 = *(const float*)inArguments[2];
instance->Rotate(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inY", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inZ", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("Rotate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local bool result = instance->IsPositionDirty();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsPositionDirty", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local bool result = instance->IsRotationDirty();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsRotationDirty", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
static thread_local bool result = instance->IsScaleDirty();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsScaleDirty", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
instance->MarkDirtyFromInspector();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("MarkDirtyFromInspector", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransformComponent* instance = static_cast<TransformComponent*>(inInstance);
bool& arg0 = *(bool*)inArguments[0];
bool& arg1 = *(bool*)inArguments[1];
bool& arg2 = *(bool*)inArguments[2];
instance->MarkDirty(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPosition", ReflectionSystem::GetOrCreateType<bool>("bool")));
arguments.Add(MethodArgument("inRotation", ReflectionSystem::GetOrCreateType<bool>("bool")));
arguments.Add(MethodArgument("inScale", ReflectionSystem::GetOrCreateType<bool>("bool")));
Method& currentMethod = currentClass->AddMethod(Method("MarkDirty", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Input>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const int& arg0 = *(const int*)inArguments[0];
static thread_local bool result = instance->IsKeyDown(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", ReflectionSystem::GetOrCreateType<const int>("const int")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyDown", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::KeyCode& arg0 = *(const Input::KeyCode*)inArguments[0];
static thread_local bool result = instance->IsKeyDown(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", ReflectionSystem::GetOrCreateType<const Input::KeyCode>("const Input::KeyCode")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyDown", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::MouseButton& arg0 = *(const Input::MouseButton*)inArguments[0];
static thread_local bool result = instance->IsKeyDown(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aMouseButton", ReflectionSystem::GetOrCreateType<const Input::MouseButton>("const Input::MouseButton")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyDown", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const int& arg0 = *(const int*)inArguments[0];
static thread_local bool result = instance->IsKeyUp(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", ReflectionSystem::GetOrCreateType<const int>("const int")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyUp", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::KeyCode& arg0 = *(const Input::KeyCode*)inArguments[0];
static thread_local bool result = instance->IsKeyUp(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", ReflectionSystem::GetOrCreateType<const Input::KeyCode>("const Input::KeyCode")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyUp", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::MouseButton& arg0 = *(const Input::MouseButton*)inArguments[0];
static thread_local bool result = instance->IsKeyUp(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aMouseButton", ReflectionSystem::GetOrCreateType<const Input::MouseButton>("const Input::MouseButton")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyUp", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const int& arg0 = *(const int*)inArguments[0];
static thread_local bool result = instance->IsKeyPressed(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", ReflectionSystem::GetOrCreateType<const int>("const int")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyPressed", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::KeyCode& arg0 = *(const Input::KeyCode*)inArguments[0];
static thread_local bool result = instance->IsKeyPressed(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aKeyCode", ReflectionSystem::GetOrCreateType<const Input::KeyCode>("const Input::KeyCode")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyPressed", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const Input::MouseButton& arg0 = *(const Input::MouseButton*)inArguments[0];
static thread_local bool result = instance->IsKeyPressed(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aMouseButton", ReflectionSystem::GetOrCreateType<const Input::MouseButton>("const Input::MouseButton")));
Method& currentMethod = currentClass->AddMethod(Method("IsKeyPressed", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const glm::vec<2, float> & result = instance->GetMousePosition();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMousePosition", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
const glm::vec<2, float> & result = instance->GetMousePositionClientRelative();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMousePositionClientRelative", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
static thread_local const glm::vec<2, float> result = instance->GetMouseDelta();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMouseDelta", ReflectionSystem::GetOrCreateType<const glm::vec<2, float>>("const glm::vec<2, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
static thread_local const glm::vec<2, float> result = instance->GetMouseDeltaClientRelative();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMouseDeltaClientRelative", ReflectionSystem::GetOrCreateType<const glm::vec<2, float>>("const glm::vec<2, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
unsigned int& arg0 = *(unsigned int*)inArguments[0];
unsigned long long& arg1 = *(unsigned long long*)inArguments[1];
long long& arg2 = *(long long*)inArguments[2];
static thread_local bool result = instance->UpdateEvents(arg0, arg1, arg2);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("message", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
arguments.Add(MethodArgument("wParam", ReflectionSystem::GetOrCreateType<unsigned long long>("unsigned long long")));
arguments.Add(MethodArgument("lParam", ReflectionSystem::GetOrCreateType<long long>("long long")));
Method& currentMethod = currentClass->AddMethod(Method("UpdateEvents", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Input* instance = static_cast<Input*>(inInstance);
instance->EndFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("EndFrame", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ReflectionSystem>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ReflectionSystem* instance = static_cast<ReflectionSystem*>(inInstance);
const List<Type *> & result = instance->GetAllTypes();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAllTypes", ReflectionSystem::GetOrCreateType<const List<Type *> &>("const List<Type *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ReflectionSystem* instance = static_cast<ReflectionSystem*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Type * result = instance->GetTypeByName(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetTypeByName", ReflectionSystem::GetOrCreateType<const Type *>("const Type *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ReflectionSystem* instance = static_cast<ReflectionSystem*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const Type * result = instance->GetTypeByFullName(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFullName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetTypeByFullName", ReflectionSystem::GetOrCreateType<const Type *>("const Type *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<BinarySerializer>();
	{
		Field& currentField = currentClass->AddField(Field("myPath", -1, ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myOutputStream", -1, ReflectionSystem::GetOrCreateType<std::basic_ostream<char>>("std::basic_ostream<char>"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsStreamOpen", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsTemporary", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myInBuffer", -1, ReflectionSystem::GetOrCreateType<List<unsigned char>>("List<unsigned char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myReadOffset", -1, ReflectionSystem::GetOrCreateType<unsigned long long>("unsigned long long"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWasLastTypeSerializationFullyComplete", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMode", -1, ReflectionSystem::GetOrCreateType<BinarySerializer::Mode>("BinarySerializer::Mode"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
static thread_local bool result = instance->IsReading();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsReading", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
static thread_local bool result = instance->IsWriting();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsWriting", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
instance->Close();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Close", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
const bool& arg2 = *(const bool*)inArguments[2];
instance->SerializeType(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOutInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inIsPointer", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SerializeType", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
static thread_local bool result = instance->WasLastTypeSerializationFullyComplete();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("WasLastTypeSerializationFullyComplete", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
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
arguments.Add(MethodArgument("inOutString", ReflectionSystem::GetOrCreateType<std::basic_string<char> &>("std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("SerializeString", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BinarySerializer* instance = static_cast<BinarySerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const int& arg1 = *(const int*)inArguments[1];
instance->SerializeBinaryData(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOutInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<const int>("const int")));
Method& currentMethod = currentClass->AddMethod(Method("SerializeBinaryData", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ImageCubeData>();
	{
		Field& currentField = currentClass->AddField(Field("myWidth", offsetof(ImageCubeData, myWidth), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myHeight", offsetof(ImageCubeData, myHeight), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myChannels", offsetof(ImageCubeData, myChannels), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumMipLevels", offsetof(ImageCubeData, myNumMipLevels), ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myPixelData", offsetof(ImageCubeData, myPixelData), ReflectionSystem::GetOrCreateType<List<float>>("List<float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<TextureCube>();
	{
		Field& currentField = currentClass->AddField(Field("myImage", offsetof(TextureCube, myImage), ReflectionSystem::GetOrCreateType<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBindlessIndex", offsetof(TextureCube, myBindlessIndex), ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImageData", offsetof(TextureCube, myImageData), ReflectionSystem::GetOrCreateType<ImageCubeData>("ImageCubeData"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureCube* instance = static_cast<TextureCube*>(inInstance);
static thread_local List<std::basic_string<char>> result = instance->GetAssetExtensions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetExtensions", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureCube* instance = static_cast<TextureCube*>(inInstance);
static thread_local bool result = instance->IsExternalAsset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsExternalAsset", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureCube* instance = static_cast<TextureCube*>(inInstance);
instance->LoadPropertiesFromSource();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LoadPropertiesFromSource", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureCube* instance = static_cast<TextureCube*>(inInstance);
instance->PostPropertiesSerialized();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("PostPropertiesSerialized", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureCube* instance = static_cast<TextureCube*>(inInstance);
static thread_local vk::ImageView result = instance->GetImageView();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImageView", ReflectionSystem::GetOrCreateType<vk::ImageView>("vk::ImageView"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureCube* instance = static_cast<TextureCube*>(inInstance);
static thread_local unsigned int result = instance->GetBindlessIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBindlessIndex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<System>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
System* instance = static_cast<System*>(inInstance);
instance->Init();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Init", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<StaticMeshComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myMaterials", offsetof(StaticMeshComponent, myMaterials), ReflectionSystem::GetOrCreateType<List<std::shared_ptr<Material>>>("List<std::shared_ptr<Material>>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(OnInspectorChangedEvent(OnModelChangedFromInspector))delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myModel", offsetof(StaticMeshComponent, myModel), ReflectionSystem::GetOrCreateType<std::shared_ptr<Model>>("std::shared_ptr<Model>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(OnInspectorChangedEvent(OnModelChangedFromInspector))delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMeshInstances", offsetof(StaticMeshComponent, myMeshInstances), ReflectionSystem::GetOrCreateType<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShouldWriteDepth", offsetof(StaticMeshComponent, myShouldWriteDepth), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
std::shared_ptr<Model>& arg0 = *(std::shared_ptr<Model>*)inArguments[0];
instance->SetModel(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inModel", ReflectionSystem::GetOrCreateType<std::shared_ptr<Model>>("std::shared_ptr<Model>")));
Method& currentMethod = currentClass->AddMethod(Method("SetModel", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
static thread_local std::shared_ptr<Model> result = instance->GetModel();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetModel", ReflectionSystem::GetOrCreateType<std::shared_ptr<Model>>("std::shared_ptr<Model>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const unsigned int& arg1 = *(const unsigned int*)inArguments[1];
instance->SetMaterialAsync(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMaterialPath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inIndex", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetMaterialAsync", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
std::shared_ptr<Material>& arg0 = *(std::shared_ptr<Material>*)inArguments[0];
const unsigned int& arg1 = *(const unsigned int*)inArguments[1];
instance->SetMaterial(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMaterial", ReflectionSystem::GetOrCreateType<std::shared_ptr<Material>>("std::shared_ptr<Material>")));
arguments.Add(MethodArgument("inIndex", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetMaterial", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const unsigned int& arg1 = *(const unsigned int*)inArguments[1];
instance->SetMaterial(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMaterialPath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inIndex", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetMaterial", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
static thread_local std::shared_ptr<Material> result = instance->GetMaterial(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIndex", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("GetMaterial", ReflectionSystem::GetOrCreateType<std::shared_ptr<Material>>("std::shared_ptr<Material>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
std::shared_ptr<Material>& arg0 = *(std::shared_ptr<Material>*)inArguments[0];
Mesh * arg1 = (Mesh*)inArguments[1];
instance->SetMaterialForMesh(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMaterial", ReflectionSystem::GetOrCreateType<std::shared_ptr<Material>>("std::shared_ptr<Material>")));
arguments.Add(MethodArgument("inMesh", ReflectionSystem::GetOrCreateType<Mesh *>("Mesh *")));
Method& currentMethod = currentClass->AddMethod(Method("SetMaterialForMesh", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
Mesh * arg0 = (Mesh*)inArguments[0];
static thread_local std::shared_ptr<Material> result = instance->GetMaterialForMesh(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMesh", ReflectionSystem::GetOrCreateType<Mesh *>("Mesh *")));
Method& currentMethod = currentClass->AddMethod(Method("GetMaterialForMesh", ReflectionSystem::GetOrCreateType<std::shared_ptr<Material>>("std::shared_ptr<Material>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
instance->OnRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnRenderStateDirty", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
const bool& arg0 = *(const bool*)inArguments[0];
instance->SetDepthWriteEnabled(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inEnabled", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetDepthWriteEnabled", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
instance->RemoveFromGPUScene();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RemoveFromGPUScene", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
instance->UpdateMaterialsForNewMesh();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("UpdateMaterialsForNewMesh", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshComponent* instance = static_cast<StaticMeshComponent*>(inInstance);
instance->OnModelChangedFromInspector();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnModelChangedFromInspector", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<GlmSerializer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GlmSerializer* instance = static_cast<GlmSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GlmSerializer* instance = static_cast<GlmSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inSerializer", ReflectionSystem::GetOrCreateType<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ThreadUtils>();
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
arguments.Add(MethodArgument("inThread", ReflectionSystem::GetOrCreateType<std::thread &>("std::thread &")));
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<const std::basic_string<wchar_t> &>("const std::basic_string<wchar_t> &")));
Method& currentMethod = currentClass->AddMethod(Method("NameThread", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inThreadHandle", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inString", ReflectionSystem::GetOrCreateType<const std::basic_string<wchar_t> &>("const std::basic_string<wchar_t> &")));
Method& currentMethod = currentClass->AddMethod(Method("NameThread", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ThreadUtils* instance = static_cast<ThreadUtils*>(inInstance);
instance->RegisterThisThreadAsMainThread();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RegisterThisThreadAsMainThread", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ThreadUtils* instance = static_cast<ThreadUtils*>(inInstance);
static thread_local bool result = instance->IsOnMainThread();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsOnMainThread", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<AssetRegistry>();
	{
		Field& currentField = currentClass->AddField(Field("myMutex", -1, ReflectionSystem::GetOrCreateType<std::recursive_mutex>("std::recursive_mutex"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLoadedAssets", -1, ReflectionSystem::GetOrCreateType<std::unordered_map<std::filesystem::path, std::weak_ptr<Asset>>>("std::unordered_map<std::filesystem::path, std::weak_ptr<Asset>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPendingAssets", -1, ReflectionSystem::GetOrCreateType<MutexList<std::filesystem::path>>("MutexList<std::filesystem::path>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
AssetRegistry * result = instance->Get();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Get", ReflectionSystem::GetOrCreateType<AssetRegistry *>("AssetRegistry *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
const Delegate<void (std::shared_ptr<Asset>)> & arg2 = *(const Delegate<void (std::shared_ptr<Asset>)>*)inArguments[2];
instance->GetAssetAsync(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inOnCompleteDelegate", ReflectionSystem::GetOrCreateType<const Delegate<void (std::shared_ptr<Asset>)> &>("const Delegate<void (std::shared_ptr<Asset>)> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetAssetAsync", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
static thread_local std::shared_ptr<Asset> result = instance->GetAssetSynchronous(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("GetAssetSynchronous", ReflectionSystem::GetOrCreateType<std::shared_ptr<Asset>>("std::shared_ptr<Asset>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
static thread_local std::shared_ptr<Asset> result = instance->LoadExternalAsset(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("LoadExternalAsset", ReflectionSystem::GetOrCreateType<std::shared_ptr<Asset>>("std::shared_ptr<Asset>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
static thread_local std::shared_ptr<Asset> result = instance->LoadInternalAsset(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("LoadInternalAsset", ReflectionSystem::GetOrCreateType<std::shared_ptr<Asset>>("std::shared_ptr<Asset>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
static thread_local std::shared_ptr<Asset> result = instance->CreateNewAsset(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("CreateNewAsset", ReflectionSystem::GetOrCreateType<std::shared_ptr<Asset>>("std::shared_ptr<Asset>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
static thread_local std::filesystem::path result = instance->SourceToCachePath(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("SourceToCachePath", ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
static thread_local std::shared_ptr<Asset> result = instance->TryGetLoadedAsset(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("TryGetLoadedAsset", ReflectionSystem::GetOrCreateType<std::shared_ptr<Asset>>("std::shared_ptr<Asset>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetRegistry* instance = static_cast<AssetRegistry*>(inInstance);
Asset * arg0 = (Asset*)inArguments[0];
instance->OnAssetRemoved(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inAsset", ReflectionSystem::GetOrCreateType<Asset *>("Asset *")));
Method& currentMethod = currentClass->AddMethod(Method("OnAssetRemoved", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<SkyboxPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDescriptorSet", -1, ReflectionSystem::GetOrCreateType<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", -1, ReflectionSystem::GetOrCreateType<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", -1, ReflectionSystem::GetOrCreateType<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkyboxModel", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Model>>("std::shared_ptr<Model>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkybox", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SkyboxPipeline* instance = static_cast<SkyboxPipeline*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->AddGraphicsCommands(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("AddGraphicsCommands", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ConvexColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myModel", -1, ReflectionSystem::GetOrCreateType<Model>("Model"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ConvexColliderComponent* instance = static_cast<ConvexColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ConvexColliderComponent* instance = static_cast<ConvexColliderComponent*>(inInstance);
instance->OnScaleChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnScaleChanged", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inModel", ReflectionSystem::GetOrCreateType<Model *>("Model *")));
Method& currentMethod = currentClass->AddMethod(Method("SetModel", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PointLightActor>();
	{
		Field& currentField = currentClass->AddField(Field("myPointLight", offsetof(PointLightActor, myPointLight), ReflectionSystem::GetOrCreateType<PointLightComponent>("PointLightComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Actor>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightActor* instance = static_cast<PointLightActor*>(inInstance);
PointLightComponent & result = instance->GetPointLightComponent();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPointLightComponent", ReflectionSystem::GetOrCreateType<PointLightComponent &>("PointLightComponent &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MutexList<std::filesystem::path>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<JsonAsset>();
	{
		Field& currentField = currentClass->AddField(Field("myJson", -1, ReflectionSystem::GetOrCreateType<nlohmann::basic_json<>>("nlohmann::basic_json<>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
JsonAsset* instance = static_cast<JsonAsset*>(inInstance);
static thread_local bool result = instance->IsCacheValid();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsCacheValid", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
JsonAsset* instance = static_cast<JsonAsset*>(inInstance);
static thread_local List<std::basic_string<char>> result = instance->GetAssetExtensions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetExtensions", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
JsonAsset* instance = static_cast<JsonAsset*>(inInstance);
static thread_local bool result = instance->IsExternalAsset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsExternalAsset", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
JsonAsset* instance = static_cast<JsonAsset*>(inInstance);
instance->LoadPropertiesFromSource();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LoadPropertiesFromSource", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
JsonAsset* instance = static_cast<JsonAsset*>(inInstance);
const nlohmann::basic_json<> & result = instance->GetJson();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetJson", ReflectionSystem::GetOrCreateType<const nlohmann::basic_json<> &>("const nlohmann::basic_json<> &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PreviousWorldsSystem>();
	{
		Field& currentField = currentClass->AddField(Field("mySavePath", -1, ReflectionSystem::GetOrCreateType<const std::basic_string<char>>("const std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaxWorldsSaved", -1, ReflectionSystem::GetOrCreateType<const int>("const int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPreviousWorlds", -1, ReflectionSystem::GetOrCreateType<List<std::filesystem::path>>("List<std::filesystem::path>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<EditorSystem>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PreviousWorldsSystem* instance = static_cast<PreviousWorldsSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Material>();
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoTexture", offsetof(Material, myAlbedoTexture), ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalTexture", offsetof(Material, myNormalTexture), ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialTexture", offsetof(Material, myMaterialTexture), ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myDepthWriteEnabled", offsetof(Material, myDepthWriteEnabled), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
static thread_local bool result = instance->IsExternalAsset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsExternalAsset", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
static thread_local List<std::basic_string<char>> result = instance->GetAssetExtensions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetExtensions", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
static thread_local std::shared_ptr<Texture> result = instance->GetAlbedo();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAlbedo", ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
static thread_local std::shared_ptr<Texture> result = instance->GetNormal();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNormal", ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
static thread_local std::shared_ptr<Texture> result = instance->GetMaterial();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMaterial", ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
std::shared_ptr<Texture>& arg0 = *(std::shared_ptr<Texture>*)inArguments[0];
instance->SetAlbedo(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTexture", ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>")));
Method& currentMethod = currentClass->AddMethod(Method("SetAlbedo", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
instance->SetAlbedo(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("SetAlbedo", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
std::shared_ptr<Texture>& arg0 = *(std::shared_ptr<Texture>*)inArguments[0];
instance->SetNormal(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTexture", ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>")));
Method& currentMethod = currentClass->AddMethod(Method("SetNormal", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
instance->SetNormal(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("SetNormal", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
std::shared_ptr<Texture>& arg0 = *(std::shared_ptr<Texture>*)inArguments[0];
instance->SetMaterial(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTexture", ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>")));
Method& currentMethod = currentClass->AddMethod(Method("SetMaterial", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
const std::filesystem::path & arg0 = *(const std::filesystem::path*)inArguments[0];
instance->SetMaterial(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourcePath", ReflectionSystem::GetOrCreateType<const std::filesystem::path &>("const std::filesystem::path &")));
Method& currentMethod = currentClass->AddMethod(Method("SetMaterial", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Material* instance = static_cast<Material*>(inInstance);
static thread_local bool result = instance->GetDepthWriteEnabled();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDepthWriteEnabled", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<SerializationMeshData>();
	{
		Field& currentField = currentClass->AddField(Field("myVertices", offsetof(SerializationMeshData, myVertices), ReflectionSystem::GetOrCreateType<List<Vertex>>("List<Vertex>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndices", offsetof(SerializationMeshData, myIndices), ReflectionSystem::GetOrCreateType<List<unsigned int>>("List<unsigned int>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySphereCenterBounds", offsetof(SerializationMeshData, mySphereCenterBounds), ReflectionSystem::GetOrCreateType<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoPath", offsetof(SerializationMeshData, myAlbedoPath), ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalPath", offsetof(SerializationMeshData, myNormalPath), ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialPath", offsetof(SerializationMeshData, myMaterialPath), ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Model>();
	{
		Field& currentField = currentClass->AddField(Field("myMeshes", offsetof(Model, myMeshes), ReflectionSystem::GetOrCreateType<List<Mesh *>>("List<Mesh *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMeshDatas", offsetof(Model, myMeshDatas), ReflectionSystem::GetOrCreateType<List<SerializationMeshData>>("List<SerializationMeshData>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Model* instance = static_cast<Model*>(inInstance);
static thread_local List<std::basic_string<char>> result = instance->GetAssetExtensions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetExtensions", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Model* instance = static_cast<Model*>(inInstance);
instance->PostPropertiesSerialized();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("PostPropertiesSerialized", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Model* instance = static_cast<Model*>(inInstance);
instance->LoadPropertiesFromSource();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LoadPropertiesFromSource", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Model* instance = static_cast<Model*>(inInstance);
static thread_local bool result = instance->IsExternalAsset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsExternalAsset", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Model* instance = static_cast<Model*>(inInstance);
const List<Mesh *> & result = instance->GetMeshes();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMeshes", ReflectionSystem::GetOrCreateType<const List<Mesh *> &>("const List<Mesh *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Model* instance = static_cast<Model*>(inInstance);
const List<SerializationMeshData> & result = instance->GetSerializationMeshDatas();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSerializationMeshDatas", ReflectionSystem::GetOrCreateType<const List<SerializationMeshData> &>("const List<SerializationMeshData> &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IncludeData>();
	{
		Field& currentField = currentClass->AddField(Field("myIncludePath", offsetof(IncludeData, myIncludePath), ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myLastWriteTime", offsetof(IncludeData, myLastWriteTime), ReflectionSystem::GetOrCreateType<std::chrono::time_point<std::filesystem::_File_time_clock>>("std::chrono::time_point<std::filesystem::_File_time_clock>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myCallbackHandle", offsetof(IncludeData, myCallbackHandle), ReflectionSystem::GetOrCreateType<Filewatcher::CallbackHandle>("Filewatcher::CallbackHandle"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Shader>();
	{
		Field& currentField = currentClass->AddField(Field("OnShaderRecompiled", offsetof(Shader, OnShaderRecompiled), ReflectionSystem::GetOrCreateType<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShaderModule", offsetof(Shader, myShaderModule), ReflectionSystem::GetOrCreateType<vk::ShaderModule>("vk::ShaderModule"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCallbackHandle", offsetof(Shader, myCallbackHandle), ReflectionSystem::GetOrCreateType<Filewatcher::CallbackHandle>("Filewatcher::CallbackHandle"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShaderBinary", offsetof(Shader, myShaderBinary), ReflectionSystem::GetOrCreateType<List<unsigned int>>("List<unsigned int>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myIncludes", offsetof(Shader, myIncludes), ReflectionSystem::GetOrCreateType<List<IncludeData>>("List<IncludeData>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
static thread_local List<std::basic_string<char>> result = instance->GetAssetExtensions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetExtensions", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
instance->LoadPropertiesFromSource();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("LoadPropertiesFromSource", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
instance->PostPropertiesSerialized();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("PostPropertiesSerialized", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
static thread_local bool result = instance->IsExternalAsset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsExternalAsset", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
static thread_local bool result = instance->IsCacheValid();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsCacheValid", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
instance->Recompile();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Recompile", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
static thread_local vk::ShaderModule result = instance->GetAPIResource();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAPIResource", ReflectionSystem::GetOrCreateType<vk::ShaderModule>("vk::ShaderModule"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
const List<IncludeData> & arg0 = *(const List<IncludeData>*)inArguments[0];
instance->CreateFilewatcherCallbacks(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIncludePaths", ReflectionSystem::GetOrCreateType<const List<IncludeData> &>("const List<IncludeData> &")));
Method& currentMethod = currentClass->AddMethod(Method("CreateFilewatcherCallbacks", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
instance->RemoveFilewatcherCallbacks();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RemoveFilewatcherCallbacks", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Shader* instance = static_cast<Shader*>(inInstance);
const List<unsigned int> & arg0 = *(const List<unsigned int>*)inArguments[0];
instance->InitFromBinary(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inData", ReflectionSystem::GetOrCreateType<const List<unsigned int> &>("const List<unsigned int> &")));
Method& currentMethod = currentClass->AddMethod(Method("InitFromBinary", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MulticastDelegate<void ()>>();
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<void ()>("void ()"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<CharacterControllerComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myController", offsetof(CharacterControllerComponent, myController), ReflectionSystem::GetOrCreateType<physx::PxController>("physx::PxController"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCollisionFlags", offsetof(CharacterControllerComponent, myCollisionFlags), ReflectionSystem::GetOrCreateType<physx::PxFlags<physx::PxControllerCollisionFlag::Enum, unsigned char>>("physx::PxFlags<physx::PxControllerCollisionFlag::Enum, unsigned char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFilter", offsetof(CharacterControllerComponent, myFilter), ReflectionSystem::GetOrCreateType<physx::PxControllerFilters>("physx::PxControllerFilters"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHeight", offsetof(CharacterControllerComponent, myHeight), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myRadius", offsetof(CharacterControllerComponent, myRadius), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySlopeLimitDegrees", offsetof(CharacterControllerComponent, mySlopeLimitDegrees), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMinDist", offsetof(CharacterControllerComponent, myMinDist), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStepOffset", offsetof(CharacterControllerComponent, myStepOffset), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myUseGravity", offsetof(CharacterControllerComponent, myUseGravity), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myDownVelocity", offsetof(CharacterControllerComponent, myDownVelocity), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGravity", offsetof(CharacterControllerComponent, myGravity), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myTerminalVelocity", offsetof(CharacterControllerComponent, myTerminalVelocity), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myDisplacement", offsetof(CharacterControllerComponent, myDisplacement), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPositionOffset", offsetof(CharacterControllerComponent, myPositionOffset), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
instance->OnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnDestroy", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
instance->OnPositionChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnPositionChanged", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inMovement", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("Move", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
static thread_local float result = instance->GetHeight();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHeight", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetHeight(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inHeight", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetHeight", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
static thread_local float result = instance->GetRadius();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRadius", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetRadius(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRadius", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetRadius", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
static thread_local bool result = instance->IsGrounded();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsGrounded", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
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
arguments.Add(MethodArgument("inOffset", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetPositionOffset", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CharacterControllerComponent* instance = static_cast<CharacterControllerComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->Jump(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inJumpForce", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("Jump", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IRenderPass>();
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<RigidbodyComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myActor", offsetof(RigidbodyComponent, myActor), ReflectionSystem::GetOrCreateType<physx::PxRigidDynamic>("physx::PxRigidDynamic"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFramesSinceStartSleep", offsetof(RigidbodyComponent, myFramesSinceStartSleep), ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMass", offsetof(RigidbodyComponent, myMass), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
instance->OnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnDestroy", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inVelocity", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetVelocity", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetVelocity();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetVelocity", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
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
arguments.Add(MethodArgument("inAngularVelocity", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetAngularVelocity", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
static thread_local glm::vec<3, float> result = instance->GetAngularVelocity();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAngularVelocity", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const ForceMode& arg1 = *(const ForceMode*)inArguments[1];
instance->AddForce(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inForce", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inForceMode", ReflectionSystem::GetOrCreateType<const ForceMode>("const ForceMode")));
Method& currentMethod = currentClass->AddMethod(Method("AddForce", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const ForceMode& arg1 = *(const ForceMode*)inArguments[1];
instance->AddTorque(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTorque", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inForceMode", ReflectionSystem::GetOrCreateType<const ForceMode>("const ForceMode")));
Method& currentMethod = currentClass->AddMethod(Method("AddTorque", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetMass(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMass", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetMass", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
static thread_local float result = instance->GetMass();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMass", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetDrag(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inDrag", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetDrag", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
static thread_local float result = instance->GetDrag();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDrag", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetAngularDrag(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inDrag", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetAngularDrag", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
static thread_local float result = instance->GetAngularDrag();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAngularDrag", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const bool& arg0 = *(const bool*)inArguments[0];
instance->UseGravity(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inValue", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("UseGravity", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const bool& arg0 = *(const bool*)inArguments[0];
instance->SetKinematic(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inValue", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetKinematic", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inCollider", ReflectionSystem::GetOrCreateType<ColliderComponent *>("ColliderComponent *")));
Method& currentMethod = currentClass->AddMethod(Method("AttachCollider", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inCollider", ReflectionSystem::GetOrCreateType<ColliderComponent *>("ColliderComponent *")));
Method& currentMethod = currentClass->AddMethod(Method("DetachCollider", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
const bool& arg0 = *(const bool*)inArguments[0];
const bool& arg1 = *(const bool*)inArguments[1];
const bool& arg2 = *(const bool*)inArguments[2];
instance->SetRotationConstraint(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inX", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
arguments.Add(MethodArgument("inY", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
arguments.Add(MethodArgument("inZ", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetRotationConstraint", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
instance->OnPositionChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnPositionChanged", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RigidbodyComponent* instance = static_cast<RigidbodyComponent*>(inInstance);
instance->OnRotationChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnRotationChanged", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<GPUSceneSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myFreeSparseIndices", -1, ReflectionSystem::GetOrCreateType<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNextFreeSparseIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseBuffer", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumGPUObjectUpdatesThisFrame", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDenseBuffer", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDenseBufferCPURepresentation", -1, ReflectionSystem::GetOrCreateType<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inMeshInstanceData", ReflectionSystem::GetOrCreateType<const MeshInstanceData &>("const MeshInstanceData &")));
Method& currentMethod = currentClass->AddMethod(Method("AddMeshInstance", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
const MeshInstanceData & arg1 = *(const MeshInstanceData*)inArguments[1];
instance->UpdateMeshInstance(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMeshIndex", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inMeshInstanceData", ReflectionSystem::GetOrCreateType<const MeshInstanceData &>("const MeshInstanceData &")));
Method& currentMethod = currentClass->AddMethod(Method("UpdateMeshInstance", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
instance->RemoveMeshInstance(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMeshInstance", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveMeshInstance", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
const ResizableBuffer * result = instance->GetSparseBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSparseBuffer", ReflectionSystem::GetOrCreateType<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
const ResizableBuffer * result = instance->GetDenseBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDenseBuffer", ReflectionSystem::GetOrCreateType<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GPUSceneSystem* instance = static_cast<GPUSceneSystem*>(inInstance);
static thread_local unsigned int result = instance->GetNumObjects();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNumObjects", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myShape", -1, ReflectionSystem::GetOrCreateType<physx::PxShape>("physx::PxShape"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myActor", -1, ReflectionSystem::GetOrCreateType<physx::PxRigidStatic>("physx::PxRigidStatic"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLocalShapePosition", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLocalShapeRotation", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
instance->OnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnDestroy", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
instance->OnPhysicsStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnPhysicsStateDirty", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
physx::PxShape * result = instance->GetShape();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetShape", ReflectionSystem::GetOrCreateType<physx::PxShape *>("physx::PxShape *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ColliderComponent* instance = static_cast<ColliderComponent*>(inInstance);
const bool& arg0 = *(const bool*)inArguments[0];
instance->SetIsTrigger(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIsTrigger", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("SetIsTrigger", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inOffset", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetLocalShapePosition", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inRotation", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetLocalShapeRotation", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<BoxColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myHalfSize", offsetof(BoxColliderComponent, myHalfSize), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BoxColliderComponent* instance = static_cast<BoxColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inHalfSize", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetHalfSize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
BoxColliderComponent* instance = static_cast<BoxColliderComponent*>(inInstance);
instance->OnScaleChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnScaleChanged", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<LandscapeRenderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myHeightfield", -1, ReflectionSystem::GetOrCreateType<Heightfield>("Heightfield"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexBuffer", -1, ReflectionSystem::GetOrCreateType<VertexBufferHandle>("VertexBufferHandle"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexBuffer", -1, ReflectionSystem::GetOrCreateType<IndexBufferHandle>("IndexBufferHandle"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMesh", -1, ReflectionSystem::GetOrCreateType<Mesh>("Mesh"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMeshInstance", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterial", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Material>>("std::shared_ptr<Material>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeRenderComponent* instance = static_cast<LandscapeRenderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeRenderComponent* instance = static_cast<LandscapeRenderComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeRenderComponent* instance = static_cast<LandscapeRenderComponent*>(inInstance);
instance->OnRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnRenderStateDirty", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeRenderComponent* instance = static_cast<LandscapeRenderComponent*>(inInstance);
const Heightfield & result = instance->GetHeightfield();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHeightfield", ReflectionSystem::GetOrCreateType<const Heightfield &>("const Heightfield &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<CameraComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myResolution", offsetof(CameraComponent, myResolution), ReflectionSystem::GetOrCreateType<glm::vec<2, float>>("glm::vec<2, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myProjection", offsetof(CameraComponent, myProjection), ReflectionSystem::GetOrCreateType<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFov", offsetof(CameraComponent, myFov), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myNearPlane", offsetof(CameraComponent, myNearPlane), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myFarPlane", offsetof(CameraComponent, myFarPlane), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsOrthographic", offsetof(CameraComponent, myIsOrthographic), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const glm::mat<4, 4, float> & result = instance->GetProjection();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetProjection", ReflectionSystem::GetOrCreateType<const glm::mat<4, 4, float> &>("const glm::mat<4, 4, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
const float& arg1 = *(const float*)inArguments[1];
const float& arg2 = *(const float*)inArguments[2];
instance->CreateOrthographic(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inResolution", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
arguments.Add(MethodArgument("inNearPlane", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inFarPlane", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("CreateOrthographic", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
const float& arg1 = *(const float*)inArguments[1];
const float& arg2 = *(const float*)inArguments[2];
const float& arg3 = *(const float*)inArguments[3];
instance->CreatePerspective(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inResolution", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
arguments.Add(MethodArgument("inFov", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inNearPlane", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inFarPlane", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("CreatePerspective", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
instance->SetAsMainCamera();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetAsMainCamera", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
glm::vec<3, float> & arg1 = *(glm::vec<3, float>*)inArguments[1];
static thread_local bool result = instance->ScreenToWorldPos(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inNormalizedScreenPos", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
arguments.Add(MethodArgument("outWorldPos", ReflectionSystem::GetOrCreateType<glm::vec<3, float> &>("glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("ScreenToWorldPos", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
static thread_local glm::vec<3, float> result = instance->ScreenToNearPlane(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inNormalizedScreenPos", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("ScreenToNearPlane", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
static thread_local glm::vec<3, float> result = instance->ScreenToFarPlane(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inNormalizedScreenPos", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("ScreenToFarPlane", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
static thread_local glm::vec<3, float> result = instance->GetRayDirectionFromScreen(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inNormalizedScreenPos", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("GetRayDirectionFromScreen", ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
static thread_local float result = instance->ToLinearDepth(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inNonLinearDepth", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("ToLinearDepth", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CameraComponent* instance = static_cast<CameraComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
static thread_local float result = instance->ToNormalizedLinearDepth(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inNDCDepth", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("ToNormalizedLinearDepth", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<EditorThemes>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorThemes* instance = static_cast<EditorThemes*>(inInstance);
instance->DefaultTheme();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DefaultTheme", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
currentMethod.AddMetadata(R"delim(EditorMenuItem("Themes/Default"))delim");
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorThemes* instance = static_cast<EditorThemes*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
static thread_local unsigned int result = instance->FromHex(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inHex", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("FromHex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<CapsuleColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myRadius", offsetof(CapsuleColliderComponent, myRadius), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myHeight", offsetof(CapsuleColliderComponent, myHeight), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CapsuleColliderComponent* instance = static_cast<CapsuleColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CapsuleColliderComponent* instance = static_cast<CapsuleColliderComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetRadius(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRadius", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetRadius", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CapsuleColliderComponent* instance = static_cast<CapsuleColliderComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetHeight(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inHeight", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetHeight", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CapsuleColliderComponent* instance = static_cast<CapsuleColliderComponent*>(inInstance);
instance->OnScaleChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnScaleChanged", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<DirectionalLightComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myIsShadowsEnabled", offsetof(DirectionalLightComponent, myIsShadowsEnabled), ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myColor", offsetof(DirectionalLightComponent, myColor), ReflectionSystem::GetOrCreateType<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(ExposeAsColor)delim");
		currentField.AddMetadata(R"delim(OnInspectorChangedEvent(OnColorChanged))delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myLightProjection", offsetof(DirectionalLightComponent, myLightProjection), ReflectionSystem::GetOrCreateType<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inColor", ReflectionSystem::GetOrCreateType<const glm::vec<4, float> &>("const glm::vec<4, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetColor", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetIntensity(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inValue", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetIntensity", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
const glm::vec<4, float> & result = instance->GetColor();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetColor", ReflectionSystem::GetOrCreateType<const glm::vec<4, float> &>("const glm::vec<4, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
const glm::mat<4, 4, float> & result = instance->GetLightProjection();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetLightProjection", ReflectionSystem::GetOrCreateType<const glm::mat<4, 4, float> &>("const glm::mat<4, 4, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
static thread_local bool result = instance->IsShadowsEnabled();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsShadowsEnabled", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
instance->EnableShadows();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("EnableShadows", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightComponent* instance = static_cast<DirectionalLightComponent*>(inInstance);
instance->OnColorChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnColorChanged", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<EditorCameraMovementComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myResetMouseDelta", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMouseDelta", -1, ReflectionSystem::GetOrCreateType<glm::vec<2, float>>("glm::vec<2, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myYaw", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPitch", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMovementSpeed", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShiftMultiplier", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myScrollMultiplier", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorCameraMovementComponent* instance = static_cast<EditorCameraMovementComponent*>(inInstance);
instance->ViewportTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ViewportTick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorCameraMovementComponent* instance = static_cast<EditorCameraMovementComponent*>(inInstance);
instance->ResetMouseDelta();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ResetMouseDelta", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<WorldSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myWorld", -1, ReflectionSystem::GetOrCreateType<World>("World"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WorldSystem* instance = static_cast<WorldSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WorldSystem* instance = static_cast<WorldSystem*>(inInstance);
instance->EditorTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("EditorTick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WorldSystem* instance = static_cast<WorldSystem*>(inInstance);
World * result = instance->GetWorld();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetWorld", ReflectionSystem::GetOrCreateType<World *>("World *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<LandscapeColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myHasBoundDelegate", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeColliderComponent* instance = static_cast<LandscapeColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
LandscapeColliderComponent* instance = static_cast<LandscapeColliderComponent*>(inInstance);
instance->OnScaleChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnScaleChanged", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PhysicsSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myPhysicsCommands", -1, ReflectionSystem::GetOrCreateType<List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>>("List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myToleranceScale", -1, ReflectionSystem::GetOrCreateType<physx::PxTolerancesScale>("physx::PxTolerancesScale"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDefaultMaterial", -1, ReflectionSystem::GetOrCreateType<physx::PxMaterial>("physx::PxMaterial"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myListener", -1, ReflectionSystem::GetOrCreateType<PhysicsListener>("PhysicsListener"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPhysics", -1, ReflectionSystem::GetOrCreateType<physx::PxPhysics>("physx::PxPhysics"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myScene", -1, ReflectionSystem::GetOrCreateType<physx::PxScene>("physx::PxScene"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myControllerManager", -1, ReflectionSystem::GetOrCreateType<physx::PxControllerManager>("physx::PxControllerManager"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasActiveSimulation", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<WorldSystem>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
instance->CreateStaticObjects();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateStaticObjects", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
instance->DestoryStaticObjects();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DestoryStaticObjects", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
Delegate<void (physx::PxPhysics *, physx::PxScene *)>& arg0 = *(Delegate<void (physx::PxPhysics *, physx::PxScene *)>*)inArguments[0];
instance->QueuePhysicsCommand(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inDelegate", ReflectionSystem::GetOrCreateType<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>("Delegate<void (physx::PxPhysics *, physx::PxScene *)>")));
Method& currentMethod = currentClass->AddMethod(Method("QueuePhysicsCommand", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
physx::PxTolerancesScale * result = instance->GetToleranceScale();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetToleranceScale", ReflectionSystem::GetOrCreateType<physx::PxTolerancesScale *>("physx::PxTolerancesScale *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
physx::PxMaterial * result = instance->GetDefaultMaterial();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDefaultMaterial", ReflectionSystem::GetOrCreateType<physx::PxMaterial *>("physx::PxMaterial *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
physx::PxScene * result = instance->GetScene();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetScene", ReflectionSystem::GetOrCreateType<physx::PxScene *>("physx::PxScene *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
physx::PxControllerManager * result = instance->GetControllerManager();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetControllerManager", ReflectionSystem::GetOrCreateType<physx::PxControllerManager *>("physx::PxControllerManager *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsSystem* instance = static_cast<PhysicsSystem*>(inInstance);
PhysicsListener * result = instance->GetPhysicsListener();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPhysicsListener", ReflectionSystem::GetOrCreateType<PhysicsListener *>("PhysicsListener *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Heightfield>();
	{
		Field& currentField = currentClass->AddField(Field("mySeed", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPerlinNoise", -1, ReflectionSystem::GetOrCreateType<siv::BasicPerlinNoise<float>>("siv::BasicPerlinNoise<float>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Heightfield* instance = static_cast<Heightfield*>(inInstance);
const glm::vec<2, float>& arg0 = *(const glm::vec<2, float>*)inArguments[0];
static thread_local float result = instance->GetHeight(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCoordinates", ReflectionSystem::GetOrCreateType<const glm::vec<2, float>>("const glm::vec<2, float>")));
Method& currentMethod = currentClass->AddMethod(Method("GetHeight", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MeshColliderComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myModel", -1, ReflectionSystem::GetOrCreateType<Model>("Model"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<ColliderComponent>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshColliderComponent* instance = static_cast<MeshColliderComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshColliderComponent* instance = static_cast<MeshColliderComponent*>(inInstance);
instance->OnScaleChanged();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnScaleChanged", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inModel", ReflectionSystem::GetOrCreateType<Model *>("Model *")));
Method& currentMethod = currentClass->AddMethod(Method("SetModel", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<TypeSerializer>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TypeSerializer* instance = static_cast<TypeSerializer*>(inInstance);
instance->RegisterSerializers();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RegisterSerializers", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TypeSerializer* instance = static_cast<TypeSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
TypeSerializer * result = instance->GetSerializer(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("GetSerializer", ReflectionSystem::GetOrCreateType<TypeSerializer *>("TypeSerializer *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PointLightComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myColor", offsetof(PointLightComponent, myColor), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(ExposeAsColor)delim");
		currentField.AddMetadata(R"delim(OnInspectorChangedEvent(MarkRenderStateDirty))delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myIntensity", offsetof(PointLightComponent, myIntensity), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(OnInspectorChangedEvent(MarkRenderStateDirty))delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myRange", offsetof(PointLightComponent, myRange), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
		currentField.AddMetadata(R"delim(OnInspectorChangedEvent(MarkRenderStateDirty))delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myPointLightInstanceIndex", offsetof(PointLightComponent, myPointLightInstanceIndex), ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
instance->OnDestroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnDestroy", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
instance->OnRenderStateDirty();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnRenderStateDirty", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
const glm::vec<3, float> & result = instance->GetColor();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetColor", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &"), invoker, arguments));
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
arguments.Add(MethodArgument("inColor", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetColor", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
static thread_local float result = instance->GetIntensity();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIntensity", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetIntensity(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIntensity", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetIntensity", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
static thread_local float result = instance->GetRange();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRange", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightComponent* instance = static_cast<PointLightComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetRange(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRange", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetRange", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PromiseReturnTypeImplementation<void>>();
	{
		Field& currentField = currentClass->AddField(Field("myReturnValue", -1, ReflectionSystem::GetOrCreateType<char>("char"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasReturnValue", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<void>("void"), false, false);
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PromiseReturnTypeImplementation<void>* instance = static_cast<PromiseReturnTypeImplementation<void>*>(inInstance);
instance->return_void();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("return_void", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<SinWaveMovementComponent>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SinWaveMovementComponent* instance = static_cast<SinWaveMovementComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<DirectionalLightActor>();
	{
		Field& currentField = currentClass->AddField(Field("myDirectionalLight", offsetof(DirectionalLightActor, myDirectionalLight), ReflectionSystem::GetOrCreateType<DirectionalLightComponent>("DirectionalLightComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Actor>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DirectionalLightActor* instance = static_cast<DirectionalLightActor*>(inInstance);
DirectionalLightComponent & result = instance->GetDirectionalLightComponent();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDirectionalLightComponent", ReflectionSystem::GetOrCreateType<DirectionalLightComponent &>("DirectionalLightComponent &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<LandscapeActor>();
	{
		Field& currentField = currentClass->AddField(Field("myLandscapeRenderComponent", offsetof(LandscapeActor, myLandscapeRenderComponent), ReflectionSystem::GetOrCreateType<LandscapeRenderComponent>("LandscapeRenderComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myLandscapeColliderComponent", offsetof(LandscapeActor, myLandscapeColliderComponent), ReflectionSystem::GetOrCreateType<LandscapeColliderComponent>("LandscapeColliderComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Actor>());
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PhysicsCubeActor>();
	{
		Field& currentField = currentClass->AddField(Field("myMesh", offsetof(PhysicsCubeActor, myMesh), ReflectionSystem::GetOrCreateType<StaticMeshComponent>("StaticMeshComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myCollider", offsetof(PhysicsCubeActor, myCollider), ReflectionSystem::GetOrCreateType<BoxColliderComponent>("BoxColliderComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myRigidbody", offsetof(PhysicsCubeActor, myRigidbody), ReflectionSystem::GetOrCreateType<RigidbodyComponent>("RigidbodyComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Actor>());
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<StaticMeshActor>();
	{
		Field& currentField = currentClass->AddField(Field("myStaticMeshComponent", offsetof(StaticMeshActor, myStaticMeshComponent), ReflectionSystem::GetOrCreateType<StaticMeshComponent>("StaticMeshComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Actor>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StaticMeshActor* instance = static_cast<StaticMeshActor*>(inInstance);
StaticMeshComponent & result = instance->GetStaticMeshComponent();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetStaticMeshComponent", ReflectionSystem::GetOrCreateType<StaticMeshComponent &>("StaticMeshComponent &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<AutoInit>();
	{
		Field& currentField = currentClass->AddField(Field("myHasInited", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<AutoInitManager>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AutoInitManager* instance = static_cast<AutoInitManager*>(inInstance);
AutoInit * arg0 = (AutoInit*)inArguments[0];
instance->AddInitObject(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", ReflectionSystem::GetOrCreateType<AutoInit *>("AutoInit *")));
Method& currentMethod = currentClass->AddMethod(Method("AddInitObject", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AutoInitManager* instance = static_cast<AutoInitManager*>(inInstance);
AutoInit * arg0 = (AutoInit*)inArguments[0];
instance->RemoveInitObject(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", ReflectionSystem::GetOrCreateType<AutoInit *>("AutoInit *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveInitObject", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AutoInitManager* instance = static_cast<AutoInitManager*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ThreadPool>();
	{
		Field& currentField = currentClass->AddField(Field("myThreads", -1, ReflectionSystem::GetOrCreateType<List<std::thread>>("List<std::thread>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLookingForTaskMutex", -1, ReflectionSystem::GetOrCreateType<std::mutex>("std::mutex"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myWaitingForTaskCondition", -1, ReflectionSystem::GetOrCreateType<std::condition_variable>("std::condition_variable"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShouldExit", -1, ReflectionSystem::GetOrCreateType<std::atomic<bool>>("std::atomic<bool>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTasks", -1, ReflectionSystem::GetOrCreateType<std::queue<std::function<void ()>>>("std::queue<std::function<void ()>>"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Time>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
static thread_local float result = instance->GetDeltaTime();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDeltaTime", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
static thread_local float result = instance->GetUnscaledDeltaTime();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetUnscaledDeltaTime", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetTimeScale(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTimeScale", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetTimeScale", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
static thread_local double result = instance->GetSeconds();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSeconds", ReflectionSystem::GetOrCreateType<double>("double"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Time* instance = static_cast<Time*>(inInstance);
static thread_local float result = instance->GetTimeSinceStart();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTimeSinceStart", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Awaitable>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Awaitable* instance = static_cast<Awaitable*>(inInstance);
static thread_local bool result = instance->await_ready();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("await_ready", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Awaitable* instance = static_cast<Awaitable*>(inInstance);
std::coroutine_handle<void>& arg0 = *(std::coroutine_handle<void>*)inArguments[0];
instance->await_suspend(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCoroutineHandle", ReflectionSystem::GetOrCreateType<std::coroutine_handle<void>>("std::coroutine_handle<void>")));
Method& currentMethod = currentClass->AddMethod(Method("await_suspend", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Awaitable* instance = static_cast<Awaitable*>(inInstance);
instance->await_resume();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("await_resume", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<CoroutineManager>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CoroutineManager* instance = static_cast<CoroutineManager*>(inInstance);
static thread_local Coroutine<void, void> result = instance->Load();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Load", ReflectionSystem::GetOrCreateType<Coroutine<void, void>>("Coroutine<void, void>"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PhysicsErrorCallback>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<physx::PxErrorCallback>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsErrorCallback* instance = static_cast<PhysicsErrorCallback*>(inInstance);
physx::PxErrorCode::Enum& arg0 = *(physx::PxErrorCode::Enum*)inArguments[0];
const char * arg1 = (const char*)inArguments[1];
const char * arg2 = (const char*)inArguments[2];
int& arg3 = *(int*)inArguments[3];
instance->reportError(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("code", ReflectionSystem::GetOrCreateType<physx::PxErrorCode::Enum>("physx::PxErrorCode::Enum")));
arguments.Add(MethodArgument("message", ReflectionSystem::GetOrCreateType<const char *>("const char *")));
arguments.Add(MethodArgument("file", ReflectionSystem::GetOrCreateType<const char *>("const char *")));
arguments.Add(MethodArgument("line", ReflectionSystem::GetOrCreateType<int>("int")));
Method& currentMethod = currentClass->AddMethod(Method("reportError", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PhysicsListener>();
	{
		Field& currentField = currentClass->AddField(Field("myDequeueCollisionsDelegate", -1, ReflectionSystem::GetOrCreateType<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<physx::PxSimulationEventCallback>());
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<physx::PxUserControllerHitReport>());
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<physx::PxControllerBehaviorCallback>());
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<physx::PxControllerFilterCallback>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
physx::PxTriggerPair * arg0 = (physx::PxTriggerPair*)inArguments[0];
unsigned int& arg1 = *(unsigned int*)inArguments[1];
instance->onTrigger(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("pairs", ReflectionSystem::GetOrCreateType<physx::PxTriggerPair *>("physx::PxTriggerPair *")));
arguments.Add(MethodArgument("count", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onTrigger", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxContactPairHeader & arg0 = *(const physx::PxContactPairHeader*)inArguments[0];
const physx::PxContactPair * arg1 = (const physx::PxContactPair*)inArguments[1];
unsigned int& arg2 = *(unsigned int*)inArguments[2];
instance->onContact(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("pairHeader", ReflectionSystem::GetOrCreateType<const physx::PxContactPairHeader &>("const physx::PxContactPairHeader &")));
arguments.Add(MethodArgument("pairs", ReflectionSystem::GetOrCreateType<const physx::PxContactPair *>("const physx::PxContactPair *")));
arguments.Add(MethodArgument("nbPairs", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onContact", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
physx::PxConstraintInfo * arg0 = (physx::PxConstraintInfo*)inArguments[0];
unsigned int& arg1 = *(unsigned int*)inArguments[1];
instance->onConstraintBreak(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("constraints", ReflectionSystem::GetOrCreateType<physx::PxConstraintInfo *>("physx::PxConstraintInfo *")));
arguments.Add(MethodArgument("count", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onConstraintBreak", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
physx::PxActor ** arg0 = (physx::PxActor **)inArguments[0];
unsigned int& arg1 = *(unsigned int*)inArguments[1];
instance->onWake(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("actors", ReflectionSystem::GetOrCreateType<physx::PxActor **>("physx::PxActor **")));
arguments.Add(MethodArgument("count", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onWake", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
physx::PxActor ** arg0 = (physx::PxActor **)inArguments[0];
unsigned int& arg1 = *(unsigned int*)inArguments[1];
instance->onSleep(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("actors", ReflectionSystem::GetOrCreateType<physx::PxActor **>("physx::PxActor **")));
arguments.Add(MethodArgument("count", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onSleep", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
const physx::PxRigidBody *const * arg0 = (const physx::PxRigidBody *const*)inArguments[0];
const physx::PxTransformT<float> * arg1 = (const physx::PxTransformT<float>*)inArguments[1];
const unsigned int& arg2 = *(const unsigned int*)inArguments[2];
instance->onAdvance(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("bodyBuffer", ReflectionSystem::GetOrCreateType<const physx::PxRigidBody *const *>("const physx::PxRigidBody *const *")));
arguments.Add(MethodArgument("poseBuffer", ReflectionSystem::GetOrCreateType<const physx::PxTransformT<float> *>("const physx::PxTransformT<float> *")));
arguments.Add(MethodArgument("count", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("onAdvance", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("hit", ReflectionSystem::GetOrCreateType<const physx::PxControllerShapeHit &>("const physx::PxControllerShapeHit &")));
Method& currentMethod = currentClass->AddMethod(Method("onShapeHit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("hit", ReflectionSystem::GetOrCreateType<const physx::PxControllersHit &>("const physx::PxControllersHit &")));
Method& currentMethod = currentClass->AddMethod(Method("onControllerHit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("hit", ReflectionSystem::GetOrCreateType<const physx::PxControllerObstacleHit &>("const physx::PxControllerObstacleHit &")));
Method& currentMethod = currentClass->AddMethod(Method("onObstacleHit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("shape", ReflectionSystem::GetOrCreateType<const physx::PxShape &>("const physx::PxShape &")));
arguments.Add(MethodArgument("actor", ReflectionSystem::GetOrCreateType<const physx::PxActor &>("const physx::PxActor &")));
Method& currentMethod = currentClass->AddMethod(Method("getBehaviorFlags", ReflectionSystem::GetOrCreateType<physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>>("physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>"), invoker, arguments));
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
arguments.Add(MethodArgument("controller", ReflectionSystem::GetOrCreateType<const physx::PxController &>("const physx::PxController &")));
Method& currentMethod = currentClass->AddMethod(Method("getBehaviorFlags", ReflectionSystem::GetOrCreateType<physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>>("physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>"), invoker, arguments));
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
arguments.Add(MethodArgument("obstacle", ReflectionSystem::GetOrCreateType<const physx::PxObstacle &>("const physx::PxObstacle &")));
Method& currentMethod = currentClass->AddMethod(Method("getBehaviorFlags", ReflectionSystem::GetOrCreateType<physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>>("physx::PxFlags<physx::PxControllerBehaviorFlag::Enum, unsigned char>"), invoker, arguments));
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
arguments.Add(MethodArgument("a", ReflectionSystem::GetOrCreateType<const physx::PxController &>("const physx::PxController &")));
arguments.Add(MethodArgument("b", ReflectionSystem::GetOrCreateType<const physx::PxController &>("const physx::PxController &")));
Method& currentMethod = currentClass->AddMethod(Method("filter", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PhysicsListener* instance = static_cast<PhysicsListener*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<NoDepthPass>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<GraphicsPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
NoDepthPass* instance = static_cast<NoDepthPass*>(inInstance);
instance->SetupDescriptors();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupDescriptors", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
NoDepthPass* instance = static_cast<NoDepthPass*>(inInstance);
instance->SetupAttachments();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupAttachments", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
NoDepthPass* instance = static_cast<NoDepthPass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->DrawCall(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("DrawCall", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<RaycastHit>();
	{
		Field& currentField = currentClass->AddField(Field("myHitPosition", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitNormal", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitDistance", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitActor", -1, ReflectionSystem::GetOrCreateType<Actor>("Actor"), true, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanAllocator>();
	{
		Field& currentField = currentClass->AddField(Field("myAllocator", -1, ReflectionSystem::GetOrCreateType<VmaAllocator>("VmaAllocator"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const vk::BufferCreateInfo & arg1 = *(const vk::BufferCreateInfo*)inArguments[1];
VmaMemoryUsage& arg2 = *(VmaMemoryUsage*)inArguments[2];
bool& arg3 = *(bool*)inArguments[3];
VulkanBuffer * result = instance->AllocateBuffer_TS(arg0, arg1, arg2, arg3);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inCreateInfo", ReflectionSystem::GetOrCreateType<const vk::BufferCreateInfo &>("const vk::BufferCreateInfo &")));
arguments.Add(MethodArgument("inUsage", ReflectionSystem::GetOrCreateType<VmaMemoryUsage>("VmaMemoryUsage")));
arguments.Add(MethodArgument("inMappable", ReflectionSystem::GetOrCreateType<bool>("bool")));
Method& currentMethod = currentClass->AddMethod(Method("AllocateBuffer_TS", ReflectionSystem::GetOrCreateType<VulkanBuffer *>("VulkanBuffer *"), invoker, arguments));
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
arguments.Add(MethodArgument("inBuffer", ReflectionSystem::GetOrCreateType<VulkanBuffer *>("VulkanBuffer *")));
Method& currentMethod = currentClass->AddMethod(Method("DestroyBuffer_TS", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inBuffer", ReflectionSystem::GetOrCreateType<ResizableBuffer *>("ResizableBuffer *")));
Method& currentMethod = currentClass->AddMethod(Method("DestroyBuffer_TS", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inCommand", ReflectionSystem::GetOrCreateType<const Delegate<void ()> &>("const Delegate<void ()> &")));
Method& currentMethod = currentClass->AddMethod(Method("QueueDestroyCommand", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
const std::basic_string<char> & arg0 = *(const std::basic_string<char>*)inArguments[0];
const vk::ImageCreateInfo & arg1 = *(const vk::ImageCreateInfo*)inArguments[1];
VmaMemoryUsage& arg2 = *(VmaMemoryUsage*)inArguments[2];
VulkanImage * result = instance->AllocateImage_TS(arg0, arg1, arg2);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inCreateInfo", ReflectionSystem::GetOrCreateType<const vk::ImageCreateInfo &>("const vk::ImageCreateInfo &")));
arguments.Add(MethodArgument("inUsage", ReflectionSystem::GetOrCreateType<VmaMemoryUsage>("VmaMemoryUsage")));
Method& currentMethod = currentClass->AddMethod(Method("AllocateImage_TS", ReflectionSystem::GetOrCreateType<VulkanImage *>("VulkanImage *"), invoker, arguments));
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
arguments.Add(MethodArgument("inImage", ReflectionSystem::GetOrCreateType<VulkanImage *>("VulkanImage *")));
Method& currentMethod = currentClass->AddMethod(Method("DestroyImage_TS", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanAllocator* instance = static_cast<VulkanAllocator*>(inInstance);
VmaAllocator_T * result = instance->GetVMAAllocator();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetVMAAllocator", ReflectionSystem::GetOrCreateType<VmaAllocator_T *>("VmaAllocator_T *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("myAllocation", -1, ReflectionSystem::GetOrCreateType<VmaAllocation>("VmaAllocation"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", -1, ReflectionSystem::GetOrCreateType<vk::Buffer>("vk::Buffer"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCreateInfo", -1, ReflectionSystem::GetOrCreateType<vk::BufferCreateInfo>("vk::BufferCreateInfo"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMemoryUsage", -1, ReflectionSystem::GetOrCreateType<VmaMemoryUsage>("VmaMemoryUsage"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsMappingAllowed", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPtr", -1, ReflectionSystem::GetOrCreateType<void>("void"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
unsigned int& arg0 = *(unsigned int*)inArguments[0];
static thread_local vk::BufferCreateInfo result = instance->StagingCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("StagingCreateInfo", ReflectionSystem::GetOrCreateType<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
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
arguments.Add(MethodArgument("inVertices", ReflectionSystem::GetOrCreateType<const List<Vertex> &>("const List<Vertex> &")));
Method& currentMethod = currentClass->AddMethod(Method("VertexBufferCreateInfo", ReflectionSystem::GetOrCreateType<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
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
arguments.Add(MethodArgument("inIndices", ReflectionSystem::GetOrCreateType<const List<unsigned int> &>("const List<unsigned int> &")));
Method& currentMethod = currentClass->AddMethod(Method("IndexBufferCreateInfo", ReflectionSystem::GetOrCreateType<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
unsigned int& arg0 = *(unsigned int*)inArguments[0];
static thread_local vk::BufferCreateInfo result = instance->UniformBufferCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("UniformBufferCreateInfo", ReflectionSystem::GetOrCreateType<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
unsigned int& arg0 = *(unsigned int*)inArguments[0];
static thread_local vk::BufferCreateInfo result = instance->StorageBufferCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("StorageBufferCreateInfo", ReflectionSystem::GetOrCreateType<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
unsigned int& arg0 = *(unsigned int*)inArguments[0];
static thread_local vk::BufferCreateInfo result = instance->ResizableStorageBufferCreateInfo(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("ResizableStorageBufferCreateInfo", ReflectionSystem::GetOrCreateType<vk::BufferCreateInfo>("vk::BufferCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
static thread_local vk::Buffer result = instance->GetAPIResource();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAPIResource", ReflectionSystem::GetOrCreateType<vk::Buffer>("vk::Buffer"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
VulkanBuffer * arg0 = (VulkanBuffer*)inArguments[0];
const unsigned int& arg1 = *(const unsigned int*)inArguments[1];
unsigned int& arg2 = *(unsigned int*)inArguments[2];
instance->CopyDataFromBuffer(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inStagingBuffer", ReflectionSystem::GetOrCreateType<VulkanBuffer *>("VulkanBuffer *")));
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inOffset", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("CopyDataFromBuffer", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
const void * arg0 = (const void*)inArguments[0];
const unsigned int& arg1 = *(const unsigned int*)inArguments[1];
unsigned int& arg2 = *(unsigned int*)inArguments[2];
instance->SetData(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inData", ReflectionSystem::GetOrCreateType<const void *>("const void *")));
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inOffset", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetData", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
static thread_local unsigned int result = instance->GetSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSize", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
const vk::BufferCreateInfo & result = instance->GetCreateInfo();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetCreateInfo", ReflectionSystem::GetOrCreateType<const vk::BufferCreateInfo &>("const vk::BufferCreateInfo &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
static thread_local VmaMemoryUsage result = instance->GetVmaMemoryUsage();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetVmaMemoryUsage", ReflectionSystem::GetOrCreateType<VmaMemoryUsage>("VmaMemoryUsage"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
static thread_local bool result = instance->IsMappable();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsMappable", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
static thread_local std::basic_string<char> result = instance->GetName();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetName", ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanBuffer* instance = static_cast<VulkanBuffer*>(inInstance);
void * result = instance->GetPtr();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPtr", ReflectionSystem::GetOrCreateType<void *>("void *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<RenderSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myDepthBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderTexture", -1, ReflectionSystem::GetOrCreateType<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsUsingGPUDrivenRendering", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGDRPipeline", -1, ReflectionSystem::GetOrCreateType<GDRPipeline>("GDRPipeline"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderPass", -1, ReflectionSystem::GetOrCreateType<vk::RenderPass>("vk::RenderPass"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCopyToSwapchainRenderPass", -1, ReflectionSystem::GetOrCreateType<vk::RenderPass>("vk::RenderPass"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVkFrameBuffer", -1, ReflectionSystem::GetOrCreateType<vk::Framebuffer>("vk::Framebuffer"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCopyToSwapchainFrameBuffers", -1, ReflectionSystem::GetOrCreateType<List<vk::Framebuffer>>("List<vk::Framebuffer>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myClearValues", -1, ReflectionSystem::GetOrCreateType<vk::ClearValue[2]>("vk::ClearValue[2]"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myResolvedRenderTexture", -1, ReflectionSystem::GetOrCreateType<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myResolvedDepthTexture", -1, ReflectionSystem::GetOrCreateType<VulkanImage>("VulkanImage"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
RenderSystem * result = instance->Get();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Get", ReflectionSystem::GetOrCreateType<RenderSystem *>("RenderSystem *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
instance->Init();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Init", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
vk::RenderPass & result = instance->GetRenderPass();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRenderPass", ReflectionSystem::GetOrCreateType<vk::RenderPass &>("vk::RenderPass &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
vk::RenderPass & result = instance->GetImGuiRenderPass();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImGuiRenderPass", ReflectionSystem::GetOrCreateType<vk::RenderPass &>("vk::RenderPass &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
VulkanImage * result = instance->GetRenderTexture();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRenderTexture", ReflectionSystem::GetOrCreateType<VulkanImage *>("VulkanImage *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
VulkanImage * result = instance->GetResolvedRenderTexture();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetResolvedRenderTexture", ReflectionSystem::GetOrCreateType<VulkanImage *>("VulkanImage *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
VulkanImage * result = instance->GetDepthTexture();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDepthTexture", ReflectionSystem::GetOrCreateType<VulkanImage *>("VulkanImage *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
instance->OnSwapChainResize();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnSwapChainResize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
static thread_local float result = instance->ReadDepthAtScreenPos(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inNormalizedScreenPos", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("ReadDepthAtScreenPos", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
VulkanCommandBuffer * result = instance->CreateUploadCommandBuffer_TS();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateUploadCommandBuffer_TS", ReflectionSystem::GetOrCreateType<VulkanCommandBuffer *>("VulkanCommandBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
VulkanCommandBuffer * arg0 = (VulkanCommandBuffer*)inArguments[0];
instance->QueueCommandBufferForUpload_TS(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("commandBuffer", ReflectionSystem::GetOrCreateType<VulkanCommandBuffer *>("VulkanCommandBuffer *")));
Method& currentMethod = currentClass->AddMethod(Method("QueueCommandBufferForUpload_TS", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
RenderSystem* instance = static_cast<RenderSystem*>(inInstance);
const GDRPipeline & result = instance->GetGDRPipeline();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGDRPipeline", ReflectionSystem::GetOrCreateType<const GDRPipeline &>("const GDRPipeline &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MathUtils>();
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IVulkanDynamicBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("OnBufferRecreated", -1, ReflectionSystem::GetOrCreateType<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanDescriptorSet>();
	{
		Field& currentField = currentClass->AddField(Field("myUsesSharedLayout", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLayout", -1, ReflectionSystem::GetOrCreateType<vk::DescriptorSetLayout>("vk::DescriptorSetLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySet", -1, ReflectionSystem::GetOrCreateType<vk::DescriptorSet>("vk::DescriptorSet"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
static thread_local vk::DescriptorSetLayout result = instance->GetLayout();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetLayout", ReflectionSystem::GetOrCreateType<vk::DescriptorSetLayout>("vk::DescriptorSetLayout"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
static thread_local vk::DescriptorSet result = instance->GetSet();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSet", ReflectionSystem::GetOrCreateType<vk::DescriptorSet>("vk::DescriptorSet"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
const VulkanBuffer * arg0 = (const VulkanBuffer*)inArguments[0];
vk::Flags<vk::ShaderStageFlagBits>& arg1 = *(vk::Flags<vk::ShaderStageFlagBits>*)inArguments[1];
unsigned int& arg2 = *(unsigned int*)inArguments[2];
vk::DescriptorType& arg3 = *(vk::DescriptorType*)inArguments[3];
instance->BindBuffer(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBuffer", ReflectionSystem::GetOrCreateType<const VulkanBuffer *>("const VulkanBuffer *")));
arguments.Add(MethodArgument("inShaderStages", ReflectionSystem::GetOrCreateType<vk::Flags<vk::ShaderStageFlagBits>>("vk::Flags<vk::ShaderStageFlagBits>")));
arguments.Add(MethodArgument("inBindingIndex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
arguments.Add(MethodArgument("inDescriptorType", ReflectionSystem::GetOrCreateType<vk::DescriptorType>("vk::DescriptorType")));
Method& currentMethod = currentClass->AddMethod(Method("BindBuffer", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
const ResizableBuffer * arg0 = (const ResizableBuffer*)inArguments[0];
vk::Flags<vk::ShaderStageFlagBits>& arg1 = *(vk::Flags<vk::ShaderStageFlagBits>*)inArguments[1];
unsigned int& arg2 = *(unsigned int*)inArguments[2];
vk::DescriptorType& arg3 = *(vk::DescriptorType*)inArguments[3];
instance->BindBuffer(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBuffer", ReflectionSystem::GetOrCreateType<const ResizableBuffer *>("const ResizableBuffer *")));
arguments.Add(MethodArgument("inShaderStages", ReflectionSystem::GetOrCreateType<vk::Flags<vk::ShaderStageFlagBits>>("vk::Flags<vk::ShaderStageFlagBits>")));
arguments.Add(MethodArgument("inBindingIndex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
arguments.Add(MethodArgument("inDescriptorType", ReflectionSystem::GetOrCreateType<vk::DescriptorType>("vk::DescriptorType")));
Method& currentMethod = currentClass->AddMethod(Method("BindBuffer", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
const IGPUList * arg0 = (const IGPUList*)inArguments[0];
vk::Flags<vk::ShaderStageFlagBits>& arg1 = *(vk::Flags<vk::ShaderStageFlagBits>*)inArguments[1];
unsigned int& arg2 = *(unsigned int*)inArguments[2];
vk::DescriptorType& arg3 = *(vk::DescriptorType*)inArguments[3];
instance->BindBuffer(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBuffer", ReflectionSystem::GetOrCreateType<const IGPUList *>("const IGPUList *")));
arguments.Add(MethodArgument("inShaderStages", ReflectionSystem::GetOrCreateType<vk::Flags<vk::ShaderStageFlagBits>>("vk::Flags<vk::ShaderStageFlagBits>")));
arguments.Add(MethodArgument("inBindingIndex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
arguments.Add(MethodArgument("inDescriptorType", ReflectionSystem::GetOrCreateType<vk::DescriptorType>("vk::DescriptorType")));
Method& currentMethod = currentClass->AddMethod(Method("BindBuffer", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
const VulkanImage * arg0 = (const VulkanImage*)inArguments[0];
const vk::Sampler& arg1 = *(const vk::Sampler*)inArguments[1];
const unsigned int& arg2 = *(const unsigned int*)inArguments[2];
const vk::Flags<vk::ShaderStageFlagBits>& arg3 = *(const vk::Flags<vk::ShaderStageFlagBits>*)inArguments[3];
const vk::ImageLayout& arg4 = *(const vk::ImageLayout*)inArguments[4];
instance->BindImage(arg0, arg1, arg2, arg3, arg4);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inImage", ReflectionSystem::GetOrCreateType<const VulkanImage *>("const VulkanImage *")));
arguments.Add(MethodArgument("inSampler", ReflectionSystem::GetOrCreateType<const vk::Sampler>("const vk::Sampler")));
arguments.Add(MethodArgument("inBinding", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inShaderFlags", ReflectionSystem::GetOrCreateType<const vk::Flags<vk::ShaderStageFlagBits>>("const vk::Flags<vk::ShaderStageFlagBits>")));
arguments.Add(MethodArgument("inImageLayout", ReflectionSystem::GetOrCreateType<const vk::ImageLayout>("const vk::ImageLayout")));
Method& currentMethod = currentClass->AddMethod(Method("BindImage", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
instance->Build();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Build", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDescriptorSet* instance = static_cast<VulkanDescriptorSet*>(inInstance);
instance->Rebuild();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Rebuild", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<DebugPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", -1, ReflectionSystem::GetOrCreateType<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", -1, ReflectionSystem::GetOrCreateType<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDescriptorSet", -1, ReflectionSystem::GetOrCreateType<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanBuffer>("VulkanBuffer"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DebugPipeline* instance = static_cast<DebugPipeline*>(inInstance);
const vk::CommandBuffer& arg0 = *(const vk::CommandBuffer*)inArguments[0];
instance->AddDrawCommands(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<const vk::CommandBuffer>("const vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("AddDrawCommands", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<FullscreenPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", -1, ReflectionSystem::GetOrCreateType<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", -1, ReflectionSystem::GetOrCreateType<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderPass", -1, ReflectionSystem::GetOrCreateType<vk::RenderPass>("vk::RenderPass"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorSet", -1, ReflectionSystem::GetOrCreateType<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
FullscreenPipeline* instance = static_cast<FullscreenPipeline*>(inInstance);
const vk::CommandBuffer& arg0 = *(const vk::CommandBuffer*)inArguments[0];
instance->AddFullscreenPass(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<const vk::CommandBuffer>("const vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("AddFullscreenPass", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<GDRPipeline>();
	{
		Field& currentField = currentClass->AddField(Field("myIndirectCommandsBuffer", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndirectCommandsBufferNoDepth", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCountBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCountNoDepthBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPerDrawDataBuffer", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPerDrawDataNoDepthBuffer", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirectionalLightBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenderPasses", -1, ReflectionSystem::GetOrCreateType<List<IRenderPass *>>("List<IRenderPass *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCubemap", -1, ReflectionSystem::GetOrCreateType<TextureCube>("TextureCube"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDescriptorSet", -1, ReflectionSystem::GetOrCreateType<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameNoDepthDescriptorSet", -1, ReflectionSystem::GetOrCreateType<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", -1, ReflectionSystem::GetOrCreateType<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", -1, ReflectionSystem::GetOrCreateType<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPreDepthPipelineLayout", -1, ReflectionSystem::GetOrCreateType<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPreDepthPipeline", -1, ReflectionSystem::GetOrCreateType<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirtyTransforms", -1, ReflectionSystem::GetOrCreateType<List<TransformComponent *>>("List<TransformComponent *>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GDRPipeline* instance = static_cast<GDRPipeline*>(inInstance);
GDRPipeline * result = instance->Get();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Get", ReflectionSystem::GetOrCreateType<GDRPipeline *>("GDRPipeline *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GDRPipeline* instance = static_cast<GDRPipeline*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->ExecutePasses(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("ExecutePasses", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GDRPipeline* instance = static_cast<GDRPipeline*>(inInstance);
VulkanBuffer * result = instance->GetCountBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetCountBuffer", ReflectionSystem::GetOrCreateType<VulkanBuffer *>("VulkanBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GDRPipeline* instance = static_cast<GDRPipeline*>(inInstance);
VulkanBuffer * result = instance->GetIndirectBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIndirectBuffer", ReflectionSystem::GetOrCreateType<VulkanBuffer *>("VulkanBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GDRPipeline* instance = static_cast<GDRPipeline*>(inInstance);
ResizableBuffer * result = instance->GetPerDrawDataBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPerDrawDataBuffer", ReflectionSystem::GetOrCreateType<ResizableBuffer *>("ResizableBuffer *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<GDRPipeline::DirectionalLightBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("myColor", -1, ReflectionSystem::GetOrCreateType<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirection", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("padding", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLightView", -1, ReflectionSystem::GetOrCreateType<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLightProjection", -1, ReflectionSystem::GetOrCreateType<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<GDRPipeline::FrameData>();
	{
		Field& currentField = currentClass->AddField(Field("myToView", -1, ReflectionSystem::GetOrCreateType<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myProjection", -1, ReflectionSystem::GetOrCreateType<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCameraPosition", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCubemapIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IndexBufferHandle>();
	{
		Field& currentField = currentClass->AddField(Field("myIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferHandle* instance = static_cast<IndexBufferHandle*>(inInstance);
static thread_local unsigned int result = instance->GetIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIndex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MeshData>();
	{
		Field& currentField = currentClass->AddField(Field("myBoundingSphereModelSpace", -1, ReflectionSystem::GetOrCreateType<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexDataIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VertexBufferData>();
	{
		Field& currentField = currentClass->AddField(Field("myOffset", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IndexBufferData>();
	{
		Field& currentField = currentClass->AddField(Field("myOffset", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCount", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MeshInstanceData>();
	{
		Field& currentField = currentClass->AddField(Field("myToWorld", -1, ReflectionSystem::GetOrCreateType<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMeshIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDepthWriteEnabled", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PerDrawData>();
	{
		Field& currentField = currentClass->AddField(Field("myToWorld", -1, ReflectionSystem::GetOrCreateType<glm::mat<4, 4, float>>("glm::mat<4, 4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("padding", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PointLightData>();
	{
		Field& currentField = currentClass->AddField(Field("myColor", -1, ReflectionSystem::GetOrCreateType<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPosition", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRange", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIntensity", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IndexBufferSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myUsedBufferSize", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCurrentIndexOffset", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexBuffers", -1, ReflectionSystem::GetOrCreateType<List<IndexBufferHandle *>>("List<IndexBufferHandle *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseIndexDataBuffer", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseIndexData_CPURepresentation", -1, ReflectionSystem::GetOrCreateType<List<IndexBufferData>>("List<IndexBufferData>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFreeSparseIndices", -1, ReflectionSystem::GetOrCreateType<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferSystem* instance = static_cast<IndexBufferSystem*>(inInstance);
const List<unsigned int> & arg0 = *(const List<unsigned int>*)inArguments[0];
IndexBufferHandle * result = instance->UploadIndexBuffer(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inIndices", ReflectionSystem::GetOrCreateType<const List<unsigned int> &>("const List<unsigned int> &")));
Method& currentMethod = currentClass->AddMethod(Method("UploadIndexBuffer", ReflectionSystem::GetOrCreateType<IndexBufferHandle *>("IndexBufferHandle *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferSystem* instance = static_cast<IndexBufferSystem*>(inInstance);
VulkanBuffer * arg0 = (VulkanBuffer*)inArguments[0];
const unsigned int& arg1 = *(const unsigned int*)inArguments[1];
IndexBufferHandle * result = instance->UploadIndexBuffer(arg0, arg1);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inStagingBuffer", ReflectionSystem::GetOrCreateType<VulkanBuffer *>("VulkanBuffer *")));
arguments.Add(MethodArgument("inIndexCount", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("UploadIndexBuffer", ReflectionSystem::GetOrCreateType<IndexBufferHandle *>("IndexBufferHandle *"), invoker, arguments));
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
arguments.Add(MethodArgument("inHandle", ReflectionSystem::GetOrCreateType<IndexBufferHandle *>("IndexBufferHandle *")));
Method& currentMethod = currentClass->AddMethod(Method("GetIndexBufferDataFromIndexHandle", ReflectionSystem::GetOrCreateType<const IndexBufferData &>("const IndexBufferData &"), invoker, arguments));
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
arguments.Add(MethodArgument("inBuffer", ReflectionSystem::GetOrCreateType<const IndexBufferHandle *>("const IndexBufferHandle *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveIndexBuffer", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferSystem* instance = static_cast<IndexBufferSystem*>(inInstance);
const ResizableBuffer * result = instance->GetGlobalIndexBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGlobalIndexBuffer", ReflectionSystem::GetOrCreateType<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndexBufferSystem* instance = static_cast<IndexBufferSystem*>(inInstance);
const ResizableBuffer * result = instance->GetGlobalSparseIndexDataBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGlobalSparseIndexDataBuffer", ReflectionSystem::GetOrCreateType<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Mesh>();
	{
		Field& currentField = currentClass->AddField(Field("myVertexBuffer", -1, ReflectionSystem::GetOrCreateType<VertexBufferHandle>("VertexBufferHandle"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIndexBuffer", -1, ReflectionSystem::GetOrCreateType<IndexBufferHandle>("IndexBufferHandle"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySphereBounds", -1, ReflectionSystem::GetOrCreateType<glm::vec<4, float>>("glm::vec<4, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHandle", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAlbedoPath", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNormalPath", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaterialPath", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
VertexBufferHandle * result = instance->GetVertexBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetVertexBuffer", ReflectionSystem::GetOrCreateType<VertexBufferHandle *>("VertexBufferHandle *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
IndexBufferHandle * result = instance->GetIndexBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIndexBuffer", ReflectionSystem::GetOrCreateType<IndexBufferHandle *>("IndexBufferHandle *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
const glm::vec<4, float> & result = instance->GetSphereBounds();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSphereBounds", ReflectionSystem::GetOrCreateType<const glm::vec<4, float> &>("const glm::vec<4, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
static thread_local unsigned int result = instance->GetHandle();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHandle", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
const std::basic_string<char> & result = instance->GetAlbedoPath();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAlbedoPath", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
const std::basic_string<char> & result = instance->GetNormalPath();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNormalPath", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Mesh* instance = static_cast<Mesh*>(inInstance);
const std::basic_string<char> & result = instance->GetMaterialPath();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMaterialPath", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MeshSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myMeshes", -1, ReflectionSystem::GetOrCreateType<List<Mesh *>>("List<Mesh *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumObjects", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<System>());
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
arguments.Add(MethodArgument("inVertexBuffer", ReflectionSystem::GetOrCreateType<VertexBufferHandle *>("VertexBufferHandle *")));
arguments.Add(MethodArgument("inIndexBuffer", ReflectionSystem::GetOrCreateType<IndexBufferHandle *>("IndexBufferHandle *")));
arguments.Add(MethodArgument("inBoundingSphere", ReflectionSystem::GetOrCreateType<const glm::vec<4, float> &>("const glm::vec<4, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("UploadMesh", ReflectionSystem::GetOrCreateType<Mesh *>("Mesh *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshSystem* instance = static_cast<MeshSystem*>(inInstance);
Mesh * arg0 = (Mesh*)inArguments[0];
instance->RemoveMesh(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMesh", ReflectionSystem::GetOrCreateType<Mesh *>("Mesh *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveMesh", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshSystem* instance = static_cast<MeshSystem*>(inInstance);
ResizableBuffer * result = instance->GetBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBuffer", ReflectionSystem::GetOrCreateType<ResizableBuffer *>("ResizableBuffer *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MeshUtils>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MeshUtils* instance = static_cast<MeshUtils*>(inInstance);
const List<Vertex> & arg0 = *(const List<Vertex>*)inArguments[0];
static thread_local glm::vec<4, float> result = instance->CalculateSphereBounds(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inVertices", ReflectionSystem::GetOrCreateType<const List<Vertex> &>("const List<Vertex> &")));
Method& currentMethod = currentClass->AddMethod(Method("CalculateSphereBounds", ReflectionSystem::GetOrCreateType<glm::vec<4, float>>("glm::vec<4, float>"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VertexBufferHandle>();
	{
		Field& currentField = currentClass->AddField(Field("myIndex", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferHandle* instance = static_cast<VertexBufferHandle*>(inInstance);
static thread_local unsigned int result = instance->GetIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIndex", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ComputePass>();
	{
		Field& currentField = currentClass->AddField(Field("myPassName", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorSet", -1, ReflectionSystem::GetOrCreateType<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", -1, ReflectionSystem::GetOrCreateType<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", -1, ReflectionSystem::GetOrCreateType<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IRenderPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComputePass* instance = static_cast<ComputePass*>(inInstance);
instance->CreateResources();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateResources", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComputePass* instance = static_cast<ComputePass*>(inInstance);
instance->DestroyResources();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DestroyResources", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComputePass* instance = static_cast<ComputePass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->Execute(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("Execute", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ComputePass* instance = static_cast<ComputePass*>(inInstance);
instance->OnShaderRecompiled();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnShaderRecompiled", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IndirectCullPass>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<ComputePass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndirectCullPass* instance = static_cast<IndirectCullPass*>(inInstance);
instance->SetupDescriptors();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupDescriptors", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndirectCullPass* instance = static_cast<IndirectCullPass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->DispatchCall(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("DispatchCall", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<World>();
	{
		Field& currentField = currentClass->AddField(Field("mySystemManager", offsetof(World, mySystemManager), ReflectionSystem::GetOrCreateType<UniquePtr<SystemManager<WorldSystem>>>("UniquePtr<SystemManager<WorldSystem>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myActors", offsetof(World, myActors), ReflectionSystem::GetOrCreateType<List<UniquePtr<Actor>>>("List<UniquePtr<Actor>>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myActorsToDelete", offsetof(World, myActorsToDelete), ReflectionSystem::GetOrCreateType<List<Actor *>>("List<Actor *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCachedDirectionalLightActor", offsetof(World, myCachedDirectionalLightActor), ReflectionSystem::GetOrCreateType<DirectionalLightActor>("DirectionalLightActor"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMainCamera", offsetof(World, myMainCamera), ReflectionSystem::GetOrCreateType<CameraComponent>("CameraComponent"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Asset>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->Init();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Init", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->PostPropertiesSerialized();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("PostPropertiesSerialized", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
static thread_local List<std::basic_string<char>> result = instance->GetAssetExtensions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAssetExtensions", ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->DoTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DoTick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->Destroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Destroy", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const glm::vec<3, float> & arg1 = *(const glm::vec<3, float>*)inArguments[1];
RaycastHit & arg2 = *(RaycastHit*)inArguments[2];
const float& arg3 = *(const float*)inArguments[3];
const unsigned int& arg4 = *(const unsigned int*)inArguments[4];
bool& arg5 = *(bool*)inArguments[5];
static thread_local bool result = instance->Raycast(arg0, arg1, arg2, arg3, arg4, arg5);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOrigin", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inDirection", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("outHit", ReflectionSystem::GetOrCreateType<RaycastHit &>("RaycastHit &")));
arguments.Add(MethodArgument("inMaxDistance", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inExcludedTags", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inIgnoreTriggers", ReflectionSystem::GetOrCreateType<bool>("bool")));
Method& currentMethod = currentClass->AddMethod(Method("Raycast", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const glm::vec<3, float> & arg1 = *(const glm::vec<3, float>*)inArguments[1];
List<RaycastHit> & arg2 = *(List<RaycastHit>*)inArguments[2];
const float& arg3 = *(const float*)inArguments[3];
const unsigned int& arg4 = *(const unsigned int*)inArguments[4];
bool& arg5 = *(bool*)inArguments[5];
static thread_local bool result = instance->RaycastAll(arg0, arg1, arg2, arg3, arg4, arg5);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inOrigin", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inDirection", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("outHits", ReflectionSystem::GetOrCreateType<List<RaycastHit> &>("List<RaycastHit> &")));
arguments.Add(MethodArgument("inMaxDistance", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inExcludedTags", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inIgnoreTriggers", ReflectionSystem::GetOrCreateType<bool>("bool")));
Method& currentMethod = currentClass->AddMethod(Method("RaycastAll", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
const std::basic_string<char> & arg1 = *(const std::basic_string<char>*)inArguments[1];
Actor * result = instance->SpawnActor(arg0, arg1);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inActorType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("SpawnActor", ReflectionSystem::GetOrCreateType<Actor *>("Actor *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->RemoveActor(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inActor", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveActor", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->RemoveAllActors();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RemoveAllActors", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
const List<UniquePtr<Actor>> & result = instance->GetAllActors();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAllActors", ReflectionSystem::GetOrCreateType<const List<UniquePtr<Actor>> &>("const List<UniquePtr<Actor>> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
Actor * arg1 = (Actor*)inArguments[1];
instance->OnTriggerEnter(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFirst", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerEnter", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
Actor * arg1 = (Actor*)inArguments[1];
instance->OnTrigger(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFirst", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTrigger", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
Actor * arg1 = (Actor*)inArguments[1];
instance->OnTriggerExit(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFirst", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnTriggerExit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
Actor * arg1 = (Actor*)inArguments[1];
instance->OnCollisionEnter(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFirst", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionEnter", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
Actor * arg1 = (Actor*)inArguments[1];
instance->OnCollision(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFirst", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollision", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
Actor * arg1 = (Actor*)inArguments[1];
instance->OnCollisionExit(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inFirst", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
arguments.Add(MethodArgument("inOther", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("OnCollisionExit", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
DirectionalLightComponent * result = instance->GetDirectionalLight();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDirectionalLight", ReflectionSystem::GetOrCreateType<DirectionalLightComponent *>("DirectionalLightComponent *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
CameraComponent * arg0 = (CameraComponent*)inArguments[0];
instance->SetMainCamera(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCamera", ReflectionSystem::GetOrCreateType<CameraComponent *>("CameraComponent *")));
Method& currentMethod = currentClass->AddMethod(Method("SetMainCamera", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
CameraComponent * result = instance->GetMainCamera();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMainCamera", ReflectionSystem::GetOrCreateType<CameraComponent *>("CameraComponent *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->CreateWorldSystems();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateWorldSystems", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
instance->TickActorDeletes();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickActorDeletes", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
World* instance = static_cast<World*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
const std::basic_string<char> & arg1 = *(const std::basic_string<char>*)inArguments[1];
instance->InitActor(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inActor", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
arguments.Add(MethodArgument("inName", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
Method& currentMethod = currentClass->AddMethod(Method("InitActor", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IndirectPrePass>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<ComputePass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndirectPrePass* instance = static_cast<IndirectPrePass*>(inInstance);
instance->SetupDescriptors();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupDescriptors", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
IndirectPrePass* instance = static_cast<IndirectPrePass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->DispatchCall(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("DispatchCall", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ShaderDatabase>();
	{
		Field& currentField = currentClass->AddField(Field("m_shaderBinaries", -1, ReflectionSystem::GetOrCreateType<std::map<GFSDK_Aftermath_ShaderBinaryHash, std::vector<unsigned char>>>("std::map<GFSDK_Aftermath_ShaderBinaryHash, std::vector<unsigned char>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_shaderBinariesWithDebugInfo", -1, ReflectionSystem::GetOrCreateType<std::map<GFSDK_Aftermath_ShaderDebugName, std::vector<unsigned char>>>("std::map<GFSDK_Aftermath_ShaderDebugName, std::vector<unsigned char>>"), false, false));
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
arguments.Add(MethodArgument("shaderHash", ReflectionSystem::GetOrCreateType<const GFSDK_Aftermath_ShaderBinaryHash &>("const GFSDK_Aftermath_ShaderBinaryHash &")));
arguments.Add(MethodArgument("shader", ReflectionSystem::GetOrCreateType<std::vector<unsigned char> &>("std::vector<unsigned char> &")));
Method& currentMethod = currentClass->AddMethod(Method("FindShaderBinary", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
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
arguments.Add(MethodArgument("shaderDebugName", ReflectionSystem::GetOrCreateType<const GFSDK_Aftermath_ShaderDebugName &>("const GFSDK_Aftermath_ShaderDebugName &")));
arguments.Add(MethodArgument("shader", ReflectionSystem::GetOrCreateType<std::vector<unsigned char> &>("std::vector<unsigned char> &")));
Method& currentMethod = currentClass->AddMethod(Method("FindShaderBinaryWithDebugData", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<NvidiaAftermathTracker>();
	{
		Field& currentField = currentClass->AddField(Field("m_initialized", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_mutex", -1, ReflectionSystem::GetOrCreateType<std::mutex>("std::mutex"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_shaderDebugInfo", -1, ReflectionSystem::GetOrCreateType<std::map<GFSDK_Aftermath_ShaderDebugInfoIdentifier, std::vector<unsigned char>>>("std::map<GFSDK_Aftermath_ShaderDebugInfoIdentifier, std::vector<unsigned char>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_shaderDatabase", -1, ReflectionSystem::GetOrCreateType<ShaderDatabase>("ShaderDatabase"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_markerMap", -1, ReflectionSystem::GetOrCreateType<const std::array<std::map<unsigned long long, std::basic_string<char>>, 4>>("const std::array<std::map<unsigned long long, std::basic_string<char>>, 4>"), false, true));
	}
	{
		Field& currentField = currentClass->AddField(Field("m_AftermathFlags", -1, ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
NvidiaAftermathTracker* instance = static_cast<NvidiaAftermathTracker*>(inInstance);
instance->Initialize();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Initialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
NvidiaAftermathTracker* instance = static_cast<NvidiaAftermathTracker*>(inInstance);
static thread_local unsigned int result = instance->GetAftermathFlags();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAftermathFlags", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanContext>();
	{
		Field& currentField = currentClass->AddField(Field("myVulkanInstance", -1, ReflectionSystem::GetOrCreateType<vk::Instance>("vk::Instance"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDebugMessenger", -1, ReflectionSystem::GetOrCreateType<vk::DebugUtilsMessengerEXT>("vk::DebugUtilsMessengerEXT"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineCache", -1, ReflectionSystem::GetOrCreateType<vk::PipelineCache>("vk::PipelineCache"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPhysicalDevice", -1, ReflectionSystem::GetOrCreateType<UniquePtr<VulkanPhysicalDevice>>("UniquePtr<VulkanPhysicalDevice>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDevice", -1, ReflectionSystem::GetOrCreateType<UniquePtr<VulkanDevice>>("UniquePtr<VulkanDevice>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAllocator", -1, ReflectionSystem::GetOrCreateType<UniquePtr<VulkanAllocator>>("UniquePtr<VulkanAllocator>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySwapChain", -1, ReflectionSystem::GetOrCreateType<UniquePtr<VulkanSwapChain>>("UniquePtr<VulkanSwapChain>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("markerMap", -1, ReflectionSystem::GetOrCreateType<std::array<std::map<unsigned long long, std::basic_string<char>>, 4>>("std::array<std::map<unsigned long long, std::basic_string<char>>, 4>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNvidiaAftermathDebugger", -1, ReflectionSystem::GetOrCreateType<UniquePtr<NvidiaAftermathTracker>>("UniquePtr<NvidiaAftermathTracker>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorPool", -1, ReflectionSystem::GetOrCreateType<vk::DescriptorPool>("vk::DescriptorPool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myExtensions", -1, ReflectionSystem::GetOrCreateType<const List<const char *>>("const List<const char *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLayers", -1, ReflectionSystem::GetOrCreateType<const List<const char *>>("const List<const char *>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
vk::Instance & result = instance->GetInstance();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetInstance", ReflectionSystem::GetOrCreateType<vk::Instance &>("vk::Instance &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
VulkanPhysicalDevice & result = instance->GetPhysicalDevice();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPhysicalDevice", ReflectionSystem::GetOrCreateType<VulkanPhysicalDevice &>("VulkanPhysicalDevice &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
VulkanDevice & result = instance->GetDevice();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDevice", ReflectionSystem::GetOrCreateType<VulkanDevice &>("VulkanDevice &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
VulkanSwapChain & result = instance->GetSwapChain();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSwapChain", ReflectionSystem::GetOrCreateType<VulkanSwapChain &>("VulkanSwapChain &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
vk::PipelineCache & result = instance->GetPipelineCache();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPipelineCache", ReflectionSystem::GetOrCreateType<vk::PipelineCache &>("vk::PipelineCache &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
VulkanAllocator & result = instance->GetAllocator();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAllocator", ReflectionSystem::GetOrCreateType<VulkanAllocator &>("VulkanAllocator &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
NvidiaAftermathTracker * result = instance->GetAftermathTracker();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAftermathTracker", ReflectionSystem::GetOrCreateType<NvidiaAftermathTracker *>("NvidiaAftermathTracker *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
static thread_local glm::vec<2, float> result = instance->GetRenderResolution();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetRenderResolution", ReflectionSystem::GetOrCreateType<glm::vec<2, float>>("glm::vec<2, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
instance->BeginFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("BeginFrame", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
instance->EndFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("EndFrame", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanContext* instance = static_cast<VulkanContext*>(inInstance);
static thread_local vk::DescriptorPool result = instance->GetDescriptorPool();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDescriptorPool", ReflectionSystem::GetOrCreateType<vk::DescriptorPool>("vk::DescriptorPool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanDevice>();
	{
		Field& currentField = currentClass->AddField(Field("myPhysicalDevice", -1, ReflectionSystem::GetOrCreateType<const VulkanPhysicalDevice>("const VulkanPhysicalDevice"), false, true));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDevice", -1, ReflectionSystem::GetOrCreateType<vk::Device>("vk::Device"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGraphicsQueue", -1, ReflectionSystem::GetOrCreateType<vk::Queue>("vk::Queue"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComputeQueue", -1, ReflectionSystem::GetOrCreateType<vk::Queue>("vk::Queue"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTransferQueue", -1, ReflectionSystem::GetOrCreateType<vk::Queue>("vk::Queue"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
vk::Device * result = instance->operator->();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("operator->", ReflectionSystem::GetOrCreateType<vk::Device *>("vk::Device *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const vk::Device * result = instance->operator->();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("operator->", ReflectionSystem::GetOrCreateType<const vk::Device *>("const vk::Device *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const vk::Queue & result = instance->GetGraphicsQueue();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGraphicsQueue", ReflectionSystem::GetOrCreateType<const vk::Queue &>("const vk::Queue &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const vk::Queue & result = instance->GetComputeQueue();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetComputeQueue", ReflectionSystem::GetOrCreateType<const vk::Queue &>("const vk::Queue &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const vk::Queue & result = instance->GetTransferQueue();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTransferQueue", ReflectionSystem::GetOrCreateType<const vk::Queue &>("const vk::Queue &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const vk::Queue & result = instance->GetPresentQueue();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPresentQueue", ReflectionSystem::GetOrCreateType<const vk::Queue &>("const vk::Queue &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
static thread_local vk::Device result = instance->GetDevice();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDevice", ReflectionSystem::GetOrCreateType<vk::Device>("vk::Device"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const bool& arg0 = *(const bool*)inArguments[0];
const bool& arg1 = *(const bool*)inArguments[1];
VulkanCommandBuffer * result = instance->CreateCommandBuffer(arg0, arg1);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inBegin", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
arguments.Add(MethodArgument("isSecondaryBuffer", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("CreateCommandBuffer", ReflectionSystem::GetOrCreateType<VulkanCommandBuffer *>("VulkanCommandBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
VulkanCommandBuffer * arg0 = (VulkanCommandBuffer*)inArguments[0];
instance->FlushCommandBuffer(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<VulkanCommandBuffer *>("VulkanCommandBuffer *")));
Method& currentMethod = currentClass->AddMethod(Method("FlushCommandBuffer", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanDevice* instance = static_cast<VulkanDevice*>(inInstance);
const List<VulkanCommandBuffer *> & arg0 = *(const List<VulkanCommandBuffer *>*)inArguments[0];
instance->FlushSecondaryCommandBuffers(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffers", ReflectionSystem::GetOrCreateType<const List<VulkanCommandBuffer *> &>("const List<VulkanCommandBuffer *> &")));
Method& currentMethod = currentClass->AddMethod(Method("FlushSecondaryCommandBuffers", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanUtils>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanUtils* instance = static_cast<VulkanUtils*>(inInstance);
instance->CreateSamplers();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateSamplers", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanUtils* instance = static_cast<VulkanUtils*>(inInstance);
const SamplerMode& arg0 = *(const SamplerMode*)inArguments[0];
static thread_local vk::Sampler result = instance->GetSampler(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inMode", ReflectionSystem::GetOrCreateType<const SamplerMode>("const SamplerMode")));
Method& currentMethod = currentClass->AddMethod(Method("GetSampler", ReflectionSystem::GetOrCreateType<vk::Sampler>("vk::Sampler"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanUtils* instance = static_cast<VulkanUtils*>(inInstance);
instance->DestroySamplers();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DestroySamplers", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PointLightSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myPointLightBuffer", -1, ReflectionSystem::GetOrCreateType<GPUSparseDenseBuffer<PointLightData>>("GPUSparseDenseBuffer<PointLightData>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightSystem* instance = static_cast<PointLightSystem*>(inInstance);
const PointLightData & arg0 = *(const PointLightData*)inArguments[0];
static thread_local unsigned int result = instance->AddLight(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPointLightData", ReflectionSystem::GetOrCreateType<const PointLightData &>("const PointLightData &")));
Method& currentMethod = currentClass->AddMethod(Method("AddLight", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightSystem* instance = static_cast<PointLightSystem*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
const PointLightData & arg1 = *(const PointLightData*)inArguments[1];
instance->UpdateLight(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inLightInstance", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inPointLightData", ReflectionSystem::GetOrCreateType<const PointLightData &>("const PointLightData &")));
Method& currentMethod = currentClass->AddMethod(Method("UpdateLight", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightSystem* instance = static_cast<PointLightSystem*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
instance->RemoveLight(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inLightInstance", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveLight", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PointLightSystem* instance = static_cast<PointLightSystem*>(inInstance);
const IGPUList * result = instance->GetBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBuffer", ReflectionSystem::GetOrCreateType<const IGPUList *>("const IGPUList *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanImage>();
	{
		Field& currentField = currentClass->AddField(Field("myAllocation", -1, ReflectionSystem::GetOrCreateType<VmaAllocation>("VmaAllocation"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myImage", -1, ReflectionSystem::GetOrCreateType<vk::Image>("vk::Image"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myView", -1, ReflectionSystem::GetOrCreateType<vk::ImageView>("vk::ImageView"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFormat", -1, ReflectionSystem::GetOrCreateType<vk::Format>("vk::Format"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySize", -1, ReflectionSystem::GetOrCreateType<glm::vec<2, float>>("glm::vec<2, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumMipLevels", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNumMSAASamples", -1, ReflectionSystem::GetOrCreateType<vk::SampleCountFlagBits>("vk::SampleCountFlagBits"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
const glm::vec<2, float> & arg0 = *(const glm::vec<2, float>*)inArguments[0];
vk::SampleCountFlagBits& arg1 = *(vk::SampleCountFlagBits*)inArguments[1];
static thread_local vk::ImageCreateInfo result = instance->DepthCreateInfo(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inResolution", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
arguments.Add(MethodArgument("inSampleCount", ReflectionSystem::GetOrCreateType<vk::SampleCountFlagBits>("vk::SampleCountFlagBits")));
Method& currentMethod = currentClass->AddMethod(Method("DepthCreateInfo", ReflectionSystem::GetOrCreateType<vk::ImageCreateInfo>("vk::ImageCreateInfo"), invoker, arguments));
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
arguments.Add(MethodArgument("inResolution", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("ShadowMapCreateInfo", ReflectionSystem::GetOrCreateType<vk::ImageCreateInfo>("vk::ImageCreateInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local vk::Image result = instance->GetAPIResource();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAPIResource", ReflectionSystem::GetOrCreateType<vk::Image>("vk::Image"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local vk::ImageView result = instance->GetImageView();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetImageView", ReflectionSystem::GetOrCreateType<vk::ImageView>("vk::ImageView"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local vk::Format result = instance->GetFormat();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetFormat", ReflectionSystem::GetOrCreateType<vk::Format>("vk::Format"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local bool result = instance->HasImageView();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("HasImageView", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
vk::ImageViewType& arg0 = *(vk::ImageViewType*)inArguments[0];
instance->CreateView(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inViewType", ReflectionSystem::GetOrCreateType<vk::ImageViewType>("vk::ImageViewType")));
Method& currentMethod = currentClass->AddMethod(Method("CreateView", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
vk::ImageViewType& arg0 = *(vk::ImageViewType*)inArguments[0];
vk::ImageSubresourceRange& arg1 = *(vk::ImageSubresourceRange*)inArguments[1];
instance->CreateView(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inViewType", ReflectionSystem::GetOrCreateType<vk::ImageViewType>("vk::ImageViewType")));
arguments.Add(MethodArgument("inRange", ReflectionSystem::GetOrCreateType<vk::ImageSubresourceRange>("vk::ImageSubresourceRange")));
Method& currentMethod = currentClass->AddMethod(Method("CreateView", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
instance->CreateDepthView();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateDepthView", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
const glm::vec<2, float> & result = instance->GetSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSize", ReflectionSystem::GetOrCreateType<const glm::vec<2, float> &>("const glm::vec<2, float> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local unsigned int result = instance->GetNumMipLevels();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNumMipLevels", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImage* instance = static_cast<VulkanImage*>(inInstance);
static thread_local vk::SampleCountFlagBits result = instance->GetNumMSAASamples();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNumMSAASamples", ReflectionSystem::GetOrCreateType<vk::SampleCountFlagBits>("vk::SampleCountFlagBits"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<TextureAttachmentDesc>();
	{
		Field& currentField = currentClass->AddField(Field("myTexture", -1, ReflectionSystem::GetOrCreateType<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myLoadOp", -1, ReflectionSystem::GetOrCreateType<vk::AttachmentLoadOp>("vk::AttachmentLoadOp"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStoreOp", -1, ReflectionSystem::GetOrCreateType<vk::AttachmentStoreOp>("vk::AttachmentStoreOp"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myInitialLayout", -1, ReflectionSystem::GetOrCreateType<vk::ImageLayout>("vk::ImageLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFinalLayout", -1, ReflectionSystem::GetOrCreateType<vk::ImageLayout>("vk::ImageLayout"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<GraphicsPass>();
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorSet", -1, ReflectionSystem::GetOrCreateType<VulkanDescriptorSet>("VulkanDescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipelineLayout", -1, ReflectionSystem::GetOrCreateType<vk::PipelineLayout>("vk::PipelineLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPipeline", -1, ReflectionSystem::GetOrCreateType<vk::Pipeline>("vk::Pipeline"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFragmentShader", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Shader>>("std::shared_ptr<Shader>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPassName", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasDynamicAttachments", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasDepthAttachment", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNeedsBindlessTextureDescriptor", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myColorAttachments", -1, ReflectionSystem::GetOrCreateType<List<vk::RenderingAttachmentInfo>>("List<vk::RenderingAttachmentInfo>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myColorFormats", -1, ReflectionSystem::GetOrCreateType<List<vk::Format>>("List<vk::Format>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDepthAttachment", -1, ReflectionSystem::GetOrCreateType<vk::RenderingAttachmentInfo>("vk::RenderingAttachmentInfo"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDepthFormat", -1, ReflectionSystem::GetOrCreateType<vk::Format>("vk::Format"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IRenderPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GraphicsPass* instance = static_cast<GraphicsPass*>(inInstance);
static thread_local List<vk::RenderingAttachmentInfo> result = instance->GetDynamicColorAttachments();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDynamicColorAttachments", ReflectionSystem::GetOrCreateType<List<vk::RenderingAttachmentInfo>>("List<vk::RenderingAttachmentInfo>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GraphicsPass* instance = static_cast<GraphicsPass*>(inInstance);
static thread_local vk::RenderingAttachmentInfo result = instance->GetDynamicDepthAttachments();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDynamicDepthAttachments", ReflectionSystem::GetOrCreateType<vk::RenderingAttachmentInfo>("vk::RenderingAttachmentInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GraphicsPass* instance = static_cast<GraphicsPass*>(inInstance);
static thread_local vk::SampleCountFlagBits result = instance->GetNumSamples();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNumSamples", ReflectionSystem::GetOrCreateType<vk::SampleCountFlagBits>("vk::SampleCountFlagBits"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GraphicsPass* instance = static_cast<GraphicsPass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->Execute(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("Execute", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GraphicsPass* instance = static_cast<GraphicsPass*>(inInstance);
instance->CreateResources();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateResources", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GraphicsPass* instance = static_cast<GraphicsPass*>(inInstance);
instance->DestroyResources();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DestroyResources", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GraphicsPass* instance = static_cast<GraphicsPass*>(inInstance);
instance->OnShaderRecompiled();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnShaderRecompiled", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GraphicsPass* instance = static_cast<GraphicsPass*>(inInstance);
VulkanImage * arg0 = (VulkanImage*)inArguments[0];
vk::ImageLayout& arg1 = *(vk::ImageLayout*)inArguments[1];
vk::AttachmentLoadOp& arg2 = *(vk::AttachmentLoadOp*)inArguments[2];
vk::AttachmentStoreOp& arg3 = *(vk::AttachmentStoreOp*)inArguments[3];
VulkanImage * arg4 = (VulkanImage*)inArguments[4];
instance->AddColorAttachment(arg0, arg1, arg2, arg3, arg4);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inImage", ReflectionSystem::GetOrCreateType<VulkanImage *>("VulkanImage *")));
arguments.Add(MethodArgument("inLayout", ReflectionSystem::GetOrCreateType<vk::ImageLayout>("vk::ImageLayout")));
arguments.Add(MethodArgument("inLoadOp", ReflectionSystem::GetOrCreateType<vk::AttachmentLoadOp>("vk::AttachmentLoadOp")));
arguments.Add(MethodArgument("inStoreOp", ReflectionSystem::GetOrCreateType<vk::AttachmentStoreOp>("vk::AttachmentStoreOp")));
arguments.Add(MethodArgument("myResolveImage", ReflectionSystem::GetOrCreateType<VulkanImage *>("VulkanImage *")));
Method& currentMethod = currentClass->AddMethod(Method("AddColorAttachment", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GraphicsPass* instance = static_cast<GraphicsPass*>(inInstance);
VulkanImage * arg0 = (VulkanImage*)inArguments[0];
vk::ImageLayout& arg1 = *(vk::ImageLayout*)inArguments[1];
vk::AttachmentLoadOp& arg2 = *(vk::AttachmentLoadOp*)inArguments[2];
vk::AttachmentStoreOp& arg3 = *(vk::AttachmentStoreOp*)inArguments[3];
instance->AddDepthAttachment(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inImage", ReflectionSystem::GetOrCreateType<VulkanImage *>("VulkanImage *")));
arguments.Add(MethodArgument("inLayout", ReflectionSystem::GetOrCreateType<vk::ImageLayout>("vk::ImageLayout")));
arguments.Add(MethodArgument("inLoadOp", ReflectionSystem::GetOrCreateType<vk::AttachmentLoadOp>("vk::AttachmentLoadOp")));
arguments.Add(MethodArgument("inStoreOp", ReflectionSystem::GetOrCreateType<vk::AttachmentStoreOp>("vk::AttachmentStoreOp")));
Method& currentMethod = currentClass->AddMethod(Method("AddDepthAttachment", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<CopyTexturePass>();
	{
		Field& currentField = currentClass->AddField(Field("mySourceTexture", -1, ReflectionSystem::GetOrCreateType<VulkanImage>("VulkanImage"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTargetTexture", -1, ReflectionSystem::GetOrCreateType<VulkanImage>("VulkanImage"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<GraphicsPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CopyTexturePass* instance = static_cast<CopyTexturePass*>(inInstance);
instance->SetupAttachments();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupAttachments", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CopyTexturePass* instance = static_cast<CopyTexturePass*>(inInstance);
instance->SetupDescriptors();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupDescriptors", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CopyTexturePass* instance = static_cast<CopyTexturePass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->DrawCall(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("DrawCall", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<CopyToSwapchainPass>();
	{
		Field& currentField = currentClass->AddField(Field("mySourceTexture", -1, ReflectionSystem::GetOrCreateType<VulkanImage>("VulkanImage"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<GraphicsPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CopyToSwapchainPass* instance = static_cast<CopyToSwapchainPass*>(inInstance);
instance->SetupDescriptors();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupDescriptors", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CopyToSwapchainPass* instance = static_cast<CopyToSwapchainPass*>(inInstance);
instance->SetupAttachments();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupAttachments", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CopyToSwapchainPass* instance = static_cast<CopyToSwapchainPass*>(inInstance);
static thread_local vk::SampleCountFlagBits result = instance->GetNumSamples();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNumSamples", ReflectionSystem::GetOrCreateType<vk::SampleCountFlagBits>("vk::SampleCountFlagBits"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CopyToSwapchainPass* instance = static_cast<CopyToSwapchainPass*>(inInstance);
static thread_local List<vk::RenderingAttachmentInfo> result = instance->GetDynamicColorAttachments();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDynamicColorAttachments", ReflectionSystem::GetOrCreateType<List<vk::RenderingAttachmentInfo>>("List<vk::RenderingAttachmentInfo>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CopyToSwapchainPass* instance = static_cast<CopyToSwapchainPass*>(inInstance);
static thread_local vk::RenderingAttachmentInfo result = instance->GetDynamicDepthAttachments();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDynamicDepthAttachments", ReflectionSystem::GetOrCreateType<vk::RenderingAttachmentInfo>("vk::RenderingAttachmentInfo"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
CopyToSwapchainPass* instance = static_cast<CopyToSwapchainPass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->DrawCall(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("DrawCall", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<DebugPass>();
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<GraphicsPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DebugPass* instance = static_cast<DebugPass*>(inInstance);
instance->SetupAttachments();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupAttachments", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DebugPass* instance = static_cast<DebugPass*>(inInstance);
instance->SetupDescriptors();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupDescriptors", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DebugPass* instance = static_cast<DebugPass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->DrawCall(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("DrawCall", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ImGuiPass>();
	{
		Field& currentField = currentClass->AddField(Field("myRenderPass", -1, ReflectionSystem::GetOrCreateType<vk::RenderPass>("vk::RenderPass"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameBuffers", -1, ReflectionSystem::GetOrCreateType<List<vk::Framebuffer>>("List<vk::Framebuffer>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IRenderPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiPass* instance = static_cast<ImGuiPass*>(inInstance);
instance->CreateResources();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateResources", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiPass* instance = static_cast<ImGuiPass*>(inInstance);
instance->DestroyResources();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DestroyResources", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiPass* instance = static_cast<ImGuiPass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->Execute(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("Execute", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MainPass>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<GraphicsPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MainPass* instance = static_cast<MainPass*>(inInstance);
instance->SetupDescriptors();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupDescriptors", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MainPass* instance = static_cast<MainPass*>(inInstance);
instance->SetupAttachments();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupAttachments", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MainPass* instance = static_cast<MainPass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->DrawCall(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("DrawCall", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<SkyboxPass>();
	{
		Field& currentField = currentClass->AddField(Field("mySkyboxModel", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Model>>("std::shared_ptr<Model>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySkybox", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFrameDataBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<GraphicsPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SkyboxPass* instance = static_cast<SkyboxPass*>(inInstance);
instance->SetupAttachments();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupAttachments", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SkyboxPass* instance = static_cast<SkyboxPass*>(inInstance);
instance->SetupDescriptors();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("SetupDescriptors", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SkyboxPass* instance = static_cast<SkyboxPass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->DrawCall(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("DrawCall", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<TransitionImagePass>();
	{
		Field& currentField = currentClass->AddField(Field("myImage", -1, ReflectionSystem::GetOrCreateType<vk::Image>("vk::Image"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySrcAccess", -1, ReflectionSystem::GetOrCreateType<vk::AccessFlagBits>("vk::AccessFlagBits"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDstAccess", -1, ReflectionSystem::GetOrCreateType<vk::AccessFlagBits>("vk::AccessFlagBits"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySrcLayout", -1, ReflectionSystem::GetOrCreateType<vk::ImageLayout>("vk::ImageLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDstLayout", -1, ReflectionSystem::GetOrCreateType<vk::ImageLayout>("vk::ImageLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySrcStage", -1, ReflectionSystem::GetOrCreateType<vk::PipelineStageFlagBits>("vk::PipelineStageFlagBits"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDstStage", -1, ReflectionSystem::GetOrCreateType<vk::PipelineStageFlagBits>("vk::PipelineStageFlagBits"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IRenderPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransitionImagePass* instance = static_cast<TransitionImagePass*>(inInstance);
instance->CreateResources();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateResources", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransitionImagePass* instance = static_cast<TransitionImagePass*>(inInstance);
instance->DestroyResources();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DestroyResources", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransitionImagePass* instance = static_cast<TransitionImagePass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->Execute(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("Execute", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<TransitionSwapchainImagePass>();
	{
		Field& currentField = currentClass->AddField(Field("mySrcAccess", -1, ReflectionSystem::GetOrCreateType<vk::AccessFlagBits>("vk::AccessFlagBits"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDstAccess", -1, ReflectionSystem::GetOrCreateType<vk::AccessFlagBits>("vk::AccessFlagBits"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySrcLayout", -1, ReflectionSystem::GetOrCreateType<vk::ImageLayout>("vk::ImageLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDstLayout", -1, ReflectionSystem::GetOrCreateType<vk::ImageLayout>("vk::ImageLayout"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySrcStage", -1, ReflectionSystem::GetOrCreateType<vk::PipelineStageFlagBits>("vk::PipelineStageFlagBits"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDstStage", -1, ReflectionSystem::GetOrCreateType<vk::PipelineStageFlagBits>("vk::PipelineStageFlagBits"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IRenderPass>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransitionSwapchainImagePass* instance = static_cast<TransitionSwapchainImagePass*>(inInstance);
instance->CreateResources();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("CreateResources", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransitionSwapchainImagePass* instance = static_cast<TransitionSwapchainImagePass*>(inInstance);
instance->DestroyResources();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DestroyResources", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TransitionSwapchainImagePass* instance = static_cast<TransitionSwapchainImagePass*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->Execute(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("Execute", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<TextureSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorPool", -1, ReflectionSystem::GetOrCreateType<vk::DescriptorPool>("vk::DescriptorPool"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureSystem* instance = static_cast<TextureSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inTexture", ReflectionSystem::GetOrCreateType<Texture *>("Texture *")));
Method& currentMethod = currentClass->AddMethod(Method("RegisterTexture_TS", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inTexture", ReflectionSystem::GetOrCreateType<TextureCube *>("TextureCube *")));
Method& currentMethod = currentClass->AddMethod(Method("RegisterTextureCube_TS", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureSystem* instance = static_cast<TextureSystem*>(inInstance);
Texture * arg0 = (Texture*)inArguments[0];
instance->RemoveTexture_TS(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTexture", ReflectionSystem::GetOrCreateType<Texture *>("Texture *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveTexture_TS", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureSystem* instance = static_cast<TextureSystem*>(inInstance);
TextureCube * arg0 = (TextureCube*)inArguments[0];
instance->RemoveTextureCube_TS(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTexture", ReflectionSystem::GetOrCreateType<TextureCube *>("TextureCube *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveTextureCube_TS", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureSystem* instance = static_cast<TextureSystem*>(inInstance);
static thread_local vk::DescriptorSet result = instance->GetDescriptorSet();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDescriptorSet", ReflectionSystem::GetOrCreateType<vk::DescriptorSet>("vk::DescriptorSet"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
TextureSystem* instance = static_cast<TextureSystem*>(inInstance);
static thread_local vk::DescriptorSetLayout result = instance->GetDescriptorLayout();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDescriptorLayout", ReflectionSystem::GetOrCreateType<vk::DescriptorSetLayout>("vk::DescriptorSetLayout"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ResizableBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("OnBufferResized", -1, ReflectionSystem::GetOrCreateType<MulticastDelegate<void ()>>("MulticastDelegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasActiveUpload", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHasRegisteredForTick", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanBuffer>("VulkanBuffer"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
VulkanBuffer * result = instance->GetBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetBuffer", ReflectionSystem::GetOrCreateType<VulkanBuffer *>("VulkanBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
VulkanBuffer * arg0 = (VulkanBuffer*)inArguments[0];
const unsigned int& arg1 = *(const unsigned int*)inArguments[1];
unsigned int& arg2 = *(unsigned int*)inArguments[2];
instance->CopyDataFromBuffer(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inStagingBuffer", ReflectionSystem::GetOrCreateType<VulkanBuffer *>("VulkanBuffer *")));
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inOffset", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("CopyDataFromBuffer", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
const void * arg0 = (const void*)inArguments[0];
const unsigned int& arg1 = *(const unsigned int*)inArguments[1];
unsigned int& arg2 = *(unsigned int*)inArguments[2];
instance->SetData(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inData", ReflectionSystem::GetOrCreateType<const void *>("const void *")));
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inOffset", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetData", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
const unsigned int& arg1 = *(const unsigned int*)inArguments[1];
const unsigned int& arg2 = *(const unsigned int*)inArguments[2];
instance->MoveData(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSourceOffset", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inDstOffset", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("MoveData", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
instance->Resize(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRequiredSize", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("Resize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ResizableBuffer* instance = static_cast<ResizableBuffer*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
instance->CheckCapacity(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRequiredSize", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("CheckCapacity", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VertexBufferSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myUsedBufferSize", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCurrentVertexOffset", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myVertexBuffers", -1, ReflectionSystem::GetOrCreateType<List<VertexBufferHandle *>>("List<VertexBufferHandle *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBuffer", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseVertexDataBuffer", -1, ReflectionSystem::GetOrCreateType<ResizableBuffer>("ResizableBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySparseVertexData_CPURepresentation", -1, ReflectionSystem::GetOrCreateType<List<VertexBufferData>>("List<VertexBufferData>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFreeSparseIndices", -1, ReflectionSystem::GetOrCreateType<List<unsigned int>>("List<unsigned int>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
VulkanBuffer * arg0 = (VulkanBuffer*)inArguments[0];
const unsigned int& arg1 = *(const unsigned int*)inArguments[1];
VertexBufferHandle * result = instance->UploadVertexBuffer(arg0, arg1);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inStagingBuffer", ReflectionSystem::GetOrCreateType<VulkanBuffer *>("VulkanBuffer *")));
arguments.Add(MethodArgument("inVertexCount", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("UploadVertexBuffer", ReflectionSystem::GetOrCreateType<VertexBufferHandle *>("VertexBufferHandle *"), invoker, arguments));
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
arguments.Add(MethodArgument("inVertices", ReflectionSystem::GetOrCreateType<const List<Vertex> &>("const List<Vertex> &")));
Method& currentMethod = currentClass->AddMethod(Method("UploadVertexBuffer", ReflectionSystem::GetOrCreateType<VertexBufferHandle *>("VertexBufferHandle *"), invoker, arguments));
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
arguments.Add(MethodArgument("inBuffer", ReflectionSystem::GetOrCreateType<const VertexBufferHandle *>("const VertexBufferHandle *")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveVertexBuffer", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
const ResizableBuffer * result = instance->GetGlobalVertexBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGlobalVertexBuffer", ReflectionSystem::GetOrCreateType<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
const ResizableBuffer * result = instance->GetGlobalSparseVertexDataBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGlobalSparseVertexDataBuffer", ReflectionSystem::GetOrCreateType<const ResizableBuffer *>("const ResizableBuffer *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VertexBufferSystem* instance = static_cast<VertexBufferSystem*>(inInstance);
static thread_local unsigned int result = instance->GetUsedBufferSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetUsedBufferSize", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
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
arguments.Add(MethodArgument("inBuffer", ReflectionSystem::GetOrCreateType<VertexBufferHandle *>("VertexBufferHandle *")));
Method& currentMethod = currentClass->AddMethod(Method("GetVertexOffsetFromVertexHandle", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<AssetSerializer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetSerializer* instance = static_cast<AssetSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetSerializer* instance = static_cast<AssetSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inSerializer", ReflectionSystem::GetOrCreateType<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<FileTimeSerializer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
FileTimeSerializer* instance = static_cast<FileTimeSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
FileTimeSerializer* instance = static_cast<FileTimeSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inSerializer", ReflectionSystem::GetOrCreateType<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ListSerializer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ListSerializer* instance = static_cast<ListSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ListSerializer* instance = static_cast<ListSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inSerializer", ReflectionSystem::GetOrCreateType<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<MapSerializer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MapSerializer* instance = static_cast<MapSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
MapSerializer* instance = static_cast<MapSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inSerializer", ReflectionSystem::GetOrCreateType<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PathSerializer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PathSerializer* instance = static_cast<PathSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PathSerializer* instance = static_cast<PathSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inSerializer", ReflectionSystem::GetOrCreateType<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<AssetPropertyDrawer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<AdvancedPropertyDrawer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetPropertyDrawer* instance = static_cast<AssetPropertyDrawer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->DrawsType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("DrawsType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetPropertyDrawer* instance = static_cast<AssetPropertyDrawer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Field & arg1 = *(const Field*)inArguments[1];
static thread_local bool result = instance->Draw(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inField", ReflectionSystem::GetOrCreateType<const Field &>("const Field &")));
Method& currentMethod = currentClass->AddMethod(Method("Draw", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PrimitiveSerializer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PrimitiveSerializer* instance = static_cast<PrimitiveSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PrimitiveSerializer* instance = static_cast<PrimitiveSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inSerializer", ReflectionSystem::GetOrCreateType<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<StringSerializer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StringSerializer* instance = static_cast<StringSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StringSerializer* instance = static_cast<StringSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inSerializer", ReflectionSystem::GetOrCreateType<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<WStringSerializer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WStringSerializer* instance = static_cast<WStringSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WStringSerializer* instance = static_cast<WStringSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inClass", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inSerializer", ReflectionSystem::GetOrCreateType<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<UniquePtrSerializer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<TypeSerializer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
UniquePtrSerializer* instance = static_cast<UniquePtrSerializer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->SerializesType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SerializesType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
UniquePtrSerializer* instance = static_cast<UniquePtrSerializer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
BinarySerializer * arg2 = (BinarySerializer*)inArguments[2];
instance->Serialize(arg0, arg1, arg2);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
arguments.Add(MethodArgument("inSerializer", ReflectionSystem::GetOrCreateType<BinarySerializer *>("BinarySerializer *")));
Method& currentMethod = currentClass->AddMethod(Method("Serialize", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PlayerCameraControllerComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myMouseSensitivity", offsetof(PlayerCameraControllerComponent, myMouseSensitivity), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySpringArmChangeAmount", offsetof(PlayerCameraControllerComponent, mySpringArmChangeAmount), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMinSpringArmLength", offsetof(PlayerCameraControllerComponent, myMinSpringArmLength), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMaxSpringArmLength", offsetof(PlayerCameraControllerComponent, myMaxSpringArmLength), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myPitch", offsetof(PlayerCameraControllerComponent, myPitch), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myYaw", offsetof(PlayerCameraControllerComponent, myYaw), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStoredPitch", offsetof(PlayerCameraControllerComponent, myStoredPitch), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStoredYaw", offsetof(PlayerCameraControllerComponent, myStoredYaw), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStoredSpringArmLength", offsetof(PlayerCameraControllerComponent, myStoredSpringArmLength), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PlayerCameraControllerComponent* instance = static_cast<PlayerCameraControllerComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<LandscapeSystem>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<WorldSystem>());
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanCommandBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("myCommandBuffer", -1, ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCommandPool", -1, ReflectionSystem::GetOrCreateType<vk::CommandPool>("vk::CommandPool"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanCommandBuffer* instance = static_cast<VulkanCommandBuffer*>(inInstance);
static thread_local vk::CommandBuffer result = instance->GetAPIResource();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAPIResource", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ListPropertyDrawer>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<AdvancedPropertyDrawer>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ListPropertyDrawer* instance = static_cast<ListPropertyDrawer*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->DrawsType(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("DrawsType", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ListPropertyDrawer* instance = static_cast<ListPropertyDrawer*>(inInstance);
void * arg0 = (void*)inArguments[0];
const Field & arg1 = *(const Field*)inArguments[1];
static thread_local bool result = instance->Draw(arg0, arg1);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
arguments.Add(MethodArgument("inField", ReflectionSystem::GetOrCreateType<const Field &>("const Field &")));
Method& currentMethod = currentClass->AddMethod(Method("Draw", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<IGPUList>();
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<GPUSparseDenseBuffer<PointLightData>>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<IGPUList>());
	currentClass->AddTemplateArgument(ReflectionSystem::GetOrCreateType<PointLightData>("PointLightData"), false, false);
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<InspectorWindow>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<EditorWindow>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
InspectorWindow* instance = static_cast<InspectorWindow*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ActorUtils>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ActorUtils* instance = static_cast<ActorUtils*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
Actor * result = instance->DuplicateActor(arg0);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inActor", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("DuplicateActor", ReflectionSystem::GetOrCreateType<Actor *>("Actor *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<BinaryUtils>();
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Debug>();
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
arguments.Add(MethodArgument("inStart", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inEnd", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inColor", ReflectionSystem::GetOrCreateType<const Color &>("const Color &")));
Method& currentMethod = currentClass->AddMethod(Method("DrawLine", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inStart", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inEnd", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inColor", ReflectionSystem::GetOrCreateType<const Color &>("const Color &")));
Method& currentMethod = currentClass->AddMethod(Method("DrawArrow", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Debug* instance = static_cast<Debug*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const float& arg1 = *(const float*)inArguments[1];
const Color & arg2 = *(const Color*)inArguments[2];
const int& arg3 = *(const int*)inArguments[3];
instance->DrawSphere(arg0, arg1, arg2, arg3);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPosition", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inRadius", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inColor", ReflectionSystem::GetOrCreateType<const Color &>("const Color &")));
arguments.Add(MethodArgument("inSphereSubdivisions", ReflectionSystem::GetOrCreateType<const int>("const int")));
Method& currentMethod = currentClass->AddMethod(Method("DrawSphere", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Debug* instance = static_cast<Debug*>(inInstance);
const glm::vec<3, float> & arg0 = *(const glm::vec<3, float>*)inArguments[0];
const glm::qua<float> & arg1 = *(const glm::qua<float>*)inArguments[1];
const float& arg2 = *(const float*)inArguments[2];
const float& arg3 = *(const float*)inArguments[3];
const Color & arg4 = *(const Color*)inArguments[4];
instance->DrawCapsule(arg0, arg1, arg2, arg3, arg4);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inPosition", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
arguments.Add(MethodArgument("inRotation", ReflectionSystem::GetOrCreateType<const glm::qua<float> &>("const glm::qua<float> &")));
arguments.Add(MethodArgument("inRadius", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inHeight", ReflectionSystem::GetOrCreateType<const float>("const float")));
arguments.Add(MethodArgument("inColor", ReflectionSystem::GetOrCreateType<const Color &>("const Color &")));
Method& currentMethod = currentClass->AddMethod(Method("DrawCapsule", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Debug* instance = static_cast<Debug*>(inInstance);
const List<Debug::DrawLineInfos> & result = instance->GetDrawInfos();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetDrawInfos", ReflectionSystem::GetOrCreateType<const List<Debug::DrawLineInfos> &>("const List<Debug::DrawLineInfos> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Debug* instance = static_cast<Debug*>(inInstance);
instance->ClearDrawInfos();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ClearDrawInfos", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Debug::DrawLineInfos>();
	{
		Field& currentField = currentClass->AddField(Field("myStart", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myEnd", -1, ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myColor", -1, ReflectionSystem::GetOrCreateType<Color>("Color"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<StagingBuffer>();
	{
		Field& currentField = currentClass->AddField(Field("myUnderlyingBuffer", -1, ReflectionSystem::GetOrCreateType<VulkanBuffer>("VulkanBuffer"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySize", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myOffset", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingBuffer* instance = static_cast<StagingBuffer*>(inInstance);
const void * arg0 = (const void*)inArguments[0];
unsigned int& arg1 = *(unsigned int*)inArguments[1];
instance->SetData(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inData", ReflectionSystem::GetOrCreateType<const void *>("const void *")));
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetData", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingBuffer* instance = static_cast<StagingBuffer*>(inInstance);
static thread_local unsigned int result = instance->GetSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSize", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingBuffer* instance = static_cast<StagingBuffer*>(inInstance);
static thread_local unsigned int result = instance->GetOffset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetOffset", ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingBuffer* instance = static_cast<StagingBuffer*>(inInstance);
VulkanBuffer * result = instance->GetUnderlyingBuffer();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetUnderlyingBuffer", ReflectionSystem::GetOrCreateType<VulkanBuffer *>("VulkanBuffer *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<StagingSystem>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<System>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingSystem* instance = static_cast<StagingSystem*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
static thread_local StagingBuffer result = instance->GetStagingBufferWithSize_TS(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inSize", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("GetStagingBufferWithSize_TS", ReflectionSystem::GetOrCreateType<StagingBuffer>("StagingBuffer"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
StagingSystem* instance = static_cast<StagingSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PlayerCameraActor>();
	{
		Field& currentField = currentClass->AddField(Field("myCamera", offsetof(PlayerCameraActor, myCamera), ReflectionSystem::GetOrCreateType<CameraComponent>("CameraComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myPlayerCameraController", offsetof(PlayerCameraActor, myPlayerCameraController), ReflectionSystem::GetOrCreateType<PlayerCameraControllerComponent>("PlayerCameraControllerComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySpringArmComponent", offsetof(PlayerCameraActor, mySpringArmComponent), ReflectionSystem::GetOrCreateType<SpringArmComponent>("SpringArmComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Actor>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PlayerCameraActor* instance = static_cast<PlayerCameraActor*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanImGui>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImGui* instance = static_cast<VulkanImGui*>(inInstance);
vk::RenderPass& arg0 = *(vk::RenderPass*)inArguments[0];
instance->Start(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRenderPass", ReflectionSystem::GetOrCreateType<vk::RenderPass>("vk::RenderPass")));
Method& currentMethod = currentClass->AddMethod(Method("Start", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImGui* instance = static_cast<VulkanImGui*>(inInstance);
instance->Destroy();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Destroy", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImGui* instance = static_cast<VulkanImGui*>(inInstance);
instance->BeginFrame();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("BeginFrame", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanImGui* instance = static_cast<VulkanImGui*>(inInstance);
vk::CommandBuffer& arg0 = *(vk::CommandBuffer*)inArguments[0];
instance->Render(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inCommandBuffer", ReflectionSystem::GetOrCreateType<vk::CommandBuffer>("vk::CommandBuffer")));
Method& currentMethod = currentClass->AddMethod(Method("Render", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanPhysicalDevice>();
	{
		Field& currentField = currentClass->AddField(Field("myDevice", -1, ReflectionSystem::GetOrCreateType<vk::PhysicalDevice>("vk::PhysicalDevice"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFeatures", -1, ReflectionSystem::GetOrCreateType<vk::PhysicalDeviceFeatures2>("vk::PhysicalDeviceFeatures2"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMemoryProperties", -1, ReflectionSystem::GetOrCreateType<vk::PhysicalDeviceMemoryProperties>("vk::PhysicalDeviceMemoryProperties"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myQueueFamilyProperties", -1, ReflectionSystem::GetOrCreateType<List<vk::QueueFamilyProperties>>("List<vk::QueueFamilyProperties>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myMSAASampleCount", -1, ReflectionSystem::GetOrCreateType<vk::SampleCountFlagBits>("vk::SampleCountFlagBits"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGraphicsQueueIndex", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myComputeQueueIndex", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTransferQueueIndex", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPresentQueueIndex", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDeviceExtensions", -1, ReflectionSystem::GetOrCreateType<List<const char *>>("List<const char *>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
const List<const char *> & result = instance->GetExtensions();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetExtensions", ReflectionSystem::GetOrCreateType<const List<const char *> &>("const List<const char *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local int result = instance->GetGraphicsQueueIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetGraphicsQueueIndex", ReflectionSystem::GetOrCreateType<int>("int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local int result = instance->GetComputeQueueIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetComputeQueueIndex", ReflectionSystem::GetOrCreateType<int>("int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local int result = instance->GetTransferQueueIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetTransferQueueIndex", ReflectionSystem::GetOrCreateType<int>("int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local int result = instance->GetPresentQueueIndex();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPresentQueueIndex", ReflectionSystem::GetOrCreateType<int>("int"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
vk::PhysicalDevice * result = instance->operator->();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("operator->", ReflectionSystem::GetOrCreateType<vk::PhysicalDevice *>("vk::PhysicalDevice *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
const vk::PhysicalDevice * result = instance->operator->();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("operator->", ReflectionSystem::GetOrCreateType<const vk::PhysicalDevice *>("const vk::PhysicalDevice *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local const List<vk::QueueFamilyProperties> result = instance->GetQueueFamilyProperties();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetQueueFamilyProperties", ReflectionSystem::GetOrCreateType<const List<vk::QueueFamilyProperties>>("const List<vk::QueueFamilyProperties>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
const vk::PhysicalDeviceMemoryProperties & result = instance->GetMemoryProperties();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMemoryProperties", ReflectionSystem::GetOrCreateType<const vk::PhysicalDeviceMemoryProperties &>("const vk::PhysicalDeviceMemoryProperties &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local vk::SampleCountFlagBits result = instance->GetMaxMSAASamples();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMaxMSAASamples", ReflectionSystem::GetOrCreateType<vk::SampleCountFlagBits>("vk::SampleCountFlagBits"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanPhysicalDevice* instance = static_cast<VulkanPhysicalDevice*>(inInstance);
static thread_local vk::PhysicalDevice result = instance->GetPhysicalDevice();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetPhysicalDevice", ReflectionSystem::GetOrCreateType<vk::PhysicalDevice>("vk::PhysicalDevice"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<VulkanShaderIncluder>();
	{
		Field& currentField = currentClass->AddField(Field("myExternalIncludePaths", -1, ReflectionSystem::GetOrCreateType<const List<std::filesystem::path>>("const List<std::filesystem::path>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<shaderc::CompileOptions::IncluderInterface>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanShaderIncluder* instance = static_cast<VulkanShaderIncluder*>(inInstance);
const char * arg0 = (const char*)inArguments[0];
shaderc_include_type& arg1 = *(shaderc_include_type*)inArguments[1];
const char * arg2 = (const char*)inArguments[2];
unsigned long long& arg3 = *(unsigned long long*)inArguments[3];
shaderc_include_result * result = instance->GetInclude(arg0, arg1, arg2, arg3);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRequestedSource", ReflectionSystem::GetOrCreateType<const char *>("const char *")));
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<shaderc_include_type>("shaderc_include_type")));
arguments.Add(MethodArgument("inRequrestingSource", ReflectionSystem::GetOrCreateType<const char *>("const char *")));
arguments.Add(MethodArgument("inIncludeDepth", ReflectionSystem::GetOrCreateType<unsigned long long>("unsigned long long")));
Method& currentMethod = currentClass->AddMethod(Method("GetInclude", ReflectionSystem::GetOrCreateType<shaderc_include_result *>("shaderc_include_result *"), invoker, arguments));
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
arguments.Add(MethodArgument("inData", ReflectionSystem::GetOrCreateType<shaderc_include_result *>("shaderc_include_result *")));
Method& currentMethod = currentClass->AddMethod(Method("ReleaseInclude", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanShaderIncluder* instance = static_cast<VulkanShaderIncluder*>(inInstance);
static thread_local List<std::filesystem::path> result = instance->GetIncludedFiles();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetIncludedFiles", ReflectionSystem::GetOrCreateType<List<std::filesystem::path>>("List<std::filesystem::path>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
VulkanShaderIncluder* instance = static_cast<VulkanShaderIncluder*>(inInstance);
const char * arg0 = (const char*)inArguments[0];
static thread_local std::filesystem::path result = instance->ResolvePath(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inRequestedSource", ReflectionSystem::GetOrCreateType<const char *>("const char *")));
Method& currentMethod = currentClass->AddMethod(Method("ResolvePath", ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<WindowHandler>();
	{
		Field& currentField = currentClass->AddField(Field("myHWND", -1, ReflectionSystem::GetOrCreateType<HWND__>("HWND__"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHInstance", -1, ReflectionSystem::GetOrCreateType<HINSTANCE__>("HINSTANCE__"), true, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WindowHandler* instance = static_cast<WindowHandler*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WindowHandler* instance = static_cast<WindowHandler*>(inInstance);
HWND__ * result = instance->GetHWND();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHWND", ReflectionSystem::GetOrCreateType<HWND__ *>("HWND__ *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
WindowHandler* instance = static_cast<WindowHandler*>(inInstance);
HINSTANCE__ * result = instance->GetHInstance();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetHInstance", ReflectionSystem::GetOrCreateType<HINSTANCE__ *>("HINSTANCE__ *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<GameWorld>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<World>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameWorld* instance = static_cast<GameWorld*>(inInstance);
instance->Init();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Init", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GameWorld* instance = static_cast<GameWorld*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<EditorCameraActor>();
	{
		Field& currentField = currentClass->AddField(Field("myCamera", offsetof(EditorCameraActor, myCamera), ReflectionSystem::GetOrCreateType<CameraComponent>("CameraComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myMovementComponent", offsetof(EditorCameraActor, myMovementComponent), ReflectionSystem::GetOrCreateType<EditorCameraMovementComponent>("EditorCameraMovementComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Actor>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorCameraActor* instance = static_cast<EditorCameraActor*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorCameraActor* instance = static_cast<EditorCameraActor*>(inInstance);
static thread_local bool result = instance->IsTransientActor();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsTransientActor", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorCameraActor* instance = static_cast<EditorCameraActor*>(inInstance);
EditorCameraMovementComponent & result = instance->GetMovementComponent();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetMovementComponent", ReflectionSystem::GetOrCreateType<EditorCameraMovementComponent &>("EditorCameraMovementComponent &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorCameraActor* instance = static_cast<EditorCameraActor*>(inInstance);
CameraComponent & result = instance->GetCameraComponent();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetCameraComponent", ReflectionSystem::GetOrCreateType<CameraComponent &>("CameraComponent &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<AssetEditor>();
	{
		Field& currentField = currentClass->AddField(Field("myAsset", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Asset>>("std::shared_ptr<Asset>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myType", -1, ReflectionSystem::GetOrCreateType<const Type>("const Type"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsOpen", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetEditor* instance = static_cast<AssetEditor*>(inInstance);
instance->DoDrawAssetEditor();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DoDrawAssetEditor", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetEditor* instance = static_cast<AssetEditor*>(inInstance);
static thread_local std::shared_ptr<Asset> result = instance->GetAsset();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetAsset", ReflectionSystem::GetOrCreateType<std::shared_ptr<Asset>>("std::shared_ptr<Asset>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetEditor* instance = static_cast<AssetEditor*>(inInstance);
const Type * result = instance->GetType();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetEditor* instance = static_cast<AssetEditor*>(inInstance);
std::shared_ptr<Asset>& arg0 = *(std::shared_ptr<Asset>*)inArguments[0];
instance->SetAsset(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inAsset", ReflectionSystem::GetOrCreateType<std::shared_ptr<Asset>>("std::shared_ptr<Asset>")));
Method& currentMethod = currentClass->AddMethod(Method("SetAsset", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetEditor* instance = static_cast<AssetEditor*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
instance->SetType(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("SetType", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetEditor* instance = static_cast<AssetEditor*>(inInstance);
static thread_local bool result = instance->ShouldClose();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ShouldClose", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<EditorSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myID", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<HierarchyWindow>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<EditorWindow>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
HierarchyWindow* instance = static_cast<HierarchyWindow*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
HierarchyWindow* instance = static_cast<HierarchyWindow*>(inInstance);
instance->TickInput();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickInput", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<AssetEditorSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myOpenEditors", -1, ReflectionSystem::GetOrCreateType<List<AssetEditor *>>("List<AssetEditor *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myAssetEditors", -1, ReflectionSystem::GetOrCreateType<List<AssetEditor *>>("List<AssetEditor *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDefaultAssetEditor", -1, ReflectionSystem::GetOrCreateType<AssetEditor>("AssetEditor"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<EditorSystem>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetEditorSystem* instance = static_cast<AssetEditorSystem*>(inInstance);
std::shared_ptr<Asset>& arg0 = *(std::shared_ptr<Asset>*)inArguments[0];
const Type * arg1 = (const Type*)inArguments[1];
AssetEditor * result = instance->OpenAssetEditor(arg0, arg1);
return (void*)result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inAsset", ReflectionSystem::GetOrCreateType<std::shared_ptr<Asset>>("std::shared_ptr<Asset>")));
arguments.Add(MethodArgument("inAssetType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("OpenAssetEditor", ReflectionSystem::GetOrCreateType<AssetEditor *>("AssetEditor *"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetEditorSystem* instance = static_cast<AssetEditorSystem*>(inInstance);
AssetEditor * arg0 = (AssetEditor*)inArguments[0];
instance->CloseAssetEditor(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inEditor", ReflectionSystem::GetOrCreateType<AssetEditor *>("AssetEditor *")));
Method& currentMethod = currentClass->AddMethod(Method("CloseAssetEditor", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
AssetEditorSystem* instance = static_cast<AssetEditorSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<DefaultAssetEditor>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<AssetEditor>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DefaultAssetEditor* instance = static_cast<DefaultAssetEditor*>(inInstance);
const Type * arg0 = (const Type*)inArguments[0];
static thread_local bool result = instance->EditsAsset(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inType", ReflectionSystem::GetOrCreateType<const Type *>("const Type *")));
Method& currentMethod = currentClass->AddMethod(Method("EditsAsset", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
DefaultAssetEditor* instance = static_cast<DefaultAssetEditor*>(inInstance);
instance->DrawAssetEditor();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DrawAssetEditor", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ContentBrowserItem>();
	{
		Field& currentField = currentClass->AddField(Field("myPath", -1, ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTexture", -1, ReflectionSystem::GetOrCreateType<vk::DescriptorSet>("vk::DescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsSelected", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsDirectory", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ContentBrowserWindow>();
	{
		Field& currentField = currentClass->AddField(Field("myTooltipProgress", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTooltipDuration", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCanSeeTooltip", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRoot", -1, ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCurrentPath", -1, ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySelectedTreePath", -1, ReflectionSystem::GetOrCreateType<std::filesystem::path>("std::filesystem::path"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySearchString", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySearchPaths", -1, ReflectionSystem::GetOrCreateType<List<std::filesystem::path>>("List<std::filesystem::path>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDirectories", -1, ReflectionSystem::GetOrCreateType<std::map<std::filesystem::path, List<std::filesystem::path>>>("std::map<std::filesystem::path, List<std::filesystem::path>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myItems", -1, ReflectionSystem::GetOrCreateType<List<ContentBrowserItem>>("List<ContentBrowserItem>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myHistory", -1, ReflectionSystem::GetOrCreateType<std::deque<std::filesystem::path>>("std::deque<std::filesystem::path>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myForwardHistory", -1, ReflectionSystem::GetOrCreateType<std::deque<std::filesystem::path>>("std::deque<std::filesystem::path>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myContentBrowserTextures", -1, ReflectionSystem::GetOrCreateType<std::array<std::shared_ptr<Texture>, 5>>("std::array<std::shared_ptr<Texture>, 5>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myContentBrowserTextureDescriptors", -1, ReflectionSystem::GetOrCreateType<std::array<vk::DescriptorSet, 5>>("std::array<vk::DescriptorSet, 5>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myExcludedExtensions", -1, ReflectionSystem::GetOrCreateType<List<std::basic_string<char>>>("List<std::basic_string<char>>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDragDropPath", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySelectedItem", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenamingItem", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNewFileName", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myRenamingInProgress", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySearchBarOpenedThisFrame", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTextureSize", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTextureSizeMin", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myTextureSizeMax", -1, ReflectionSystem::GetOrCreateType<float>("float"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myFocusedResult", -1, ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myConfirmPrompt", -1, ReflectionSystem::GetOrCreateType<EditorConfirmPrompt>("EditorConfirmPrompt"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<EditorWindow>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ContentBrowserWindow* instance = static_cast<ContentBrowserWindow*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ContentBrowserWindow* instance = static_cast<ContentBrowserWindow*>(inInstance);
instance->ReloadDirectory();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ReloadDirectory", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Editor>();
	{
		Field& currentField = currentClass->AddField(Field("myWindows", -1, ReflectionSystem::GetOrCreateType<List<EditorWindow *>>("List<EditorWindow *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySystems", -1, ReflectionSystem::GetOrCreateType<List<EditorSystem *>>("List<EditorSystem *>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myNextID", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myGameTickFunction", -1, ReflectionSystem::GetOrCreateType<Delegate<void ()>>("Delegate<void ()>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsPIE", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
instance->StaticTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("StaticTick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
EditorWindow * arg0 = (EditorWindow*)inArguments[0];
const bool& arg1 = *(const bool*)inArguments[1];
instance->RemoveWindow(arg0, arg1);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inEditorWindow", ReflectionSystem::GetOrCreateType<EditorWindow *>("EditorWindow *")));
arguments.Add(MethodArgument("inIsShutdown", ReflectionSystem::GetOrCreateType<const bool>("const bool")));
Method& currentMethod = currentClass->AddMethod(Method("RemoveWindow", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inTickFunction", ReflectionSystem::GetOrCreateType<const Delegate<void ()> &>("const Delegate<void ()> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetGameTickFunction", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
instance->TogglePIE();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TogglePIE", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
static thread_local bool result = instance->IsPIE();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("IsPIE", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
instance->StartPIE();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("StartPIE", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
instance->StopPIE();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("StopPIE", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Editor* instance = static_cast<Editor*>(inInstance);
static thread_local std::shared_ptr<EditorWorld> result = instance->GetEditorWorld();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetEditorWorld", ReflectionSystem::GetOrCreateType<std::shared_ptr<EditorWorld>>("std::shared_ptr<EditorWorld>"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<EditorToolbar>();
	{
		Field& currentField = currentClass->AddField(Field("myToolbarMethods", -1, ReflectionSystem::GetOrCreateType<List<const Method *>>("List<const Method *>"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<EditorSystem>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorToolbar* instance = static_cast<EditorToolbar*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inPath", ReflectionSystem::GetOrCreateType<const std::basic_string<char> &>("const std::basic_string<char> &")));
arguments.Add(MethodArgument("inCallback", ReflectionSystem::GetOrCreateType<const Delegate<void ()> &>("const Delegate<void ()> &")));
Method& currentMethod = currentClass->AddMethod(Method("AddToolbarButton", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<GuizmoSystem>();
	{
		Field& currentField = currentClass->AddField(Field("myOperation", -1, ReflectionSystem::GetOrCreateType<ImGuizmo::OPERATION>("ImGuizmo::OPERATION"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySpace", -1, ReflectionSystem::GetOrCreateType<ImGuizmo::MODE>("ImGuizmo::MODE"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<EditorSystem>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GuizmoSystem* instance = static_cast<GuizmoSystem*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
GuizmoSystem* instance = static_cast<GuizmoSystem*>(inInstance);
instance->DrawGuizmo();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DrawGuizmo", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ImGuiDemoSystem>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<EditorSystem>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiDemoSystem* instance = static_cast<ImGuiDemoSystem*>(inInstance);
instance->OpenImGuiDemoWindow();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OpenImGuiDemoWindow", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<SelectionSystem>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SelectionSystem* instance = static_cast<SelectionSystem*>(inInstance);
const List<Actor *> & result = instance->GetSelectedObjects();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetSelectedObjects", ReflectionSystem::GetOrCreateType<const List<Actor *> &>("const List<Actor *> &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SelectionSystem* instance = static_cast<SelectionSystem*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
static thread_local bool result = instance->IsObjectSelected(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("IsObjectSelected", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SelectionSystem* instance = static_cast<SelectionSystem*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->SelectObject(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("SelectObject", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SelectionSystem* instance = static_cast<SelectionSystem*>(inInstance);
Actor * arg0 = (Actor*)inArguments[0];
instance->DeselectObject(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inObject", ReflectionSystem::GetOrCreateType<Actor *>("Actor *")));
Method& currentMethod = currentClass->AddMethod(Method("DeselectObject", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SelectionSystem* instance = static_cast<SelectionSystem*>(inInstance);
instance->ClearSelection();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("ClearSelection", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<AdvancedPropertyDrawer>();
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ImGuiPropertyDrawer>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiPropertyDrawer* instance = static_cast<ImGuiPropertyDrawer*>(inInstance);
instance->RegisterDrawers();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("RegisterDrawers", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
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
arguments.Add(MethodArgument("inField", ReflectionSystem::GetOrCreateType<const Field &>("const Field &")));
arguments.Add(MethodArgument("inInstance", ReflectionSystem::GetOrCreateType<void *>("void *")));
Method& currentMethod = currentClass->AddMethod(Method("DrawProperty", ReflectionSystem::GetOrCreateType<bool>("bool"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<EditorWorld>();
	{
		Field& currentField = currentClass->AddField(Field("myShouldTickComponents", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myEditorCamera", -1, ReflectionSystem::GetOrCreateType<EditorCameraActor>("EditorCameraActor"), true, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<World>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorWorld* instance = static_cast<EditorWorld*>(inInstance);
instance->Init();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Init", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorWorld* instance = static_cast<EditorWorld*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorWorld* instance = static_cast<EditorWorld*>(inInstance);
EditorCameraActor * result = instance->GetEditorCamera();
return (void*)result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetEditorCamera", ReflectionSystem::GetOrCreateType<EditorCameraActor *>("EditorCameraActor *"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<FileDialog>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
FileDialog* instance = static_cast<FileDialog*>(inInstance);
const char *const & arg0 = *(const char *const*)inArguments[0];
static thread_local std::basic_string<char> result = instance->OpenFileDialog(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aFilter", ReflectionSystem::GetOrCreateType<const char *const &>("const char *const &")));
Method& currentMethod = currentClass->AddMethod(Method("OpenFileDialog", ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
FileDialog* instance = static_cast<FileDialog*>(inInstance);
const char *const & arg0 = *(const char *const*)inArguments[0];
static thread_local std::basic_string<char> result = instance->SaveFileDialog(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("aFilter", ReflectionSystem::GetOrCreateType<const char *const &>("const char *const &")));
Method& currentMethod = currentClass->AddMethod(Method("SaveFileDialog", ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<EditorConfirmPrompt>();
	{
		Field& currentField = currentClass->AddField(Field("myTitle", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myBody", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myCallback", -1, ReflectionSystem::GetOrCreateType<Delegate<void ()>>("Delegate<void ()>"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorConfirmPrompt* instance = static_cast<EditorConfirmPrompt*>(inInstance);
instance->Show();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Show", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorConfirmPrompt* instance = static_cast<EditorConfirmPrompt*>(inInstance);
instance->Update();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Update", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<ImGuiTextureUtils>();
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
ImGuiTextureUtils* instance = static_cast<ImGuiTextureUtils*>(inInstance);
std::shared_ptr<Texture>& arg0 = *(std::shared_ptr<Texture>*)inArguments[0];
static thread_local vk::DescriptorSet result = instance->CreateDescriptorSetForTexture(arg0);
return (void*)&result;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTexture", ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>")));
Method& currentMethod = currentClass->AddMethod(Method("CreateDescriptorSetForTexture", ReflectionSystem::GetOrCreateType<vk::DescriptorSet>("vk::DescriptorSet"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<EditorWindow>();
	{
		Field& currentField = currentClass->AddField(Field("myWindowName", -1, ReflectionSystem::GetOrCreateType<std::basic_string<char>>("std::basic_string<char>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myIsClosable", -1, ReflectionSystem::GetOrCreateType<bool>("bool"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myID", -1, ReflectionSystem::GetOrCreateType<int>("int"), false, false));
	}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorWindow* instance = static_cast<EditorWindow*>(inInstance);
instance->DoTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("DoTick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
EditorWindow* instance = static_cast<EditorWindow*>(inInstance);
instance->TickInput();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickInput", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Viewport>();
	{
		Field& currentField = currentClass->AddField(Field("myPreviewActor", -1, ReflectionSystem::GetOrCreateType<StaticMeshActor>("StaticMeshActor"), true, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPlayButtonTexture", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myPlayButtonDescriptor", -1, ReflectionSystem::GetOrCreateType<vk::DescriptorSet>("vk::DescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStopButtonTexture", -1, ReflectionSystem::GetOrCreateType<std::shared_ptr<Texture>>("std::shared_ptr<Texture>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myStopButtonDescriptor", -1, ReflectionSystem::GetOrCreateType<vk::DescriptorSet>("vk::DescriptorSet"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myDescriptorSets", -1, ReflectionSystem::GetOrCreateType<List<vk::DescriptorSet>>("List<vk::DescriptorSet>"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("mySampler", -1, ReflectionSystem::GetOrCreateType<vk::Sampler>("vk::Sampler"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myP0", -1, ReflectionSystem::GetOrCreateType<ImVec2>("ImVec2"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myP1", -1, ReflectionSystem::GetOrCreateType<ImVec2>("ImVec2"), false, false));
	}
	{
		Field& currentField = currentClass->AddField(Field("myViewportSize", -1, ReflectionSystem::GetOrCreateType<ImVec2>("ImVec2"), false, false));
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<EditorWindow>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Viewport* instance = static_cast<Viewport*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Viewport* instance = static_cast<Viewport*>(inInstance);
instance->TickInput();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickInput", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Viewport* instance = static_cast<Viewport*>(inInstance);
static thread_local glm::vec<2, float> result = instance->GetNormalizedMousePositionInViewport();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetNormalizedMousePositionInViewport", ReflectionSystem::GetOrCreateType<glm::vec<2, float>>("glm::vec<2, float>"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Viewport* instance = static_cast<Viewport*>(inInstance);
const ImVec2 & result = instance->GetViewportPos();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetViewportPos", ReflectionSystem::GetOrCreateType<const ImVec2 &>("const ImVec2 &"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Viewport* instance = static_cast<Viewport*>(inInstance);
const ImVec2 & result = instance->GetViewportSize();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetViewportSize", ReflectionSystem::GetOrCreateType<const ImVec2 &>("const ImVec2 &"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PlayerComponent>();
	{
		Field& currentField = currentClass->AddField(Field("mySpeed", offsetof(PlayerComponent, mySpeed), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("mySprintSpeed", offsetof(PlayerComponent, mySprintSpeed), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myJumpForce", offsetof(PlayerComponent, myJumpForce), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PlayerComponent* instance = static_cast<PlayerComponent*>(inInstance);
instance->TickPhysics();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("TickPhysics", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<PlayerActor>();
	{
		Field& currentField = currentClass->AddField(Field("myStaticMesh", offsetof(PlayerActor, myStaticMesh), ReflectionSystem::GetOrCreateType<StaticMeshComponent>("StaticMeshComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myPlayerComponent", offsetof(PlayerActor, myPlayerComponent), ReflectionSystem::GetOrCreateType<PlayerComponent>("PlayerComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myCharacterController", offsetof(PlayerActor, myCharacterController), ReflectionSystem::GetOrCreateType<CharacterControllerComponent>("CharacterControllerComponent"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Actor>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
PlayerActor* instance = static_cast<PlayerActor*>(inInstance);
instance->OnCreate();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("OnCreate", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<SpringArmComponent>();
	{
		Field& currentField = currentClass->AddField(Field("myLength", offsetof(SpringArmComponent, myLength), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myOffset", offsetof(SpringArmComponent, myOffset), ReflectionSystem::GetOrCreateType<glm::vec<3, float>>("glm::vec<3, float>"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myExclusionTags", offsetof(SpringArmComponent, myExclusionTags), ReflectionSystem::GetOrCreateType<unsigned int>("unsigned int"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	{
		Field& currentField = currentClass->AddField(Field("myHitOffset", offsetof(SpringArmComponent, myHitOffset), ReflectionSystem::GetOrCreateType<float>("float"), false, false));
		currentField.AddMetadata(R"delim(SerializeField)delim");
	}
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<Component>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SpringArmComponent* instance = static_cast<SpringArmComponent*>(inInstance);
const float& arg0 = *(const float*)inArguments[0];
instance->SetLength(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inLength", ReflectionSystem::GetOrCreateType<const float>("const float")));
Method& currentMethod = currentClass->AddMethod(Method("SetLength", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SpringArmComponent* instance = static_cast<SpringArmComponent*>(inInstance);
const unsigned int& arg0 = *(const unsigned int*)inArguments[0];
instance->SetExclusionTags(arg0);
return nullptr;
});
List<MethodArgument> arguments{};
arguments.Add(MethodArgument("inTags", ReflectionSystem::GetOrCreateType<const unsigned int>("const unsigned int")));
Method& currentMethod = currentClass->AddMethod(Method("SetExclusionTags", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SpringArmComponent* instance = static_cast<SpringArmComponent*>(inInstance);
static thread_local float result = instance->GetLength();
return (void*)&result;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("GetLength", ReflectionSystem::GetOrCreateType<float>("float"), invoker, arguments));
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
arguments.Add(MethodArgument("inOffset", ReflectionSystem::GetOrCreateType<const glm::vec<3, float> &>("const glm::vec<3, float> &")));
Method& currentMethod = currentClass->AddMethod(Method("SetOffset", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
SpringArmComponent* instance = static_cast<SpringArmComponent*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}
{ 
	Type* currentClass = ReflectionSystem::GetMutableType<Game>();
	currentClass->AddBaseType(ReflectionSystem::GetMutableType<AutoInit>());
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Game* instance = static_cast<Game*>(inInstance);
instance->Init();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Init", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Game* instance = static_cast<Game*>(inInstance);
instance->StaticTick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("StaticTick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
{
Method::InvokerType invoker = Delegate<void*(void*, const List<void*>&)>([] (void* inInstance, const List<void*>& inArguments) -> void*
{
Game* instance = static_cast<Game*>(inInstance);
instance->Tick();
return nullptr;
});
List<MethodArgument> arguments{};
Method& currentMethod = currentClass->AddMethod(Method("Tick", ReflectionSystem::GetOrCreateType<void>("void"), invoker, arguments));
}
}

        }
    }
};