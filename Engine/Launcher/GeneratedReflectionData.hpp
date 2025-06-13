#pragma once
#include "Engine/Utils/StdIncludes.hpp"
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/Field.h"
#include "Engine/Reflection/ReflectionSystem.h"

#define private public
#define protected public

// BEGIN INCLUDES FOR REFLECTED TYPES

#include "../Editor/Editor.h"
#include "../Engine/Animation/Skeleton.h"
#include "../Engine/Physics/PhysicsErrorCallback.h"
#include "../Editor/Windows/SelectionSystem.h"
#include "../Editor/Windows/HierarchyWindow.h"
#include "../Editor/Windows/Viewport.h"
#include "../Editor/EditorPch.h"
#include "../Engine/Components/EditorCameraMovementComponent.h"
#include "../Editor/Windows/EditorWindow.h"
#include "../Editor/Windows/InspectorWindow.h"
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
#include "../Engine/ComponentSystem/ComponentDefines.hpp"
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
		    reflectionSystem.AddClass("Editor", typeid(Editor).name());
reflectionSystem.AddClass("Skeleton", typeid(Skeleton).name());
reflectionSystem.AddClass("Skeleton::Bone", typeid(Skeleton::Bone).name());
reflectionSystem.AddClass("PhysicsErrorCallback", typeid(PhysicsErrorCallback).name());
reflectionSystem.AddClass("SelectionSystem", typeid(SelectionSystem).name());
reflectionSystem.AddClass("HierarchyWindow", typeid(HierarchyWindow).name());
reflectionSystem.AddClass("Viewport", typeid(Viewport).name());
reflectionSystem.AddClass("EditorCameraMovementComponent", typeid(EditorCameraMovementComponent).name());
reflectionSystem.AddClass("EditorWindow", typeid(EditorWindow).name());
reflectionSystem.AddClass("InspectorWindow", typeid(InspectorWindow).name());
reflectionSystem.AddClass("Filewatcher", typeid(Filewatcher).name());
reflectionSystem.AddClass("Filewatcher::CallbackHandle", typeid(Filewatcher::CallbackHandle).name());
reflectionSystem.AddClass("Filewatcher::FileData", typeid(Filewatcher::FileData).name());
reflectionSystem.AddClass("Asset", typeid(Asset).name());
reflectionSystem.AddClass("SerializationMeshData", typeid(SerializationMeshData).name());
reflectionSystem.AddClass("Model", typeid(Model).name());
reflectionSystem.AddClass("IAssetContainer", typeid(IAssetContainer).name());
reflectionSystem.AddClass("SkyboxPipeline", typeid(SkyboxPipeline).name());
reflectionSystem.AddClass("SkyboxPipeline::FrameData", typeid(SkyboxPipeline::FrameData).name());
reflectionSystem.AddClass("ConvexColliderComponent", typeid(ConvexColliderComponent).name());
reflectionSystem.AddClass("AssetRegistry", typeid(AssetRegistry).name());
reflectionSystem.AddClass("JsonAsset", typeid(JsonAsset).name());
reflectionSystem.AddClass("Material", typeid(Material).name());
reflectionSystem.AddClass("Shader", typeid(Shader).name());
reflectionSystem.AddClass("Shader::IncludeFileData", typeid(Shader::IncludeFileData).name());
reflectionSystem.AddClass("ImageData", typeid(ImageData).name());
reflectionSystem.AddClass("Texture", typeid(Texture).name());
reflectionSystem.AddClass("ThreadPool", typeid(ThreadPool).name());
reflectionSystem.AddClass("TextureCube", typeid(TextureCube).name());
reflectionSystem.AddClass("Input", typeid(Input).name());
reflectionSystem.AddClass("Input::KeyCode", typeid(Input::KeyCode).name());
reflectionSystem.AddClass("Input::MouseButton", typeid(Input::MouseButton).name());
reflectionSystem.AddClass("Input::KeyState", typeid(Input::KeyState).name());
reflectionSystem.AddClass("TransformComponent", typeid(TransformComponent).name());
reflectionSystem.AddClass("Component", typeid(Component).name());
reflectionSystem.AddClass("Time", typeid(Time).name());
reflectionSystem.AddClass("MeshColliderComponent", typeid(MeshColliderComponent).name());
reflectionSystem.AddClass("IComponentArray", typeid(IComponentArray).name());
reflectionSystem.AddClass("Console", typeid(Console).name());
reflectionSystem.AddClass("ComponentSystem", typeid(ComponentSystem).name());
reflectionSystem.AddClass("IVulkanDynamicBuffer", typeid(IVulkanDynamicBuffer).name());
reflectionSystem.AddClass("GameObject", typeid(GameObject).name());
reflectionSystem.AddClass("CoroutineManager", typeid(CoroutineManager).name());
reflectionSystem.AddClass("LandscapeRenderComponent", typeid(LandscapeRenderComponent).name());
reflectionSystem.AddClass("BoxColliderComponent", typeid(BoxColliderComponent).name());
reflectionSystem.AddClass("CameraComponent", typeid(CameraComponent).name());
reflectionSystem.AddClass("CapsuleColliderComponent", typeid(CapsuleColliderComponent).name());
reflectionSystem.AddClass("GPUSceneSystem", typeid(GPUSceneSystem).name());
reflectionSystem.AddClass("ForceMode", typeid(ForceMode).name());
reflectionSystem.AddClass("RigidbodyComponent", typeid(RigidbodyComponent).name());
reflectionSystem.AddClass("CharacterControllerComponent", typeid(CharacterControllerComponent).name());
reflectionSystem.AddClass("ColliderComponent", typeid(ColliderComponent).name());
reflectionSystem.AddClass("RenderSystem", typeid(RenderSystem).name());
reflectionSystem.AddClass("DirectionalLightComponent", typeid(DirectionalLightComponent).name());
reflectionSystem.AddClass("WorldSystem", typeid(WorldSystem).name());
reflectionSystem.AddClass("LandscapeColliderComponent", typeid(LandscapeColliderComponent).name());
reflectionSystem.AddClass("PointLightComponent", typeid(PointLightComponent).name());
reflectionSystem.AddClass("PromiseReturnTypeImplementation<void>", typeid(PromiseReturnTypeImplementation<void>).name());
reflectionSystem.AddClass("SinWaveMovementComponent", typeid(SinWaveMovementComponent).name());
reflectionSystem.AddClass("String", typeid(String).name());
reflectionSystem.AddClass("SphereColliderComponent", typeid(SphereColliderComponent).name());
reflectionSystem.AddClass("StaticMeshComponent", typeid(StaticMeshComponent).name());
reflectionSystem.AddClass("IsCopyable<int>", typeid(IsCopyable<int>).name());
reflectionSystem.AddClass("AutoInit", typeid(AutoInit).name());
reflectionSystem.AddClass("AutoInitManager", typeid(AutoInitManager).name());
reflectionSystem.AddClass("Field", typeid(Field).name());
reflectionSystem.AddClass("Awaitable", typeid(Awaitable).name());
reflectionSystem.AddClass("ThreadType", typeid(ThreadType).name());
reflectionSystem.AddClass("Engine", typeid(Engine).name());
reflectionSystem.AddClass("EngineProperties", typeid(EngineProperties).name());
reflectionSystem.AddClass("EventHandler", typeid(EventHandler).name());
reflectionSystem.AddClass("EventObserver", typeid(EventObserver).name());
reflectionSystem.AddClass("EventType", typeid(EventType).name());
reflectionSystem.AddClass("Color", typeid(Color).name());
reflectionSystem.AddClass("PhysicsSystem", typeid(PhysicsSystem).name());
reflectionSystem.AddClass("Heightfield", typeid(Heightfield).name());
reflectionSystem.AddClass("LinearColor", typeid(LinearColor).name());
reflectionSystem.AddClass("PhysicsListener", typeid(PhysicsListener).name());
reflectionSystem.AddClass("PhysicsListener::ContactPair", typeid(PhysicsListener::ContactPair).name());
reflectionSystem.AddClass("RaycastHit", typeid(RaycastHit).name());
reflectionSystem.AddClass("Class", typeid(Class).name());
reflectionSystem.AddClass("ReflectionSystem", typeid(ReflectionSystem).name());
reflectionSystem.AddClass("DebugPipeline", typeid(DebugPipeline).name());
reflectionSystem.AddClass("DebugPipeline::FrameData", typeid(DebugPipeline::FrameData).name());
reflectionSystem.AddClass("DebugPipeline::DebugVertex", typeid(DebugPipeline::DebugVertex).name());
reflectionSystem.AddClass("FullscreenPipeline", typeid(FullscreenPipeline).name());
reflectionSystem.AddClass("GDRPipeline", typeid(GDRPipeline).name());
reflectionSystem.AddClass("GDRPipeline::ComputePassResources", typeid(GDRPipeline::ComputePassResources).name());
reflectionSystem.AddClass("GDRPipeline::FrameData", typeid(GDRPipeline::FrameData).name());
reflectionSystem.AddClass("GDRPipeline::DirectionalLightBuffer", typeid(GDRPipeline::DirectionalLightBuffer).name());
reflectionSystem.AddClass("IndexBufferHandle", typeid(IndexBufferHandle).name());
reflectionSystem.AddClass("IndexBufferSystem", typeid(IndexBufferSystem).name());
reflectionSystem.AddClass("Mesh", typeid(Mesh).name());
reflectionSystem.AddClass("MeshSystem", typeid(MeshSystem).name());
reflectionSystem.AddClass("VertexBufferHandle", typeid(VertexBufferHandle).name());
reflectionSystem.AddClass("MeshUtils", typeid(MeshUtils).name());
reflectionSystem.AddClass("TextureSystem", typeid(TextureSystem).name());
reflectionSystem.AddClass("BinaryWriter", typeid(BinaryWriter).name());
reflectionSystem.AddClass("Vertex", typeid(Vertex).name());
reflectionSystem.AddClass("VertexBufferSystem", typeid(VertexBufferSystem).name());
reflectionSystem.AddClass("BinaryReader", typeid(BinaryReader).name());
reflectionSystem.AddClass("MeshData", typeid(MeshData).name());
reflectionSystem.AddClass("VertexBufferData", typeid(VertexBufferData).name());
reflectionSystem.AddClass("IndexBufferData", typeid(IndexBufferData).name());
reflectionSystem.AddClass("MeshInstanceData", typeid(MeshInstanceData).name());
reflectionSystem.AddClass("PerDrawData", typeid(PerDrawData).name());
reflectionSystem.AddClass("PointLightData", typeid(PointLightData).name());
reflectionSystem.AddClass("System", typeid(System).name());
reflectionSystem.AddClass("LandscapeSystem", typeid(LandscapeSystem).name());
reflectionSystem.AddClass("SamplerMode", typeid(SamplerMode).name());
reflectionSystem.AddClass("VulkanUtils", typeid(VulkanUtils).name());
reflectionSystem.AddClass("PointLightSystem", typeid(PointLightSystem).name());
reflectionSystem.AddClass("BinaryUtils", typeid(BinaryUtils).name());
reflectionSystem.AddClass("Debug", typeid(Debug).name());
reflectionSystem.AddClass("Debug::DrawLineInfos", typeid(Debug::DrawLineInfos).name());
reflectionSystem.AddClass("MathUtils", typeid(MathUtils).name());
reflectionSystem.AddClass("OnScopeExit", typeid(OnScopeExit).name());
reflectionSystem.AddClass("ThreadUtils", typeid(ThreadUtils).name());
reflectionSystem.AddClass("NvidiaAftermathTracker", typeid(NvidiaAftermathTracker).name());
reflectionSystem.AddClass("ShaderDatabase", typeid(ShaderDatabase).name());
reflectionSystem.AddClass("ResizableBuffer", typeid(ResizableBuffer).name());
reflectionSystem.AddClass("StagingBuffer", typeid(StagingBuffer).name());
reflectionSystem.AddClass("StagingSystem", typeid(StagingSystem).name());
reflectionSystem.AddClass("StagingSystem::BufferData", typeid(StagingSystem::BufferData).name());
reflectionSystem.AddClass("VulkanAllocator", typeid(VulkanAllocator).name());
reflectionSystem.AddClass("VulkanAllocator::DelegateDeleteData", typeid(VulkanAllocator::DelegateDeleteData).name());
reflectionSystem.AddClass("VulkanBuffer", typeid(VulkanBuffer).name());
reflectionSystem.AddClass("VulkanContext", typeid(VulkanContext).name());
reflectionSystem.AddClass("VulkanDescriptorSet", typeid(VulkanDescriptorSet).name());
reflectionSystem.AddClass("VulkanDevice", typeid(VulkanDevice).name());
reflectionSystem.AddClass("VulkanImGui", typeid(VulkanImGui).name());
reflectionSystem.AddClass("VulkanImage", typeid(VulkanImage).name());
reflectionSystem.AddClass("VulkanPhysicalDevice", typeid(VulkanPhysicalDevice).name());
reflectionSystem.AddClass("VulkanShaderIncluder", typeid(VulkanShaderIncluder).name());
reflectionSystem.AddClass("VulkanShaderIncluder::IncludeData", typeid(VulkanShaderIncluder::IncludeData).name());
reflectionSystem.AddClass("VulkanSwapChain", typeid(VulkanSwapChain).name());
reflectionSystem.AddClass("WindowHandler", typeid(WindowHandler).name());
reflectionSystem.AddClass("World", typeid(World).name());
reflectionSystem.AddClass("PlayerCameraControllerComponent", typeid(PlayerCameraControllerComponent).name());
reflectionSystem.AddClass("PlayerComponent", typeid(PlayerComponent).name());
reflectionSystem.AddClass("SpringArmComponent", typeid(SpringArmComponent).name());
reflectionSystem.AddClass("Game", typeid(Game).name());

        }
        
        // Add all fields & add base classes.
        {
            { 
	Class* currentClass = reflectionSystem.GetMutableClass<Editor>();
	currentClass->AddField(Field("myWindows", offsetof(Editor, myWindows), reflectionSystem.GetClass<List<EditorWindow *>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNextID", offsetof(Editor, myNextID), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myGameTickFunction", offsetof(Editor, myGameTickFunction), reflectionSystem.GetClass<Delegate<void ()>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Skeleton>();
	currentClass->AddField(Field("myJoints", offsetof(Skeleton, myJoints), reflectionSystem.GetClass<List<Skeleton::Bone>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Skeleton::Bone>();
	currentClass->AddField(Field("myBindPosition", offsetof(Skeleton::Bone, myBindPosition), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBindRotation", offsetof(Skeleton::Bone, myBindRotation), reflectionSystem.GetClass<glm::qua<float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBindScale", offsetof(Skeleton::Bone, myBindScale), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIndexToParent", offsetof(Skeleton::Bone, myIndexToParent), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsErrorCallback>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SelectionSystem>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<HierarchyWindow>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Viewport>();
	currentClass->AddField(Field("myEditorCamera", offsetof(Viewport, myEditorCamera), reflectionSystem.GetClass<EditorCameraMovementComponent *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDescriptorSets", offsetof(Viewport, myDescriptorSets), reflectionSystem.GetClass<List<vk::DescriptorSet>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySampler", offsetof(Viewport, mySampler), reflectionSystem.GetClass<vk::Sampler>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myP0", offsetof(Viewport, myP0), reflectionSystem.GetClass<ImVec2>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myP1", offsetof(Viewport, myP1), reflectionSystem.GetClass<ImVec2>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorCameraMovementComponent>();
	currentClass->AddField(Field("myResetMouseDelta", offsetof(EditorCameraMovementComponent, myResetMouseDelta), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMouseDelta", offsetof(EditorCameraMovementComponent, myMouseDelta), reflectionSystem.GetClass<glm::vec<2, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myYaw", offsetof(EditorCameraMovementComponent, myYaw), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPitch", offsetof(EditorCameraMovementComponent, myPitch), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMovementSpeed", offsetof(EditorCameraMovementComponent, myMovementSpeed), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myShiftMultiplier", offsetof(EditorCameraMovementComponent, myShiftMultiplier), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myScrollMultiplier", offsetof(EditorCameraMovementComponent, myScrollMultiplier), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EditorWindow>();
	currentClass->AddField(Field("myWindowName", offsetof(EditorWindow, myWindowName), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIsClosable", offsetof(EditorWindow, myIsClosable), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myID", offsetof(EditorWindow, myID), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<InspectorWindow>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher>();
	currentClass->AddField(Field("myThread", offsetof(Filewatcher, myThread), reflectionSystem.GetClass<std::thread>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLock", offsetof(Filewatcher, myLock), reflectionSystem.GetClass<std::mutex>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myShouldRun", offsetof(Filewatcher, myShouldRun), reflectionSystem.GetClass<std::atomic<bool>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFilesToWatch", offsetof(Filewatcher, myFilesToWatch), reflectionSystem.GetClass<std::unordered_map<std::filesystem::path, Filewatcher::FileData>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myModifiedPaths", offsetof(Filewatcher, myModifiedPaths), reflectionSystem.GetClass<std::set<Filewatcher::FileData *>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher::CallbackHandle>();
	currentClass->AddField(Field("myID", offsetof(Filewatcher::CallbackHandle, myID), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher::FileData>();
	currentClass->AddField(Field("myCallbackIDIndexList", offsetof(Filewatcher::FileData, myCallbackIDIndexList), reflectionSystem.GetClass<List<Filewatcher::CallbackHandle>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCallbacks", offsetof(Filewatcher::FileData, myCallbacks), reflectionSystem.GetClass<List<std::function<void ()>>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLastModifiedTime", offsetof(Filewatcher::FileData, myLastModifiedTime), reflectionSystem.GetClass<std::chrono::time_point<std::filesystem::_File_time_clock>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Asset>();
	currentClass->AddField(Field("myPath", offsetof(Asset, myPath), reflectionSystem.GetClass<std::filesystem::path>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIsValid", offsetof(Asset, myIsValid), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myAssetRegistry", offsetof(Asset, myAssetRegistry), reflectionSystem.GetClass<AssetRegistry *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SerializationMeshData>();
	currentClass->AddField(Field("myVertices", offsetof(SerializationMeshData, myVertices), reflectionSystem.GetClass<List<Vertex>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIndices", offsetof(SerializationMeshData, myIndices), reflectionSystem.GetClass<List<unsigned int>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySphereCenterBounds", offsetof(SerializationMeshData, mySphereCenterBounds), reflectionSystem.GetClass<glm::vec<4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myStagingVertexBuffer", offsetof(SerializationMeshData, myStagingVertexBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myStagingIndexBuffer", offsetof(SerializationMeshData, myStagingIndexBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myAlbedoPath", offsetof(SerializationMeshData, myAlbedoPath), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNormalPath", offsetof(SerializationMeshData, myNormalPath), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMaterialPath", offsetof(SerializationMeshData, myMaterialPath), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Model>();
	currentClass->AddField(Field("myMeshes", offsetof(Model, myMeshes), reflectionSystem.GetClass<List<Mesh *>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IAssetContainer>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SkyboxPipeline>();
	currentClass->AddField(Field("myVertexShader", offsetof(SkyboxPipeline, myVertexShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFragmentShader", offsetof(SkyboxPipeline, myFragmentShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFrameDescriptorSet", offsetof(SkyboxPipeline, myFrameDescriptorSet), reflectionSystem.GetClass<VulkanDescriptorSet>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipelineLayout", offsetof(SkyboxPipeline, myPipelineLayout), reflectionSystem.GetClass<vk::PipelineLayout>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipeline", offsetof(SkyboxPipeline, myPipeline), reflectionSystem.GetClass<vk::Pipeline>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFrameDataBuffer", offsetof(SkyboxPipeline, myFrameDataBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySkyboxModel", offsetof(SkyboxPipeline, mySkyboxModel), reflectionSystem.GetClass<Model *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySkybox", offsetof(SkyboxPipeline, mySkybox), reflectionSystem.GetClass<Texture *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SkyboxPipeline::FrameData>();
	currentClass->AddField(Field("myToView", offsetof(SkyboxPipeline::FrameData, myToView), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myProjection", offsetof(SkyboxPipeline::FrameData, myProjection), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCameraPosition", offsetof(SkyboxPipeline::FrameData, myCameraPosition), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCubemapIndex", offsetof(SkyboxPipeline::FrameData, myCubemapIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ConvexColliderComponent>();
	currentClass->AddField(Field("myModel", offsetof(ConvexColliderComponent, myModel), reflectionSystem.GetClass<Model *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AssetRegistry>();
	currentClass->AddField(Field("myMutex", offsetof(AssetRegistry, myMutex), reflectionSystem.GetClass<std::recursive_mutex>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myContainers", offsetof(AssetRegistry, myContainers), reflectionSystem.GetClass<List<IAssetContainer *>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<JsonAsset>();
	currentClass->AddField(Field("myJson", offsetof(JsonAsset, myJson), reflectionSystem.GetClass<nlohmann::basic_json<>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Material>();
	currentClass->AddField(Field("myAlbedoTexture", offsetof(Material, myAlbedoTexture), reflectionSystem.GetClass<Texture *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNormalTexture", offsetof(Material, myNormalTexture), reflectionSystem.GetClass<Texture *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMaterialTexture", offsetof(Material, myMaterialTexture), reflectionSystem.GetClass<Texture *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Shader>();
	currentClass->AddField(Field("OnShaderRecompiled", offsetof(Shader, OnShaderRecompiled), reflectionSystem.GetClass<MulticastDelegate<void ()>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myShaderModule", offsetof(Shader, myShaderModule), reflectionSystem.GetClass<vk::ShaderModule>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCallbackHandle", offsetof(Shader, myCallbackHandle), reflectionSystem.GetClass<Filewatcher::CallbackHandle>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIncludeFiles", offsetof(Shader, myIncludeFiles), reflectionSystem.GetClass<List<Shader::IncludeFileData>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Shader::IncludeFileData>();
	currentClass->AddField(Field("myPath", offsetof(Shader::IncludeFileData, myPath), reflectionSystem.GetClass<std::filesystem::path>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCallbackHandle", offsetof(Shader::IncludeFileData, myCallbackHandle), reflectionSystem.GetClass<Filewatcher::CallbackHandle>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ImageData>();
	currentClass->AddField(Field("mySourceFile", offsetof(ImageData, mySourceFile), reflectionSystem.GetClass<std::filesystem::path>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myWidth", offsetof(ImageData, myWidth), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHeight", offsetof(ImageData, myHeight), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myChannels", offsetof(ImageData, myChannels), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNumMipLevels", offsetof(ImageData, myNumMipLevels), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPixelData", offsetof(ImageData, myPixelData), reflectionSystem.GetClass<List<unsigned char>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Texture>();
	currentClass->AddField(Field("myImage", offsetof(Texture, myImage), reflectionSystem.GetClass<VulkanImage *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBindlessIndex", offsetof(Texture, myBindlessIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadPool>();
	currentClass->AddField(Field("myThreads", offsetof(ThreadPool, myThreads), reflectionSystem.GetClass<List<std::thread>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLookingForTaskMutex", offsetof(ThreadPool, myLookingForTaskMutex), reflectionSystem.GetClass<std::mutex>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myWaitingForTaskCondition", offsetof(ThreadPool, myWaitingForTaskCondition), reflectionSystem.GetClass<std::condition_variable>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myShouldExit", offsetof(ThreadPool, myShouldExit), reflectionSystem.GetClass<std::atomic<bool>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myTasks", offsetof(ThreadPool, myTasks), reflectionSystem.GetClass<std::queue<std::function<void ()>>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TextureCube>();
	currentClass->AddField(Field("myImage", offsetof(TextureCube, myImage), reflectionSystem.GetClass<VulkanImage *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBindlessIndex", offsetof(TextureCube, myBindlessIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Input>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Input::KeyCode>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Input::MouseButton>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Input::KeyState>();
	currentClass->AddField(Field("myIsDown", offsetof(Input::KeyState, myIsDown), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIsPressed", offsetof(Input::KeyState, myIsPressed), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIsUp", offsetof(Input::KeyState, myIsUp), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TransformComponent>();
	currentClass->AddField(Field("OnPositionChanged", offsetof(TransformComponent, OnPositionChanged), reflectionSystem.GetClass<MulticastDelegate<void ()>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("OnRotationChanged", offsetof(TransformComponent, OnRotationChanged), reflectionSystem.GetClass<MulticastDelegate<void ()>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("OnScaleChanged", offsetof(TransformComponent, OnScaleChanged), reflectionSystem.GetClass<MulticastDelegate<void ()>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPositionDirty", offsetof(TransformComponent, myPositionDirty), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myRotationDirty", offsetof(TransformComponent, myRotationDirty), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myScaleDirty", offsetof(TransformComponent, myScaleDirty), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPosition", offsetof(TransformComponent, myPosition), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myRotation", offsetof(TransformComponent, myRotation), reflectionSystem.GetClass<glm::qua<float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myScale", offsetof(TransformComponent, myScale), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySkipPhysicsUpdate", offsetof(TransformComponent, mySkipPhysicsUpdate), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myParent", offsetof(TransformComponent, myParent), reflectionSystem.GetClass<TransformComponent *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myChildren", offsetof(TransformComponent, myChildren), reflectionSystem.GetClass<List<TransformComponent *>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Component>();
	currentClass->AddField(Field("myGameObject", offsetof(Component, myGameObject), reflectionSystem.GetClass<GameObject *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Time>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshColliderComponent>();
	currentClass->AddField(Field("myModel", offsetof(MeshColliderComponent, myModel), reflectionSystem.GetClass<Model *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IComponentArray>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Console>();
	currentClass->AddField(Field("myConsoleHandle", offsetof(Console, myConsoleHandle), reflectionSystem.GetClass<void *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLogToFileEnabled", offsetof(Console, myLogToFileEnabled), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLogToFileStream", offsetof(Console, myLogToFileStream), reflectionSystem.GetClass<std::basic_ofstream<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCoutBuffer", offsetof(Console, myCoutBuffer), reflectionSystem.GetClass<std::basic_streambuf<char> *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ComponentSystem>();
	currentClass->AddField(Field("myObjects", offsetof(ComponentSystem, myObjects), reflectionSystem.GetClass<List<GameObject *>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myObjectsToDestory", offsetof(ComponentSystem, myObjectsToDestory), reflectionSystem.GetClass<List<GameObject *>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myComponentArrays", offsetof(ComponentSystem, myComponentArrays), reflectionSystem.GetClass<List<IComponentArray *>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IVulkanDynamicBuffer>();
	currentClass->AddField(Field("OnBufferRecreated", offsetof(IVulkanDynamicBuffer, OnBufferRecreated), reflectionSystem.GetClass<MulticastDelegate<void ()>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GameObject>();
	currentClass->AddField(Field("OnComponentAdded", offsetof(GameObject, OnComponentAdded), reflectionSystem.GetClass<MulticastDelegate<void (Component *)>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("OnComponentRemoved", offsetof(GameObject, OnComponentRemoved), reflectionSystem.GetClass<MulticastDelegate<void (Component *)>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myComponentSystem", offsetof(GameObject, myComponentSystem), reflectionSystem.GetClass<ComponentSystem *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myTransform", offsetof(GameObject, myTransform), reflectionSystem.GetClass<TransformComponent *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myRenderStateDirty", offsetof(GameObject, myRenderStateDirty), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPhysicsStateDirty", offsetof(GameObject, myPhysicsStateDirty), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myTags", offsetof(GameObject, myTags), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CoroutineManager>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeRenderComponent>();
	currentClass->AddField(Field("myHeightfield", offsetof(LandscapeRenderComponent, myHeightfield), reflectionSystem.GetClass<Heightfield>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myVertexBuffer", offsetof(LandscapeRenderComponent, myVertexBuffer), reflectionSystem.GetClass<VertexBufferHandle *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIndexBuffer", offsetof(LandscapeRenderComponent, myIndexBuffer), reflectionSystem.GetClass<IndexBufferHandle *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMesh", offsetof(LandscapeRenderComponent, myMesh), reflectionSystem.GetClass<Mesh *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMeshInstance", offsetof(LandscapeRenderComponent, myMeshInstance), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMaterial", offsetof(LandscapeRenderComponent, myMaterial), reflectionSystem.GetClass<Material *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BoxColliderComponent>();
	currentClass->AddField(Field("myHalfSize", offsetof(BoxColliderComponent, myHalfSize), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CameraComponent>();
	currentClass->AddField(Field("myResolution", offsetof(CameraComponent, myResolution), reflectionSystem.GetClass<glm::vec<2, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myProjection", offsetof(CameraComponent, myProjection), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFov", offsetof(CameraComponent, myFov), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNearPlane", offsetof(CameraComponent, myNearPlane), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFarPlane", offsetof(CameraComponent, myFarPlane), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIsOrthographic", offsetof(CameraComponent, myIsOrthographic), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CapsuleColliderComponent>();
	currentClass->AddField(Field("myRadius", offsetof(CapsuleColliderComponent, myRadius), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHeight", offsetof(CapsuleColliderComponent, myHeight), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GPUSceneSystem>();
	currentClass->AddField(Field("myFreeSparseIndices", offsetof(GPUSceneSystem, myFreeSparseIndices), reflectionSystem.GetClass<List<unsigned int>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNextFreeSparseIndex", offsetof(GPUSceneSystem, myNextFreeSparseIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySparseBuffer", offsetof(GPUSceneSystem, mySparseBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNumGPUObjectUpdatesThisFrame", offsetof(GPUSceneSystem, myNumGPUObjectUpdatesThisFrame), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDenseBuffer", offsetof(GPUSceneSystem, myDenseBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDenseBufferCPURepresentation", offsetof(GPUSceneSystem, myDenseBufferCPURepresentation), reflectionSystem.GetClass<List<unsigned int, unsigned int>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ForceMode>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RigidbodyComponent>();
	currentClass->AddField(Field("myActor", offsetof(RigidbodyComponent, myActor), reflectionSystem.GetClass<physx::PxRigidDynamic *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFramesSinceStartSleep", offsetof(RigidbodyComponent, myFramesSinceStartSleep), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMass", offsetof(RigidbodyComponent, myMass), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<CharacterControllerComponent>();
	currentClass->AddField(Field("myController", offsetof(CharacterControllerComponent, myController), reflectionSystem.GetClass<physx::PxController *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCollisionFlags", offsetof(CharacterControllerComponent, myCollisionFlags), reflectionSystem.GetClass<physx::PxFlags<physx::PxControllerCollisionFlag::Enum, unsigned char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFilter", offsetof(CharacterControllerComponent, myFilter), reflectionSystem.GetClass<physx::PxControllerFilters>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHeight", offsetof(CharacterControllerComponent, myHeight), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myRadius", offsetof(CharacterControllerComponent, myRadius), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySlopeLimitDegrees", offsetof(CharacterControllerComponent, mySlopeLimitDegrees), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMinDist", offsetof(CharacterControllerComponent, myMinDist), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myStepOffset", offsetof(CharacterControllerComponent, myStepOffset), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myUseGravity", offsetof(CharacterControllerComponent, myUseGravity), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDownVelocity", offsetof(CharacterControllerComponent, myDownVelocity), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myGravity", offsetof(CharacterControllerComponent, myGravity), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myTerminalVelocity", offsetof(CharacterControllerComponent, myTerminalVelocity), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDisplacement", offsetof(CharacterControllerComponent, myDisplacement), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPositionOffset", offsetof(CharacterControllerComponent, myPositionOffset), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ColliderComponent>();
	currentClass->AddField(Field("myShape", offsetof(ColliderComponent, myShape), reflectionSystem.GetClass<physx::PxShape *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myActor", offsetof(ColliderComponent, myActor), reflectionSystem.GetClass<physx::PxRigidStatic *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLocalShapePosition", offsetof(ColliderComponent, myLocalShapePosition), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLocalShapeRotation", offsetof(ColliderComponent, myLocalShapeRotation), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RenderSystem>();
	currentClass->AddField(Field("myIsUsingGPUDrivenRendering", offsetof(RenderSystem, myIsUsingGPUDrivenRendering), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCopyPipeline", offsetof(RenderSystem, myCopyPipeline), reflectionSystem.GetClass<FullscreenPipeline *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDebugPipeline", offsetof(RenderSystem, myDebugPipeline), reflectionSystem.GetClass<DebugPipeline *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myGDRPipeline", offsetof(RenderSystem, myGDRPipeline), reflectionSystem.GetClass<GDRPipeline *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySkyboxPipeline", offsetof(RenderSystem, mySkyboxPipeline), reflectionSystem.GetClass<SkyboxPipeline *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myRenderPass", offsetof(RenderSystem, myRenderPass), reflectionSystem.GetClass<vk::RenderPass>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCopyToSwapchainRenderPass", offsetof(RenderSystem, myCopyToSwapchainRenderPass), reflectionSystem.GetClass<vk::RenderPass>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myVkFrameBuffer", offsetof(RenderSystem, myVkFrameBuffer), reflectionSystem.GetClass<vk::Framebuffer>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCopyToSwapchainFrameBuffers", offsetof(RenderSystem, myCopyToSwapchainFrameBuffers), reflectionSystem.GetClass<List<vk::Framebuffer>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myClearValues", offsetof(RenderSystem, myClearValues), reflectionSystem.GetClass<vk::ClearValue[2]>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDepthBuffer", offsetof(RenderSystem, myDepthBuffer), reflectionSystem.GetClass<VulkanImage *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myRenderTexture", offsetof(RenderSystem, myRenderTexture), reflectionSystem.GetClass<VulkanImage *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myResolvedRenderTexture", offsetof(RenderSystem, myResolvedRenderTexture), reflectionSystem.GetClass<VulkanImage *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDirectionalLightShadowMap", offsetof(RenderSystem, myDirectionalLightShadowMap), reflectionSystem.GetClass<VulkanImage *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<DirectionalLightComponent>();
	currentClass->AddField(Field("myIsShadowsEnabled", offsetof(DirectionalLightComponent, myIsShadowsEnabled), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myColor", offsetof(DirectionalLightComponent, myColor), reflectionSystem.GetClass<glm::vec<4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLightProjection", offsetof(DirectionalLightComponent, myLightProjection), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<WorldSystem>();
	currentClass->AddField(Field("myWorld", offsetof(WorldSystem, myWorld), reflectionSystem.GetClass<World *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeColliderComponent>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightComponent>();
	currentClass->AddField(Field("myColor", offsetof(PointLightComponent, myColor), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIntensity", offsetof(PointLightComponent, myIntensity), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myRange", offsetof(PointLightComponent, myRange), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PromiseReturnTypeImplementation<void>>();
	currentClass->AddField(Field("myReturnValue", offsetof(PromiseReturnTypeImplementation<void>, myReturnValue), reflectionSystem.GetClass<char>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHasReturnValue", offsetof(PromiseReturnTypeImplementation<void>, myHasReturnValue), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SinWaveMovementComponent>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<String>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SphereColliderComponent>();
	currentClass->AddField(Field("myRadius", offsetof(SphereColliderComponent, myRadius), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StaticMeshComponent>();
	currentClass->AddField(Field("myPath", offsetof(StaticMeshComponent, myPath), reflectionSystem.GetClass<std::filesystem::path>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMaterials", offsetof(StaticMeshComponent, myMaterials), reflectionSystem.GetClass<List<Material *>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myModel", offsetof(StaticMeshComponent, myModel), reflectionSystem.GetClass<Model *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMeshInstances", offsetof(StaticMeshComponent, myMeshInstances), reflectionSystem.GetClass<List<unsigned int>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IsCopyable<int>>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AutoInit>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<AutoInitManager>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Field>();
	currentClass->AddField(Field("myName", offsetof(Field, myName), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myOffset", offsetof(Field, myOffset), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myType", offsetof(Field, myType), reflectionSystem.GetClass<const Class *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIsPointer", offsetof(Field, myIsPointer), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIsReference", offsetof(Field, myIsReference), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Awaitable>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadType>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Engine>();
	currentClass->AddField(Field("myFrameIndex", offsetof(Engine, myFrameIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIsRunning", offsetof(Engine, myIsRunning), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myEngineProperties", offsetof(Engine, myEngineProperties), reflectionSystem.GetClass<EngineProperties>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySystemManager", offsetof(Engine, mySystemManager), reflectionSystem.GetClass<SystemManager<System> *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPostMaster", offsetof(Engine, myPostMaster), reflectionSystem.GetClass<EventHandler *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myThreadPool", offsetof(Engine, myThreadPool), reflectionSystem.GetClass<ThreadPool *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFilewatcher", offsetof(Engine, myFilewatcher), reflectionSystem.GetClass<Filewatcher *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myWindowHandler", offsetof(Engine, myWindowHandler), reflectionSystem.GetClass<WindowHandler *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myVulkanContext", offsetof(Engine, myVulkanContext), reflectionSystem.GetClass<VulkanContext *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myAssetRegistry", offsetof(Engine, myAssetRegistry), reflectionSystem.GetClass<AssetRegistry *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myWorld", offsetof(Engine, myWorld), reflectionSystem.GetClass<World *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myExternalTickFunction", offsetof(Engine, myExternalTickFunction), reflectionSystem.GetClass<std::function<void ()>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EngineProperties>();
	currentClass->AddField(Field("Title", offsetof(EngineProperties, Title), reflectionSystem.GetClass<std::basic_string<wchar_t>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("WindowWidth", offsetof(EngineProperties, WindowWidth), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("WindowHeight", offsetof(EngineProperties, WindowHeight), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("WindowX", offsetof(EngineProperties, WindowX), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("WindowY", offsetof(EngineProperties, WindowY), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("StartupArguments", offsetof(EngineProperties, StartupArguments), reflectionSystem.GetClass<std::unordered_set<std::basic_string<char>>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventHandler>();
	currentClass->AddField(Field("myObservers", offsetof(EventHandler, myObservers), reflectionSystem.GetClass<List<EventObserver *>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventObserver>();
	currentClass->AddField(Field("myEvents", offsetof(EventObserver, myEvents), reflectionSystem.GetClass<std::map<EventType, std::function<void ()>>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<EventType>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Color>();
	currentClass->AddField(Field("myR", offsetof(Color, myR), reflectionSystem.GetClass<unsigned char>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myG", offsetof(Color, myG), reflectionSystem.GetClass<unsigned char>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myB", offsetof(Color, myB), reflectionSystem.GetClass<unsigned char>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myA", offsetof(Color, myA), reflectionSystem.GetClass<unsigned char>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsSystem>();
	currentClass->AddField(Field("myPhysicsCommands", offsetof(PhysicsSystem, myPhysicsCommands), reflectionSystem.GetClass<List<Delegate<void (physx::PxPhysics *, physx::PxScene *)>>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDefaultErrorCallback", offsetof(PhysicsSystem, myDefaultErrorCallback), reflectionSystem.GetClass<physx::PxErrorCallback *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDefaultAllocator", offsetof(PhysicsSystem, myDefaultAllocator), reflectionSystem.GetClass<physx::PxDefaultAllocator *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myToleranceScale", offsetof(PhysicsSystem, myToleranceScale), reflectionSystem.GetClass<physx::PxTolerancesScale *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFoundation", offsetof(PhysicsSystem, myFoundation), reflectionSystem.GetClass<physx::PxFoundation *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPvd", offsetof(PhysicsSystem, myPvd), reflectionSystem.GetClass<physx::PxPvd *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPvdTransport", offsetof(PhysicsSystem, myPvdTransport), reflectionSystem.GetClass<physx::PxPvdTransport *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDefaultMaterial", offsetof(PhysicsSystem, myDefaultMaterial), reflectionSystem.GetClass<physx::PxMaterial *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myListener", offsetof(PhysicsSystem, myListener), reflectionSystem.GetClass<PhysicsListener *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPhysics", offsetof(PhysicsSystem, myPhysics), reflectionSystem.GetClass<physx::PxPhysics *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myScene", offsetof(PhysicsSystem, myScene), reflectionSystem.GetClass<physx::PxScene *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myControllerManager", offsetof(PhysicsSystem, myControllerManager), reflectionSystem.GetClass<physx::PxControllerManager *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHasActiveSimulation", offsetof(PhysicsSystem, myHasActiveSimulation), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Heightfield>();
	currentClass->AddField(Field("mySeed", offsetof(Heightfield, mySeed), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPerlinNoise", offsetof(Heightfield, myPerlinNoise), reflectionSystem.GetClass<siv::BasicPerlinNoise<float>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LinearColor>();
	currentClass->AddField(Field("myR", offsetof(LinearColor, myR), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myG", offsetof(LinearColor, myG), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myB", offsetof(LinearColor, myB), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myA", offsetof(LinearColor, myA), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsListener>();
	currentClass->AddField(Field("myDequeueCollisionsDelegate", offsetof(PhysicsListener, myDequeueCollisionsDelegate), reflectionSystem.GetClass<MulticastDelegate<void ()>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCollisionPairs", offsetof(PhysicsListener, myCollisionPairs), reflectionSystem.GetClass<List<PhysicsListener::ContactPair>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myTriggerPairs", offsetof(PhysicsListener, myTriggerPairs), reflectionSystem.GetClass<List<PhysicsListener::ContactPair>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PhysicsListener::ContactPair>();
	currentClass->AddField(Field("myFirst", offsetof(PhysicsListener::ContactPair, myFirst), reflectionSystem.GetClass<GameObject *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySecond", offsetof(PhysicsListener::ContactPair, mySecond), reflectionSystem.GetClass<GameObject *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<RaycastHit>();
	currentClass->AddField(Field("myHitPosition", offsetof(RaycastHit, myHitPosition), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHitNormal", offsetof(RaycastHit, myHitNormal), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHitDistance", offsetof(RaycastHit, myHitDistance), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHitGameObject", offsetof(RaycastHit, myHitGameObject), reflectionSystem.GetClass<GameObject *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Class>();
	currentClass->AddField(Field("myFullName", offsetof(Class, myFullName), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myClassName", offsetof(Class, myClassName), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBaseClasses", offsetof(Class, myBaseClasses), reflectionSystem.GetClass<List<const Class *>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDerivedClasses", offsetof(Class, myDerivedClasses), reflectionSystem.GetClass<List<const Class *>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFields", offsetof(Class, myFields), reflectionSystem.GetClass<List<Field>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ReflectionSystem>();
	currentClass->AddField(Field("myClasses", offsetof(ReflectionSystem, myClasses), reflectionSystem.GetClass<List<Class>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<DebugPipeline>();
	currentClass->AddField(Field("myVertexShader", offsetof(DebugPipeline, myVertexShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFragmentShader", offsetof(DebugPipeline, myFragmentShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipeline", offsetof(DebugPipeline, myPipeline), reflectionSystem.GetClass<vk::Pipeline>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipelineLayout", offsetof(DebugPipeline, myPipelineLayout), reflectionSystem.GetClass<vk::PipelineLayout>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFrameDescriptorSet", offsetof(DebugPipeline, myFrameDescriptorSet), reflectionSystem.GetClass<VulkanDescriptorSet>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFrameDataBuffer", offsetof(DebugPipeline, myFrameDataBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<DebugPipeline::FrameData>();
	currentClass->AddField(Field("myToView", offsetof(DebugPipeline::FrameData, myToView), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myProjection", offsetof(DebugPipeline::FrameData, myProjection), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCameraPosition", offsetof(DebugPipeline::FrameData, myCameraPosition), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<DebugPipeline::DebugVertex>();
	currentClass->AddField(Field("myPosition", offsetof(DebugPipeline::DebugVertex, myPosition), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myColor", offsetof(DebugPipeline::DebugVertex, myColor), reflectionSystem.GetClass<Color>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<FullscreenPipeline>();
	currentClass->AddField(Field("myVertexShader", offsetof(FullscreenPipeline, myVertexShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFragmentShader", offsetof(FullscreenPipeline, myFragmentShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipeline", offsetof(FullscreenPipeline, myPipeline), reflectionSystem.GetClass<vk::Pipeline>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipelineLayout", offsetof(FullscreenPipeline, myPipelineLayout), reflectionSystem.GetClass<vk::PipelineLayout>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myRenderPass", offsetof(FullscreenPipeline, myRenderPass), reflectionSystem.GetClass<vk::RenderPass>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDescriptorSet", offsetof(FullscreenPipeline, myDescriptorSet), reflectionSystem.GetClass<VulkanDescriptorSet>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline>();
	currentClass->AddField(Field("myPrePassShader", offsetof(GDRPipeline, myPrePassShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCullShader", offsetof(GDRPipeline, myCullShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCubemap", offsetof(GDRPipeline, myCubemap), reflectionSystem.GetClass<TextureCube *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPrePass", offsetof(GDRPipeline, myPrePass), reflectionSystem.GetClass<GDRPipeline::ComputePassResources>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCullPass", offsetof(GDRPipeline, myCullPass), reflectionSystem.GetClass<GDRPipeline::ComputePassResources>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIndirectCommandsBuffer", offsetof(GDRPipeline, myIndirectCommandsBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCountBuffer", offsetof(GDRPipeline, myCountBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPerDrawDataBuffer", offsetof(GDRPipeline, myPerDrawDataBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFrameDescriptorSet", offsetof(GDRPipeline, myFrameDescriptorSet), reflectionSystem.GetClass<VulkanDescriptorSet>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipelineLayout", offsetof(GDRPipeline, myPipelineLayout), reflectionSystem.GetClass<vk::PipelineLayout>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipeline", offsetof(GDRPipeline, myPipeline), reflectionSystem.GetClass<vk::Pipeline>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myVertexShader", offsetof(GDRPipeline, myVertexShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFragmentShader", offsetof(GDRPipeline, myFragmentShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFrameDataBuffer", offsetof(GDRPipeline, myFrameDataBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDirectionalLightBuffer", offsetof(GDRPipeline, myDirectionalLightBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDirtyTransforms", offsetof(GDRPipeline, myDirtyTransforms), reflectionSystem.GetClass<List<TransformComponent *>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline::ComputePassResources>();
	currentClass->AddField(Field("myShader", offsetof(GDRPipeline::ComputePassResources, myShader), reflectionSystem.GetClass<Shader *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipeline", offsetof(GDRPipeline::ComputePassResources, myPipeline), reflectionSystem.GetClass<vk::Pipeline>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipelineLayout", offsetof(GDRPipeline::ComputePassResources, myPipelineLayout), reflectionSystem.GetClass<vk::PipelineLayout>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDescriptorSet", offsetof(GDRPipeline::ComputePassResources, myDescriptorSet), reflectionSystem.GetClass<VulkanDescriptorSet *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline::FrameData>();
	currentClass->AddField(Field("myToView", offsetof(GDRPipeline::FrameData, myToView), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myProjection", offsetof(GDRPipeline::FrameData, myProjection), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCameraPosition", offsetof(GDRPipeline::FrameData, myCameraPosition), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCubemapIndex", offsetof(GDRPipeline::FrameData, myCubemapIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline::DirectionalLightBuffer>();
	currentClass->AddField(Field("myColor", offsetof(GDRPipeline::DirectionalLightBuffer, myColor), reflectionSystem.GetClass<glm::vec<4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDirection", offsetof(GDRPipeline::DirectionalLightBuffer, myDirection), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("padding", offsetof(GDRPipeline::DirectionalLightBuffer, padding), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLightView", offsetof(GDRPipeline::DirectionalLightBuffer, myLightView), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLightProjection", offsetof(GDRPipeline::DirectionalLightBuffer, myLightProjection), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferHandle>();
	currentClass->AddField(Field("myIndex", offsetof(IndexBufferHandle, myIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferSystem>();
	currentClass->AddField(Field("myUsedBufferSize", offsetof(IndexBufferSystem, myUsedBufferSize), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCurrentIndexOffset", offsetof(IndexBufferSystem, myCurrentIndexOffset), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIndexBuffers", offsetof(IndexBufferSystem, myIndexBuffers), reflectionSystem.GetClass<List<IndexBufferHandle *>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBuffer", offsetof(IndexBufferSystem, myBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySparseIndexDataBuffer", offsetof(IndexBufferSystem, mySparseIndexDataBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySparseIndexData_CPURepresentation", offsetof(IndexBufferSystem, mySparseIndexData_CPURepresentation), reflectionSystem.GetClass<List<IndexBufferData>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFreeSparseIndices", offsetof(IndexBufferSystem, myFreeSparseIndices), reflectionSystem.GetClass<List<unsigned int>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Mesh>();
	currentClass->AddField(Field("myVertexBuffer", offsetof(Mesh, myVertexBuffer), reflectionSystem.GetClass<VertexBufferHandle *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIndexBuffer", offsetof(Mesh, myIndexBuffer), reflectionSystem.GetClass<IndexBufferHandle *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySphereBounds", offsetof(Mesh, mySphereBounds), reflectionSystem.GetClass<glm::vec<4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHandle", offsetof(Mesh, myHandle), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myAlbedoPath", offsetof(Mesh, myAlbedoPath), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNormalPath", offsetof(Mesh, myNormalPath), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMaterialPath", offsetof(Mesh, myMaterialPath), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshSystem>();
	currentClass->AddField(Field("myMeshes", offsetof(MeshSystem, myMeshes), reflectionSystem.GetClass<List<Mesh *>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBuffer", offsetof(MeshSystem, myBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNumObjects", offsetof(MeshSystem, myNumObjects), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferHandle>();
	currentClass->AddField(Field("myIndex", offsetof(VertexBufferHandle, myIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshUtils>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<TextureSystem>();
	currentClass->AddField(Field("myDescriptorPool", offsetof(TextureSystem, myDescriptorPool), reflectionSystem.GetClass<vk::DescriptorPool>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryWriter>();
	currentClass->AddField(Field("myStream", offsetof(BinaryWriter, myStream), reflectionSystem.GetClass<std::basic_ofstream<char>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Vertex>();
	currentClass->AddField(Field("myPosition", offsetof(Vertex, myPosition), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myColor", offsetof(Vertex, myColor), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNormal", offsetof(Vertex, myNormal), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myTangents", offsetof(Vertex, myTangents), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBinormals", offsetof(Vertex, myBinormals), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myTexCoords", offsetof(Vertex, myTexCoords), reflectionSystem.GetClass<glm::vec<2, float>[2]>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferSystem>();
	currentClass->AddField(Field("myUsedBufferSize", offsetof(VertexBufferSystem, myUsedBufferSize), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCurrentVertexOffset", offsetof(VertexBufferSystem, myCurrentVertexOffset), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myVertexBuffers", offsetof(VertexBufferSystem, myVertexBuffers), reflectionSystem.GetClass<List<VertexBufferHandle *>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBuffer", offsetof(VertexBufferSystem, myBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySparseVertexDataBuffer", offsetof(VertexBufferSystem, mySparseVertexDataBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySparseVertexData_CPURepresentation", offsetof(VertexBufferSystem, mySparseVertexData_CPURepresentation), reflectionSystem.GetClass<List<VertexBufferData>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFreeSparseIndices", offsetof(VertexBufferSystem, myFreeSparseIndices), reflectionSystem.GetClass<List<unsigned int>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryReader>();
	currentClass->AddField(Field("myBuffer", offsetof(BinaryReader, myBuffer), reflectionSystem.GetClass<List<unsigned char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myReadOffset", offsetof(BinaryReader, myReadOffset), reflectionSystem.GetClass<unsigned long long>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshData>();
	currentClass->AddField(Field("myBoundingSphereModelSpace", offsetof(MeshData, myBoundingSphereModelSpace), reflectionSystem.GetClass<glm::vec<4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myVertexIndex", offsetof(MeshData, myVertexIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIndexDataIndex", offsetof(MeshData, myIndexDataIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMaterialIndex", offsetof(MeshData, myMaterialIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferData>();
	currentClass->AddField(Field("myOffset", offsetof(VertexBufferData, myOffset), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferData>();
	currentClass->AddField(Field("myOffset", offsetof(IndexBufferData, myOffset), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCount", offsetof(IndexBufferData, myCount), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MeshInstanceData>();
	currentClass->AddField(Field("myToWorld", offsetof(MeshInstanceData, myToWorld), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMeshIndex", offsetof(MeshInstanceData, myMeshIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myAlbedoIndex", offsetof(MeshInstanceData, myAlbedoIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNormalIndex", offsetof(MeshInstanceData, myNormalIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMaterialIndex", offsetof(MeshInstanceData, myMaterialIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PerDrawData>();
	currentClass->AddField(Field("myToWorld", offsetof(PerDrawData, myToWorld), reflectionSystem.GetClass<glm::mat<4, 4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myAlbedoIndex", offsetof(PerDrawData, myAlbedoIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNormalIndex", offsetof(PerDrawData, myNormalIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMaterialIndex", offsetof(PerDrawData, myMaterialIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("padding", offsetof(PerDrawData, padding), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightData>();
	currentClass->AddField(Field("myColor", offsetof(PointLightData, myColor), reflectionSystem.GetClass<glm::vec<4, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPosition", offsetof(PointLightData, myPosition), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myRange", offsetof(PointLightData, myRange), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIntensity", offsetof(PointLightData, myIntensity), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<System>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<LandscapeSystem>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SamplerMode>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanUtils>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PointLightSystem>();
	currentClass->AddField(Field("myBuffer", offsetof(PointLightSystem, myBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNumPointLights", offsetof(PointLightSystem, myNumPointLights), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<BinaryUtils>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Debug>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Debug::DrawLineInfos>();
	currentClass->AddField(Field("myStart", offsetof(Debug::DrawLineInfos, myStart), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myEnd", offsetof(Debug::DrawLineInfos, myEnd), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myColor", offsetof(Debug::DrawLineInfos, myColor), reflectionSystem.GetClass<Color>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<MathUtils>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<OnScopeExit>();
	currentClass->AddField(Field("myOnScopeExit", offsetof(OnScopeExit, myOnScopeExit), reflectionSystem.GetClass<std::function<void ()>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ThreadUtils>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<NvidiaAftermathTracker>();
	currentClass->AddField(Field("m_initialized", offsetof(NvidiaAftermathTracker, m_initialized), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("m_mutex", offsetof(NvidiaAftermathTracker, m_mutex), reflectionSystem.GetClass<std::mutex>(), false, false /* Implement this*/));
	currentClass->AddField(Field("m_shaderDebugInfo", offsetof(NvidiaAftermathTracker, m_shaderDebugInfo), reflectionSystem.GetClass<std::map<GFSDK_Aftermath_ShaderDebugInfoIdentifier, std::vector<unsigned char>>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("m_shaderDatabase", offsetof(NvidiaAftermathTracker, m_shaderDatabase), reflectionSystem.GetClass<ShaderDatabase>(), false, false /* Implement this*/));
	currentClass->AddField(Field("m_markerMap", offsetof(NvidiaAftermathTracker, m_markerMap), reflectionSystem.GetClass<const std::array<std::map<unsigned long long, std::basic_string<char>>, 4> &>(), false, false /* Implement this*/));
	currentClass->AddField(Field("m_AftermathFlags", offsetof(NvidiaAftermathTracker, m_AftermathFlags), reflectionSystem.GetClass<const unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ShaderDatabase>();
	currentClass->AddField(Field("m_shaderBinaries", offsetof(ShaderDatabase, m_shaderBinaries), reflectionSystem.GetClass<std::map<GFSDK_Aftermath_ShaderBinaryHash, std::vector<unsigned char>>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("m_shaderBinariesWithDebugInfo", offsetof(ShaderDatabase, m_shaderBinariesWithDebugInfo), reflectionSystem.GetClass<std::map<GFSDK_Aftermath_ShaderDebugName, std::vector<unsigned char>>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<ResizableBuffer>();
	currentClass->AddField(Field("OnBufferResized", offsetof(ResizableBuffer, OnBufferResized), reflectionSystem.GetClass<MulticastDelegate<void ()>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHasActiveUpload", offsetof(ResizableBuffer, myHasActiveUpload), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHasRegisteredForTick", offsetof(ResizableBuffer, myHasRegisteredForTick), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBuffer", offsetof(ResizableBuffer, myBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StagingBuffer>();
	currentClass->AddField(Field("myUnderlyingBuffer", offsetof(StagingBuffer, myUnderlyingBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySize", offsetof(StagingBuffer, mySize), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myOffset", offsetof(StagingBuffer, myOffset), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StagingSystem>();
	currentClass->AddField(Field("myLastFrameIndex", offsetof(StagingSystem, myLastFrameIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCurrentStageData", offsetof(StagingSystem, myCurrentStageData), reflectionSystem.GetClass<StagingSystem::BufferData *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myStagingBuffers", offsetof(StagingSystem, myStagingBuffers), reflectionSystem.GetClass<std::array<StagingSystem::BufferData, 3>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<StagingSystem::BufferData>();
	currentClass->AddField(Field("myBuffer", offsetof(StagingSystem::BufferData, myBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myOffset", offsetof(StagingSystem::BufferData, myOffset), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanAllocator>();
	currentClass->AddField(Field("myAllocator", offsetof(VulkanAllocator, myAllocator), reflectionSystem.GetClass<VmaAllocator_T *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBufferDeleteData", offsetof(VulkanAllocator, myBufferDeleteData), reflectionSystem.GetClass<MutexList<VulkanAllocator::DeleteData<VulkanBuffer>>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myResizableBufferDeleteData", offsetof(VulkanAllocator, myResizableBufferDeleteData), reflectionSystem.GetClass<MutexList<VulkanAllocator::DeleteData<ResizableBuffer>>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myImageDeleteData", offsetof(VulkanAllocator, myImageDeleteData), reflectionSystem.GetClass<MutexList<VulkanAllocator::DeleteData<VulkanImage>>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDelegateDeletes", offsetof(VulkanAllocator, myDelegateDeletes), reflectionSystem.GetClass<MutexList<VulkanAllocator::DelegateDeleteData>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanAllocator::DelegateDeleteData>();
	currentClass->AddField(Field("myFramesUntilDelete", offsetof(VulkanAllocator::DelegateDeleteData, myFramesUntilDelete), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDelegate", offsetof(VulkanAllocator::DelegateDeleteData, myDelegate), reflectionSystem.GetClass<Delegate<void ()>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanBuffer>();
	currentClass->AddField(Field("myAllocation", offsetof(VulkanBuffer, myAllocation), reflectionSystem.GetClass<VmaAllocation_T *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myBuffer", offsetof(VulkanBuffer, myBuffer), reflectionSystem.GetClass<vk::Buffer>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCreateInfo", offsetof(VulkanBuffer, myCreateInfo), reflectionSystem.GetClass<vk::BufferCreateInfo>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMemoryUsage", offsetof(VulkanBuffer, myMemoryUsage), reflectionSystem.GetClass<VmaMemoryUsage>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myIsMappingAllowed", offsetof(VulkanBuffer, myIsMappingAllowed), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPtr", offsetof(VulkanBuffer, myPtr), reflectionSystem.GetClass<void *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanContext>();
	currentClass->AddField(Field("myVulkanInstance", offsetof(VulkanContext, myVulkanInstance), reflectionSystem.GetClass<vk::Instance>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDebugMessenger", offsetof(VulkanContext, myDebugMessenger), reflectionSystem.GetClass<vk::DebugUtilsMessengerEXT>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPipelineCache", offsetof(VulkanContext, myPipelineCache), reflectionSystem.GetClass<vk::PipelineCache>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPhysicalDevice", offsetof(VulkanContext, myPhysicalDevice), reflectionSystem.GetClass<VulkanPhysicalDevice *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDevice", offsetof(VulkanContext, myDevice), reflectionSystem.GetClass<VulkanDevice *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myAllocator", offsetof(VulkanContext, myAllocator), reflectionSystem.GetClass<VulkanAllocator *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySwapChain", offsetof(VulkanContext, mySwapChain), reflectionSystem.GetClass<VulkanSwapChain *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("markerMap", offsetof(VulkanContext, markerMap), reflectionSystem.GetClass<std::array<std::map<unsigned long long, std::basic_string<char>>, 4>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNvidiaAftermathDebugger", offsetof(VulkanContext, myNvidiaAftermathDebugger), reflectionSystem.GetClass<NvidiaAftermathTracker *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDescriptorPool", offsetof(VulkanContext, myDescriptorPool), reflectionSystem.GetClass<vk::DescriptorPool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myExtensions", offsetof(VulkanContext, myExtensions), reflectionSystem.GetClass<const List<const char *>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLayers", offsetof(VulkanContext, myLayers), reflectionSystem.GetClass<const List<const char *>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanDescriptorSet>();
	currentClass->AddField(Field("myBuffers", offsetof(VulkanDescriptorSet, myBuffers), reflectionSystem.GetClass<List<VulkanDescriptorSet::BindingData<const VulkanBuffer *>>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myResizableBuffer", offsetof(VulkanDescriptorSet, myResizableBuffer), reflectionSystem.GetClass<List<VulkanDescriptorSet::BindingData<const ResizableBuffer *>>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myImages", offsetof(VulkanDescriptorSet, myImages), reflectionSystem.GetClass<List<VulkanDescriptorSet::BindingData<const VulkanImage *>>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myUsesSharedLayout", offsetof(VulkanDescriptorSet, myUsesSharedLayout), reflectionSystem.GetClass<bool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myLayout", offsetof(VulkanDescriptorSet, myLayout), reflectionSystem.GetClass<vk::DescriptorSetLayout>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySet", offsetof(VulkanDescriptorSet, mySet), reflectionSystem.GetClass<vk::DescriptorSet>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanDevice>();
	currentClass->AddField(Field("myPhysicalDevice", offsetof(VulkanDevice, myPhysicalDevice), reflectionSystem.GetClass<const VulkanPhysicalDevice &>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDevice", offsetof(VulkanDevice, myDevice), reflectionSystem.GetClass<vk::Device>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myGraphicsQueue", offsetof(VulkanDevice, myGraphicsQueue), reflectionSystem.GetClass<vk::Queue>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myComputeQueue", offsetof(VulkanDevice, myComputeQueue), reflectionSystem.GetClass<vk::Queue>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myTransferQueue", offsetof(VulkanDevice, myTransferQueue), reflectionSystem.GetClass<vk::Queue>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCommandPoolsMutex", offsetof(VulkanDevice, myCommandPoolsMutex), reflectionSystem.GetClass<std::recursive_mutex>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCommandPools", offsetof(VulkanDevice, myCommandPools), reflectionSystem.GetClass<std::map<std::thread::id, vk::CommandPool>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myComputeCommandPools", offsetof(VulkanDevice, myComputeCommandPools), reflectionSystem.GetClass<std::map<std::thread::id, vk::CommandPool>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanImGui>();
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanImage>();
	currentClass->AddField(Field("myAllocation", offsetof(VulkanImage, myAllocation), reflectionSystem.GetClass<VmaAllocation_T *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myImage", offsetof(VulkanImage, myImage), reflectionSystem.GetClass<vk::Image>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myView", offsetof(VulkanImage, myView), reflectionSystem.GetClass<vk::ImageView>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFormat", offsetof(VulkanImage, myFormat), reflectionSystem.GetClass<vk::Format>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySize", offsetof(VulkanImage, mySize), reflectionSystem.GetClass<glm::vec<2, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNumMipLevels", offsetof(VulkanImage, myNumMipLevels), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myNumMSAASamples", offsetof(VulkanImage, myNumMSAASamples), reflectionSystem.GetClass<vk::SampleCountFlagBits>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanPhysicalDevice>();
	currentClass->AddField(Field("myDevice", offsetof(VulkanPhysicalDevice, myDevice), reflectionSystem.GetClass<vk::PhysicalDevice>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFeatures", offsetof(VulkanPhysicalDevice, myFeatures), reflectionSystem.GetClass<vk::PhysicalDeviceFeatures2>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMemoryProperties", offsetof(VulkanPhysicalDevice, myMemoryProperties), reflectionSystem.GetClass<vk::PhysicalDeviceMemoryProperties>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myQueueFamilyProperties", offsetof(VulkanPhysicalDevice, myQueueFamilyProperties), reflectionSystem.GetClass<List<vk::QueueFamilyProperties>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMSAASampleCount", offsetof(VulkanPhysicalDevice, myMSAASampleCount), reflectionSystem.GetClass<vk::SampleCountFlagBits>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myGraphicsQueueIndex", offsetof(VulkanPhysicalDevice, myGraphicsQueueIndex), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myComputeQueueIndex", offsetof(VulkanPhysicalDevice, myComputeQueueIndex), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myTransferQueueIndex", offsetof(VulkanPhysicalDevice, myTransferQueueIndex), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPresentQueueIndex", offsetof(VulkanPhysicalDevice, myPresentQueueIndex), reflectionSystem.GetClass<int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDeviceExtensions", offsetof(VulkanPhysicalDevice, myDeviceExtensions), reflectionSystem.GetClass<List<const char *>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanShaderIncluder>();
	currentClass->AddField(Field("myActiveIncludes", offsetof(VulkanShaderIncluder, myActiveIncludes), reflectionSystem.GetClass<List<VulkanShaderIncluder::IncludeData>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanShaderIncluder::IncludeData>();
	currentClass->AddField(Field("sourceName", offsetof(VulkanShaderIncluder::IncludeData, sourceName), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("content", offsetof(VulkanShaderIncluder::IncludeData, content), reflectionSystem.GetClass<std::basic_string<char>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("ptr", offsetof(VulkanShaderIncluder::IncludeData, ptr), reflectionSystem.GetClass<shaderc_include_result *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<VulkanSwapChain>();
	currentClass->AddField(Field("myDevice", offsetof(VulkanSwapChain, myDevice), reflectionSystem.GetClass<const VulkanDevice &>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myWindowSurface", offsetof(VulkanSwapChain, myWindowSurface), reflectionSystem.GetClass<vk::SurfaceKHR>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFormat", offsetof(VulkanSwapChain, myFormat), reflectionSystem.GetClass<vk::Format>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFences", offsetof(VulkanSwapChain, myFences), reflectionSystem.GetClass<List<vk::Fence>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myImageAcquiredSemaphores", offsetof(VulkanSwapChain, myImageAcquiredSemaphores), reflectionSystem.GetClass<List<vk::Semaphore>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myDrawCompleteSemaphores", offsetof(VulkanSwapChain, myDrawCompleteSemaphores), reflectionSystem.GetClass<List<vk::Semaphore>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myFrameIndex", offsetof(VulkanSwapChain, myFrameIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySyncIndex", offsetof(VulkanSwapChain, mySyncIndex), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySwapChain", offsetof(VulkanSwapChain, mySwapChain), reflectionSystem.GetClass<vk::SwapchainKHR>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySwapChainWidth", offsetof(VulkanSwapChain, mySwapChainWidth), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySwapChainHeight", offsetof(VulkanSwapChain, mySwapChainHeight), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMinImageCount", offsetof(VulkanSwapChain, myMinImageCount), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myImages", offsetof(VulkanSwapChain, myImages), reflectionSystem.GetClass<List<vk::Image>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myImageViews", offsetof(VulkanSwapChain, myImageViews), reflectionSystem.GetClass<List<vk::ImageView>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCommandPool", offsetof(VulkanSwapChain, myCommandPool), reflectionSystem.GetClass<vk::CommandPool>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCommandBuffers", offsetof(VulkanSwapChain, myCommandBuffers), reflectionSystem.GetClass<List<vk::CommandBuffer>>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<WindowHandler>();
	currentClass->AddField(Field("myHWND", offsetof(WindowHandler, myHWND), reflectionSystem.GetClass<HWND__ *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHInstance", offsetof(WindowHandler, myHInstance), reflectionSystem.GetClass<HINSTANCE__ *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<World>();
	currentClass->AddField(Field("myAssetRegistry", offsetof(World, myAssetRegistry), reflectionSystem.GetClass<AssetRegistry *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySystemManager", offsetof(World, mySystemManager), reflectionSystem.GetClass<SystemManager<WorldSystem> *>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myCactus", offsetof(World, myCactus), reflectionSystem.GetClass<GameObject *>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PlayerCameraControllerComponent>();
	currentClass->AddField(Field("myMouseSensitivity", offsetof(PlayerCameraControllerComponent, myMouseSensitivity), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySpringArmChangeAmount", offsetof(PlayerCameraControllerComponent, mySpringArmChangeAmount), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMinSpringArmLength", offsetof(PlayerCameraControllerComponent, myMinSpringArmLength), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myMaxSpringArmLength", offsetof(PlayerCameraControllerComponent, myMaxSpringArmLength), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myPitch", offsetof(PlayerCameraControllerComponent, myPitch), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myYaw", offsetof(PlayerCameraControllerComponent, myYaw), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myStoredPitch", offsetof(PlayerCameraControllerComponent, myStoredPitch), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myStoredYaw", offsetof(PlayerCameraControllerComponent, myStoredYaw), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myStoredSpringArmLength", offsetof(PlayerCameraControllerComponent, myStoredSpringArmLength), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<PlayerComponent>();
	currentClass->AddField(Field("mySpeed", offsetof(PlayerComponent, mySpeed), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("mySprintSpeed", offsetof(PlayerComponent, mySprintSpeed), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myJumpForce", offsetof(PlayerComponent, myJumpForce), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<SpringArmComponent>();
	currentClass->AddField(Field("myLength", offsetof(SpringArmComponent, myLength), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myOffset", offsetof(SpringArmComponent, myOffset), reflectionSystem.GetClass<glm::vec<3, float>>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myExclusionTags", offsetof(SpringArmComponent, myExclusionTags), reflectionSystem.GetClass<unsigned int>(), false, false /* Implement this*/));
	currentClass->AddField(Field("myHitOffset", offsetof(SpringArmComponent, myHitOffset), reflectionSystem.GetClass<float>(), false, false /* Implement this*/));
	// Base classes are not implemented yet
}
{ 
	Class* currentClass = reflectionSystem.GetMutableClass<Game>();
	// Base classes are not implemented yet
}

        }
    }
};
#undef private
#undef protected
