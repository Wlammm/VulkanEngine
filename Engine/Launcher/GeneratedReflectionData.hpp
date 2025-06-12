#pragma once
#include "Engine/Utils/StdIncludes.hpp"
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/ReflectionSystem.h"


#define private public
#define protected public

// BEGIN INCLUDES FOR REFLECTED TYPES
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Engine.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\EnginePch.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Animation\Skeleton.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\AssetRegistry\Asset.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\AssetRegistry\AssetContainer.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\AssetRegistry\AssetRegistry.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Assets\JsonAsset.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Assets\Material.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Assets\Model.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Assets\Shader.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Assets\Texture.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Assets\TextureCube.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\BoxColliderComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\CameraComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\CapsuleColliderComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\CharacterControllerComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\ColliderComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\ConvexColliderComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\DirectionalLightComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\EditorCameraMovementComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\LandscapeColliderComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\LandscapeRenderComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\MeshColliderComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\PointLightComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\RigidbodyComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\SinWaveMovementComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\SphereColliderComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\StaticMeshComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Components\TransformComponent.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\ComponentSystem\Component.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\ComponentSystem\ComponentArray.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\ComponentSystem\ComponentSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\ComponentSystem\GameObject.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Containers\SegmentedList.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Core\AutoInit.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Core\AutoInitManager.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Core\Console.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Core\Filewatcher.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Core\Input.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Core\ThreadPool.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Core\Time.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Coroutines\Awaitable.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Coroutines\Coroutine.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Coroutines\CoroutineManager.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Events\EventHandler.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Events\EventObserver.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Math\Color.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Math\Heightfield.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Math\LinearColor.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Physics\PhysicsErrorCallback.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Physics\PhysicsListener.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Physics\PhysicsQueryStructs.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Physics\PhysicsSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Physics\PhysXInclude.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Reflection\Class.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Reflection\Field.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Reflection\ReflectionSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\DebugPipeline.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\FullscreenPipeline.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\GDRPipeline.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\IndexBufferHandle.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\IndexBufferSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\Mesh.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\MeshSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\MeshUtils.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\RenderSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\SkyboxPipeline.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\TextureSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\VertexBufferHandle.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\VertexBufferSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Serialization\BinaryReader.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Serialization\BinaryWriter.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\System\System.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\System\WorldSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Systems\LandscapeSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Systems\PointLightSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Utils\CommonMacros.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Utils\Debug.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Utils\OnScopeExit.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\GPUSceneSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\ResizableBuffer.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanAllocator.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanBuffer.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanContext.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanDescriptorSet.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanDevice.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanImage.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanImGui.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanPhysicalDevice.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanShaderIncluder.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanSwapChain.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Windows\WindowHandler.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\World\World.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\Aftermath\NvidiaAftermathTracker.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\Aftermath\ShaderDatabase.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\Staging\StagingBuffer.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\Staging\StagingSystem.h"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\EngineProperties.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\ComponentSystem\ComponentDefines.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Containers\ContainerTypes.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Containers\List.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Containers\MutexList.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Core\CheckDefine.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Core\EngineDefines.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Delegates\Delegate.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Delegates\MulticastDelegate.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Events\EventTypes.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Math\GlmUtils.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Rendering\Vertex.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Shaders\MeshStructs.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\System\SystemManager.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Utils\BinaryUtils.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Utils\HashUtils.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Utils\MathUtils.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Utils\StdIncludes.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Utils\String.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Utils\ThreadUtils.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanDynamicBuffer.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Vulkan\VulkanUtils.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Delegates\Internal\ConstMemberFuncCtor.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Delegates\Internal\FreeFuncCtor.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Delegates\Internal\FuncCtor.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Delegates\Internal\LambdaFuncCtor.hpp"
#include "C:\Users\William\Documents\GitHub\VulkanEngine\Engine\Engine\Delegates\Internal\MemberFuncCtor.hpp"
// END INCLUDES FOR REFLECTED TYPES

class GeneratedReflectionData
{
public:
	static void RegisterReflectionData(Engine& inEngine)
	{
		ReflectionSystem& reflectionSystem = inEngine.GetReflectionSystem(); 


		// Create all classes.
		{
			reflectionSystem.AddClass("Color", typeid(Color).name());
			reflectionSystem.AddClass("LinearColor", typeid(LinearColor).name());
			reflectionSystem.AddClass("Console", typeid(Console).name());
			reflectionSystem.AddClass("OnScopeExit", typeid(OnScopeExit).name());
			reflectionSystem.AddClass("String", typeid(String).name());
			reflectionSystem.AddClass("EngineProperties", typeid(EngineProperties).name());
			reflectionSystem.AddClass("Engine", typeid(Engine).name());
			reflectionSystem.AddClass("WindowHandler", typeid(WindowHandler).name());
			reflectionSystem.AddClass("EventHandler", typeid(EventHandler).name());
			reflectionSystem.AddClass("AssetRegistry", typeid(AssetRegistry).name());
			reflectionSystem.AddClass("ThreadPool", typeid(ThreadPool).name());
			reflectionSystem.AddClass("Filewatcher", typeid(Filewatcher).name());
			reflectionSystem.AddClass("World", typeid(World).name());
			reflectionSystem.AddClass("VulkanContext", typeid(VulkanContext).name());
			reflectionSystem.AddClass("Skeleton", typeid(Skeleton).name());
			reflectionSystem.AddClass("Skeleton::Bone", typeid(Skeleton::Bone).name());
			reflectionSystem.AddClass("Asset", typeid(Asset).name());
			reflectionSystem.AddClass("IAssetContainer", typeid(IAssetContainer).name());
			reflectionSystem.AddClass("ThreadUtils", typeid(ThreadUtils).name());
			reflectionSystem.AddClass("JsonAsset", typeid(JsonAsset).name());
			reflectionSystem.AddClass("VulkanAllocator", typeid(VulkanAllocator).name());
			reflectionSystem.AddClass("VulkanPhysicalDevice", typeid(VulkanPhysicalDevice).name());
			reflectionSystem.AddClass("VulkanDevice", typeid(VulkanDevice).name());
			reflectionSystem.AddClass("VulkanBuffer", typeid(VulkanBuffer).name());
			reflectionSystem.AddClass("ResizableBuffer", typeid(ResizableBuffer).name());
			reflectionSystem.AddClass("VulkanImage", typeid(VulkanImage).name());
			reflectionSystem.AddClass("VulkanAllocator::DelegateDeleteData", typeid(VulkanAllocator::DelegateDeleteData).name());
			reflectionSystem.AddClass("Vertex", typeid(Vertex).name());
			reflectionSystem.AddClass("EventObserver", typeid(EventObserver).name());
			reflectionSystem.AddClass("System", typeid(System).name());
			reflectionSystem.AddClass("RenderSystem", typeid(RenderSystem).name());
			reflectionSystem.AddClass("GDRPipeline", typeid(GDRPipeline).name());
			reflectionSystem.AddClass("FullscreenPipeline", typeid(FullscreenPipeline).name());
			reflectionSystem.AddClass("DebugPipeline", typeid(DebugPipeline).name());
			reflectionSystem.AddClass("SkyboxPipeline", typeid(SkyboxPipeline).name());
			reflectionSystem.AddClass("MathUtils", typeid(MathUtils).name());
			reflectionSystem.AddClass("IVulkanDynamicBuffer", typeid(IVulkanDynamicBuffer).name());
			reflectionSystem.AddClass("VulkanDescriptorSet", typeid(VulkanDescriptorSet).name());
			reflectionSystem.AddClass("Material", typeid(Material).name());
			reflectionSystem.AddClass("SerializationMeshData", typeid(SerializationMeshData).name());
			reflectionSystem.AddClass("Model", typeid(Model).name());
			reflectionSystem.AddClass("Filewatcher::CallbackHandle", typeid(Filewatcher::CallbackHandle).name());
			reflectionSystem.AddClass("Filewatcher::FileData", typeid(Filewatcher::FileData).name());
			reflectionSystem.AddClass("Shader", typeid(Shader).name());
			reflectionSystem.AddClass("Shader::IncludeFileData", typeid(Shader::IncludeFileData).name());
			reflectionSystem.AddClass("ImageData", typeid(ImageData).name());
			reflectionSystem.AddClass("Texture", typeid(Texture).name());
			reflectionSystem.AddClass("TextureCube", typeid(TextureCube).name());
			reflectionSystem.AddClass("IComponentArray", typeid(IComponentArray).name());
			reflectionSystem.AddClass("Component", typeid(Component).name());
			reflectionSystem.AddClass("WorldSystem", typeid(WorldSystem).name());
			reflectionSystem.AddClass("ComponentSystem", typeid(ComponentSystem).name());
			reflectionSystem.AddClass("GameObject", typeid(GameObject).name());
			reflectionSystem.AddClass("RigidbodyComponent", typeid(RigidbodyComponent).name());
			reflectionSystem.AddClass("ColliderComponent", typeid(ColliderComponent).name());
			reflectionSystem.AddClass("BoxColliderComponent", typeid(BoxColliderComponent).name());
			reflectionSystem.AddClass("CameraComponent", typeid(CameraComponent).name());
			reflectionSystem.AddClass("CapsuleColliderComponent", typeid(CapsuleColliderComponent).name());
			reflectionSystem.AddClass("CharacterControllerComponent", typeid(CharacterControllerComponent).name());
			reflectionSystem.AddClass("ConvexColliderComponent", typeid(ConvexColliderComponent).name());
			reflectionSystem.AddClass("DirectionalLightComponent", typeid(DirectionalLightComponent).name());
			reflectionSystem.AddClass("EditorCameraMovementComponent", typeid(EditorCameraMovementComponent).name());
			reflectionSystem.AddClass("LandscapeColliderComponent", typeid(LandscapeColliderComponent).name());
			reflectionSystem.AddClass("Heightfield", typeid(Heightfield).name());
			reflectionSystem.AddClass("GPUSceneSystem", typeid(GPUSceneSystem).name());
			reflectionSystem.AddClass("LandscapeRenderComponent", typeid(LandscapeRenderComponent).name());
			reflectionSystem.AddClass("MeshColliderComponent", typeid(MeshColliderComponent).name());
			reflectionSystem.AddClass("PointLightComponent", typeid(PointLightComponent).name());
			reflectionSystem.AddClass("SinWaveMovementComponent", typeid(SinWaveMovementComponent).name());
			reflectionSystem.AddClass("SphereColliderComponent", typeid(SphereColliderComponent).name());
			reflectionSystem.AddClass("StaticMeshComponent", typeid(StaticMeshComponent).name());
			reflectionSystem.AddClass("TransformComponent", typeid(TransformComponent).name());
			reflectionSystem.AddClass("AutoInit", typeid(AutoInit).name());
			reflectionSystem.AddClass("AutoInitManager", typeid(AutoInitManager).name());
			reflectionSystem.AddClass("Input", typeid(Input).name());
			reflectionSystem.AddClass("Input::KeyState", typeid(Input::KeyState).name());
			reflectionSystem.AddClass("Time", typeid(Time).name());
			reflectionSystem.AddClass("Awaitable", typeid(Awaitable).name());
			reflectionSystem.AddClass("Awaitables::SwitchToThread", typeid(Awaitables::SwitchToThread).name());
			reflectionSystem.AddClass("Awaitables::WaitForSeconds", typeid(Awaitables::WaitForSeconds).name());
			reflectionSystem.AddClass("CoroutineManager", typeid(CoroutineManager).name());
			reflectionSystem.AddClass("PhysicsErrorCallback", typeid(PhysicsErrorCallback).name());
			reflectionSystem.AddClass("PhysicsListener", typeid(PhysicsListener).name());
			reflectionSystem.AddClass("PhysicsListener::ContactPair", typeid(PhysicsListener::ContactPair).name());
			reflectionSystem.AddClass("RaycastHit", typeid(RaycastHit).name());
			reflectionSystem.AddClass("PhysicsSystem", typeid(PhysicsSystem).name());
			reflectionSystem.AddClass("Field", typeid(Field).name());
			reflectionSystem.AddClass("Class", typeid(Class).name());
			reflectionSystem.AddClass("ReflectionSystem", typeid(ReflectionSystem).name());
			reflectionSystem.AddClass("DebugPipeline::FrameData", typeid(DebugPipeline::FrameData).name());
			reflectionSystem.AddClass("DebugPipeline::DebugVertex", typeid(DebugPipeline::DebugVertex).name());
			reflectionSystem.AddClass("GDRPipeline::ComputePassResources", typeid(GDRPipeline::ComputePassResources).name());
			reflectionSystem.AddClass("GDRPipeline::FrameData", typeid(GDRPipeline::FrameData).name());
			reflectionSystem.AddClass("GDRPipeline::DirectionalLightBuffer", typeid(GDRPipeline::DirectionalLightBuffer).name());
			reflectionSystem.AddClass("IndexBufferHandle", typeid(IndexBufferHandle).name());
			reflectionSystem.AddClass("MeshData", typeid(MeshData).name());
			reflectionSystem.AddClass("VertexBufferData", typeid(VertexBufferData).name());
			reflectionSystem.AddClass("IndexBufferData", typeid(IndexBufferData).name());
			reflectionSystem.AddClass("MeshInstanceData", typeid(MeshInstanceData).name());
			reflectionSystem.AddClass("PerDrawData", typeid(PerDrawData).name());
			reflectionSystem.AddClass("PointLightData", typeid(PointLightData).name());
			reflectionSystem.AddClass("IndexBufferSystem", typeid(IndexBufferSystem).name());
			reflectionSystem.AddClass("Mesh", typeid(Mesh).name());
			reflectionSystem.AddClass("MeshSystem", typeid(MeshSystem).name());
			reflectionSystem.AddClass("MeshUtils", typeid(MeshUtils).name());
			reflectionSystem.AddClass("SkyboxPipeline::FrameData", typeid(SkyboxPipeline::FrameData).name());
			reflectionSystem.AddClass("TextureSystem", typeid(TextureSystem).name());
			reflectionSystem.AddClass("VertexBufferHandle", typeid(VertexBufferHandle).name());
			reflectionSystem.AddClass("VertexBufferSystem", typeid(VertexBufferSystem).name());
			reflectionSystem.AddClass("BinaryReader", typeid(BinaryReader).name());
			reflectionSystem.AddClass("BinaryWriter", typeid(BinaryWriter).name());
			reflectionSystem.AddClass("LandscapeSystem", typeid(LandscapeSystem).name());
			reflectionSystem.AddClass("PointLightSystem", typeid(PointLightSystem).name());
			reflectionSystem.AddClass("Debug", typeid(Debug).name());
			reflectionSystem.AddClass("Debug::DrawLineInfos", typeid(Debug::DrawLineInfos).name());
			reflectionSystem.AddClass("ShaderDatabase", typeid(ShaderDatabase).name());
			reflectionSystem.AddClass("NvidiaAftermathTracker", typeid(NvidiaAftermathTracker).name());
			reflectionSystem.AddClass("VulkanSwapChain", typeid(VulkanSwapChain).name());
			reflectionSystem.AddClass("VulkanUtils", typeid(VulkanUtils).name());
			reflectionSystem.AddClass("VulkanImGui", typeid(VulkanImGui).name());
			reflectionSystem.AddClass("VulkanShaderIncluder", typeid(VulkanShaderIncluder).name());
			reflectionSystem.AddClass("VulkanShaderIncluder::IncludeData", typeid(VulkanShaderIncluder::IncludeData).name());
			reflectionSystem.AddClass("StagingBuffer", typeid(StagingBuffer).name());
			reflectionSystem.AddClass("StagingSystem", typeid(StagingSystem).name());
			reflectionSystem.AddClass("StagingSystem::BufferData", typeid(StagingSystem::BufferData).name());
			reflectionSystem.AddClass("BinaryUtils", typeid(BinaryUtils).name());
		}


		// Add all fields & add base classes.
		{
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Color>();
				currentClass->AddField(Field("myR", offsetof(Color, myR), reflectionSystem.GetClass<uint8_t>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myG", offsetof(Color, myG), reflectionSystem.GetClass<uint8_t>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myB", offsetof(Color, myB), reflectionSystem.GetClass<uint8_t>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myA", offsetof(Color, myA), reflectionSystem.GetClass<uint8_t>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<LinearColor>();
				currentClass->AddField(Field("myR", offsetof(LinearColor, myR), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myG", offsetof(LinearColor, myG), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myB", offsetof(LinearColor, myB), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myA", offsetof(LinearColor, myA), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Console>();
				currentClass->AddField(Field("myConsoleHandle", offsetof(Console, myConsoleHandle), reflectionSystem.GetClass<HANDLE>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myLogToFileEnabled", offsetof(Console, myLogToFileEnabled), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myLogToFileStream", offsetof(Console, myLogToFileStream), reflectionSystem.GetClass<std::ofstream>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCoutBuffer", offsetof(Console, myCoutBuffer), reflectionSystem.GetClass<std::streambuf *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<OnScopeExit>();
				currentClass->AddField(Field("myOnScopeExit", offsetof(OnScopeExit, myOnScopeExit), reflectionSystem.GetClass<std::function<void ()>>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<String>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<EngineProperties>();
				currentClass->AddField(Field("StartupArguments", offsetof(EngineProperties, StartupArguments), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Engine>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<WindowHandler>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<EventHandler>();
				currentClass->AddField(Field("myObservers", offsetof(EventHandler, myObservers), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<AssetRegistry>();
				currentClass->AddField(Field("myMutex", offsetof(AssetRegistry, myMutex), reflectionSystem.GetClass<std::recursive_mutex>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myContainers", offsetof(AssetRegistry, myContainers), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<ThreadPool>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher>();
				currentClass->AddField(Field("myThread", offsetof(Filewatcher, myThread), reflectionSystem.GetClass<std::thread>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myLock", offsetof(Filewatcher, myLock), reflectionSystem.GetClass<std::mutex>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myShouldRun", offsetof(Filewatcher, myShouldRun), reflectionSystem.GetClass<std::atomic_bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFilesToWatch", offsetof(Filewatcher, myFilesToWatch), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myModifiedPaths", offsetof(Filewatcher, myModifiedPaths), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<World>();
				currentClass->AddField(Field("myAssetRegistry", offsetof(World, myAssetRegistry), reflectionSystem.GetClass<class AssetRegistry *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySystemManager", offsetof(World, mySystemManager), reflectionSystem.GetClass<SystemManager<WorldSystem> *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCactus", offsetof(World, myCactus), reflectionSystem.GetClass<class GameObject *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanContext>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Skeleton>();
				currentClass->AddField(Field("myJoints", offsetof(Skeleton, myJoints), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Skeleton::Bone>();
				currentClass->AddField(Field("myBindPosition", offsetof(Skeleton::Bone, myBindPosition), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myBindRotation", offsetof(Skeleton::Bone, myBindRotation), reflectionSystem.GetClass<glm::quat>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myBindScale", offsetof(Skeleton::Bone, myBindScale), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIndexToParent", offsetof(Skeleton::Bone, myIndexToParent), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Asset>();
				currentClass->AddField(Field("myAssetRegistry", offsetof(Asset, myAssetRegistry), reflectionSystem.GetClass<AssetRegistry *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<IAssetContainer>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<ThreadUtils>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<JsonAsset>();
				currentClass->AddField(Field("myJson", offsetof(JsonAsset, myJson), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanAllocator>();
				currentClass->AddField(Field("myAllocator", offsetof(VulkanAllocator, myAllocator), reflectionSystem.GetClass<VmaAllocator>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myBufferDeleteData", offsetof(VulkanAllocator, myBufferDeleteData), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myResizableBufferDeleteData", offsetof(VulkanAllocator, myResizableBufferDeleteData), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myImageDeleteData", offsetof(VulkanAllocator, myImageDeleteData), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDelegateDeletes", offsetof(VulkanAllocator, myDelegateDeletes), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanPhysicalDevice>();
				currentClass->AddField(Field("myDevice", offsetof(VulkanPhysicalDevice, myDevice), reflectionSystem.GetClass<vk::PhysicalDevice>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFeatures", offsetof(VulkanPhysicalDevice, myFeatures), reflectionSystem.GetClass<vk::PhysicalDeviceFeatures2>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMemoryProperties", offsetof(VulkanPhysicalDevice, myMemoryProperties), reflectionSystem.GetClass<vk::PhysicalDeviceMemoryProperties>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myQueueFamilyProperties", offsetof(VulkanPhysicalDevice, myQueueFamilyProperties), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMSAASampleCount", offsetof(VulkanPhysicalDevice, myMSAASampleCount), reflectionSystem.GetClass<vk::SampleCountFlagBits>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myGraphicsQueueIndex", offsetof(VulkanPhysicalDevice, myGraphicsQueueIndex), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myComputeQueueIndex", offsetof(VulkanPhysicalDevice, myComputeQueueIndex), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myTransferQueueIndex", offsetof(VulkanPhysicalDevice, myTransferQueueIndex), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPresentQueueIndex", offsetof(VulkanPhysicalDevice, myPresentQueueIndex), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDeviceExtensions", offsetof(VulkanPhysicalDevice, myDeviceExtensions), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanDevice>();
				currentClass->AddField(Field("myPhysicalDevice", offsetof(VulkanDevice, myPhysicalDevice), reflectionSystem.GetClass<const VulkanPhysicalDevice &>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDevice", offsetof(VulkanDevice, myDevice), reflectionSystem.GetClass<vk::Device>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myGraphicsQueue", offsetof(VulkanDevice, myGraphicsQueue), reflectionSystem.GetClass<vk::Queue>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myComputeQueue", offsetof(VulkanDevice, myComputeQueue), reflectionSystem.GetClass<vk::Queue>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myTransferQueue", offsetof(VulkanDevice, myTransferQueue), reflectionSystem.GetClass<vk::Queue>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCommandPoolsMutex", offsetof(VulkanDevice, myCommandPoolsMutex), reflectionSystem.GetClass<std::recursive_mutex>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCommandPools", offsetof(VulkanDevice, myCommandPools), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myComputeCommandPools", offsetof(VulkanDevice, myComputeCommandPools), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanBuffer>();
				currentClass->AddField(Field("myAllocation", offsetof(VulkanBuffer, myAllocation), reflectionSystem.GetClass<VmaAllocation>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myBuffer", offsetof(VulkanBuffer, myBuffer), reflectionSystem.GetClass<vk::Buffer>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCreateInfo", offsetof(VulkanBuffer, myCreateInfo), reflectionSystem.GetClass<vk::BufferCreateInfo>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMemoryUsage", offsetof(VulkanBuffer, myMemoryUsage), reflectionSystem.GetClass<VmaMemoryUsage>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIsMappingAllowed", offsetof(VulkanBuffer, myIsMappingAllowed), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPtr", offsetof(VulkanBuffer, myPtr), reflectionSystem.GetClass<void *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<ResizableBuffer>();
				currentClass->AddField(Field("myHasActiveUpload", offsetof(ResizableBuffer, myHasActiveUpload), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myHasRegisteredForTick", offsetof(ResizableBuffer, myHasRegisteredForTick), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myBuffer", offsetof(ResizableBuffer, myBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanImage>();
				currentClass->AddField(Field("myAllocation", offsetof(VulkanImage, myAllocation), reflectionSystem.GetClass<VmaAllocation>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myImage", offsetof(VulkanImage, myImage), reflectionSystem.GetClass<vk::Image>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myView", offsetof(VulkanImage, myView), reflectionSystem.GetClass<vk::ImageView>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFormat", offsetof(VulkanImage, myFormat), reflectionSystem.GetClass<vk::Format>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySize", offsetof(VulkanImage, mySize), reflectionSystem.GetClass<glm::vec2>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNumMipLevels", offsetof(VulkanImage, myNumMipLevels), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNumMSAASamples", offsetof(VulkanImage, myNumMSAASamples), reflectionSystem.GetClass<vk::SampleCountFlagBits>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanAllocator::DelegateDeleteData>();
				currentClass->AddField(Field("myFramesUntilDelete", offsetof(VulkanAllocator::DelegateDeleteData, myFramesUntilDelete), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDelegate", offsetof(VulkanAllocator::DelegateDeleteData, myDelegate), reflectionSystem.GetClass<Delegate<void ()>>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Vertex>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<EventObserver>();
				currentClass->AddField(Field("myEvents", offsetof(EventObserver, myEvents), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<System>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<RenderSystem>();
				currentClass->AddField(Field("myDirectionalLightShadowMap", offsetof(RenderSystem, myDirectionalLightShadowMap), reflectionSystem.GetClass<class VulkanImage *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline>();
				currentClass->AddField(Field("myPrePassShader", offsetof(GDRPipeline, myPrePassShader), reflectionSystem.GetClass<Shader *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCullShader", offsetof(GDRPipeline, myCullShader), reflectionSystem.GetClass<Shader *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCubemap", offsetof(GDRPipeline, myCubemap), reflectionSystem.GetClass<TextureCube *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPrePass", offsetof(GDRPipeline, myPrePass), reflectionSystem.GetClass<GDRPipeline::ComputePassResources>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCullPass", offsetof(GDRPipeline, myCullPass), reflectionSystem.GetClass<GDRPipeline::ComputePassResources>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIndirectCommandsBuffer", offsetof(GDRPipeline, myIndirectCommandsBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCountBuffer", offsetof(GDRPipeline, myCountBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPerDrawDataBuffer", offsetof(GDRPipeline, myPerDrawDataBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFrameDescriptorSet", offsetof(GDRPipeline, myFrameDescriptorSet), reflectionSystem.GetClass<VulkanDescriptorSet>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPipelineLayout", offsetof(GDRPipeline, myPipelineLayout), reflectionSystem.GetClass<vk::PipelineLayout>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPipeline", offsetof(GDRPipeline, myPipeline), reflectionSystem.GetClass<vk::Pipeline>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myVertexShader", offsetof(GDRPipeline, myVertexShader), reflectionSystem.GetClass<Shader *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFragmentShader", offsetof(GDRPipeline, myFragmentShader), reflectionSystem.GetClass<Shader *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFrameDataBuffer", offsetof(GDRPipeline, myFrameDataBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDirectionalLightBuffer", offsetof(GDRPipeline, myDirectionalLightBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDirtyTransforms", offsetof(GDRPipeline, myDirtyTransforms), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<FullscreenPipeline>();
				currentClass->AddField(Field("myVertexShader", offsetof(FullscreenPipeline, myVertexShader), reflectionSystem.GetClass<Shader *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFragmentShader", offsetof(FullscreenPipeline, myFragmentShader), reflectionSystem.GetClass<Shader *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPipeline", offsetof(FullscreenPipeline, myPipeline), reflectionSystem.GetClass<vk::Pipeline>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPipelineLayout", offsetof(FullscreenPipeline, myPipelineLayout), reflectionSystem.GetClass<vk::PipelineLayout>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myRenderPass", offsetof(FullscreenPipeline, myRenderPass), reflectionSystem.GetClass<vk::RenderPass>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDescriptorSet", offsetof(FullscreenPipeline, myDescriptorSet), reflectionSystem.GetClass<VulkanDescriptorSet>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<DebugPipeline>();
				currentClass->AddField(Field("myVertexShader", offsetof(DebugPipeline, myVertexShader), reflectionSystem.GetClass<Shader *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFragmentShader", offsetof(DebugPipeline, myFragmentShader), reflectionSystem.GetClass<Shader *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPipeline", offsetof(DebugPipeline, myPipeline), reflectionSystem.GetClass<vk::Pipeline>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPipelineLayout", offsetof(DebugPipeline, myPipelineLayout), reflectionSystem.GetClass<vk::PipelineLayout>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFrameDescriptorSet", offsetof(DebugPipeline, myFrameDescriptorSet), reflectionSystem.GetClass<VulkanDescriptorSet>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFrameDataBuffer", offsetof(DebugPipeline, myFrameDataBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<SkyboxPipeline>();
				currentClass->AddField(Field("myVertexShader", offsetof(SkyboxPipeline, myVertexShader), reflectionSystem.GetClass<Shader *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFragmentShader", offsetof(SkyboxPipeline, myFragmentShader), reflectionSystem.GetClass<Shader *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFrameDescriptorSet", offsetof(SkyboxPipeline, myFrameDescriptorSet), reflectionSystem.GetClass<VulkanDescriptorSet>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPipelineLayout", offsetof(SkyboxPipeline, myPipelineLayout), reflectionSystem.GetClass<vk::PipelineLayout>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPipeline", offsetof(SkyboxPipeline, myPipeline), reflectionSystem.GetClass<vk::Pipeline>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFrameDataBuffer", offsetof(SkyboxPipeline, myFrameDataBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySkyboxModel", offsetof(SkyboxPipeline, mySkyboxModel), reflectionSystem.GetClass<Model *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySkybox", offsetof(SkyboxPipeline, mySkybox), reflectionSystem.GetClass<Texture *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<MathUtils>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<IVulkanDynamicBuffer>();
				currentClass->AddField(Field("OnBufferRecreated", offsetof(IVulkanDynamicBuffer, OnBufferRecreated), reflectionSystem.GetClass<MulticastDelegate<void ()>>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanDescriptorSet>();
				currentClass->AddField(Field("myBuffers", offsetof(VulkanDescriptorSet, myBuffers), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myResizableBuffer", offsetof(VulkanDescriptorSet, myResizableBuffer), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myImages", offsetof(VulkanDescriptorSet, myImages), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myUsesSharedLayout", offsetof(VulkanDescriptorSet, myUsesSharedLayout), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myLayout", offsetof(VulkanDescriptorSet, myLayout), reflectionSystem.GetClass<vk::DescriptorSetLayout>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySet", offsetof(VulkanDescriptorSet, mySet), reflectionSystem.GetClass<vk::DescriptorSet>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Material>();
				currentClass->AddField(Field("myAlbedoTexture", offsetof(Material, myAlbedoTexture), reflectionSystem.GetClass<Texture *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNormalTexture", offsetof(Material, myNormalTexture), reflectionSystem.GetClass<Texture *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMaterialTexture", offsetof(Material, myMaterialTexture), reflectionSystem.GetClass<Texture *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<SerializationMeshData>();
				currentClass->AddField(Field("myVertices", offsetof(SerializationMeshData, myVertices), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIndices", offsetof(SerializationMeshData, myIndices), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySphereCenterBounds", offsetof(SerializationMeshData, mySphereCenterBounds), reflectionSystem.GetClass<glm::vec4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myStagingVertexBuffer", offsetof(SerializationMeshData, myStagingVertexBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myStagingIndexBuffer", offsetof(SerializationMeshData, myStagingIndexBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myAlbedoPath", offsetof(SerializationMeshData, myAlbedoPath), reflectionSystem.GetClass<std::string>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNormalPath", offsetof(SerializationMeshData, myNormalPath), reflectionSystem.GetClass<std::string>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMaterialPath", offsetof(SerializationMeshData, myMaterialPath), reflectionSystem.GetClass<std::string>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Model>();
				currentClass->AddField(Field("myMeshes", offsetof(Model, myMeshes), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher::CallbackHandle>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Filewatcher::FileData>();
				currentClass->AddField(Field("myCallbackIDIndexList", offsetof(Filewatcher::FileData, myCallbackIDIndexList), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCallbacks", offsetof(Filewatcher::FileData, myCallbacks), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myLastModifiedTime", offsetof(Filewatcher::FileData, myLastModifiedTime), reflectionSystem.GetClass<std::filesystem::file_time_type>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Shader>();
				currentClass->AddField(Field("myShaderModule", offsetof(Shader, myShaderModule), reflectionSystem.GetClass<vk::ShaderModule>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCallbackHandle", offsetof(Shader, myCallbackHandle), reflectionSystem.GetClass<Filewatcher::CallbackHandle>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIncludeFiles", offsetof(Shader, myIncludeFiles), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Shader::IncludeFileData>();
				currentClass->AddField(Field("myPath", offsetof(Shader::IncludeFileData, myPath), reflectionSystem.GetClass<std::filesystem::path>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCallbackHandle", offsetof(Shader::IncludeFileData, myCallbackHandle), reflectionSystem.GetClass<Filewatcher::CallbackHandle>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<ImageData>();
				currentClass->AddField(Field("mySourceFile", offsetof(ImageData, mySourceFile), reflectionSystem.GetClass<std::filesystem::path>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myWidth", offsetof(ImageData, myWidth), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myHeight", offsetof(ImageData, myHeight), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myChannels", offsetof(ImageData, myChannels), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNumMipLevels", offsetof(ImageData, myNumMipLevels), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPixelData", offsetof(ImageData, myPixelData), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Texture>();
				currentClass->AddField(Field("myBindlessIndex", offsetof(Texture, myBindlessIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<TextureCube>();
				currentClass->AddField(Field("myBindlessIndex", offsetof(TextureCube, myBindlessIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<IComponentArray>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Component>();
				currentClass->AddField(Field("myGameObject", offsetof(Component, myGameObject), reflectionSystem.GetClass<GameObject *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<WorldSystem>();
				currentClass->AddField(Field("myWorld", offsetof(WorldSystem, myWorld), reflectionSystem.GetClass<World *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<ComponentSystem>();
				currentClass->AddField(Field("myObjects", offsetof(ComponentSystem, myObjects), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myObjectsToDestory", offsetof(ComponentSystem, myObjectsToDestory), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myComponentArrays", offsetof(ComponentSystem, myComponentArrays), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<GameObject>();
				currentClass->AddField(Field("myComponentSystem", offsetof(GameObject, myComponentSystem), reflectionSystem.GetClass<ComponentSystem *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myTransform", offsetof(GameObject, myTransform), reflectionSystem.GetClass<TransformComponent *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myRenderStateDirty", offsetof(GameObject, myRenderStateDirty), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPhysicsStateDirty", offsetof(GameObject, myPhysicsStateDirty), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myTags", offsetof(GameObject, myTags), reflectionSystem.GetClass<TagMask>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<RigidbodyComponent>();
				currentClass->AddField(Field("myActor", offsetof(RigidbodyComponent, myActor), reflectionSystem.GetClass<physx::PxRigidDynamic *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFramesSinceStartSleep", offsetof(RigidbodyComponent, myFramesSinceStartSleep), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMass", offsetof(RigidbodyComponent, myMass), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<ColliderComponent>();
				currentClass->AddField(Field("myLocalShapePosition", offsetof(ColliderComponent, myLocalShapePosition), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myLocalShapeRotation", offsetof(ColliderComponent, myLocalShapeRotation), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<BoxColliderComponent>();
				currentClass->AddField(Field("myHalfSize", offsetof(BoxColliderComponent, myHalfSize), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<CameraComponent>();
				currentClass->AddField(Field("myResolution", offsetof(CameraComponent, myResolution), reflectionSystem.GetClass<glm::vec2>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myProjection", offsetof(CameraComponent, myProjection), reflectionSystem.GetClass<glm::mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFov", offsetof(CameraComponent, myFov), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNearPlane", offsetof(CameraComponent, myNearPlane), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFarPlane", offsetof(CameraComponent, myFarPlane), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIsOrthographic", offsetof(CameraComponent, myIsOrthographic), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<CapsuleColliderComponent>();
				currentClass->AddField(Field("myRadius", offsetof(CapsuleColliderComponent, myRadius), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myHeight", offsetof(CapsuleColliderComponent, myHeight), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<CharacterControllerComponent>();
				currentClass->AddField(Field("myController", offsetof(CharacterControllerComponent, myController), reflectionSystem.GetClass<physx::PxController *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCollisionFlags", offsetof(CharacterControllerComponent, myCollisionFlags), reflectionSystem.GetClass<physx::PxControllerCollisionFlags>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFilter", offsetof(CharacterControllerComponent, myFilter), reflectionSystem.GetClass<physx::PxControllerFilters>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myHeight", offsetof(CharacterControllerComponent, myHeight), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myRadius", offsetof(CharacterControllerComponent, myRadius), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySlopeLimitDegrees", offsetof(CharacterControllerComponent, mySlopeLimitDegrees), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMinDist", offsetof(CharacterControllerComponent, myMinDist), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myStepOffset", offsetof(CharacterControllerComponent, myStepOffset), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myUseGravity", offsetof(CharacterControllerComponent, myUseGravity), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDownVelocity", offsetof(CharacterControllerComponent, myDownVelocity), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myGravity", offsetof(CharacterControllerComponent, myGravity), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myTerminalVelocity", offsetof(CharacterControllerComponent, myTerminalVelocity), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDisplacement", offsetof(CharacterControllerComponent, myDisplacement), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPositionOffset", offsetof(CharacterControllerComponent, myPositionOffset), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<ConvexColliderComponent>();
				currentClass->AddField(Field("myModel", offsetof(ConvexColliderComponent, myModel), reflectionSystem.GetClass<Model *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<DirectionalLightComponent>();
				currentClass->AddField(Field("myIsShadowsEnabled", offsetof(DirectionalLightComponent, myIsShadowsEnabled), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myColor", offsetof(DirectionalLightComponent, myColor), reflectionSystem.GetClass<glm::vec4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myLightProjection", offsetof(DirectionalLightComponent, myLightProjection), reflectionSystem.GetClass<glm::mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<EditorCameraMovementComponent>();
				currentClass->AddField(Field("myResetMouseDelta", offsetof(EditorCameraMovementComponent, myResetMouseDelta), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMouseDelta", offsetof(EditorCameraMovementComponent, myMouseDelta), reflectionSystem.GetClass<glm::vec2>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myYaw", offsetof(EditorCameraMovementComponent, myYaw), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPitch", offsetof(EditorCameraMovementComponent, myPitch), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMovementSpeed", offsetof(EditorCameraMovementComponent, myMovementSpeed), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myShiftMultiplier", offsetof(EditorCameraMovementComponent, myShiftMultiplier), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myScrollMultiplier", offsetof(EditorCameraMovementComponent, myScrollMultiplier), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<LandscapeColliderComponent>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Heightfield>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<GPUSceneSystem>();
				currentClass->AddField(Field("myFreeSparseIndices", offsetof(GPUSceneSystem, myFreeSparseIndices), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNextFreeSparseIndex", offsetof(GPUSceneSystem, myNextFreeSparseIndex), reflectionSystem.GetClass<MeshInstanceIndex>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySparseBuffer", offsetof(GPUSceneSystem, mySparseBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNumGPUObjectUpdatesThisFrame", offsetof(GPUSceneSystem, myNumGPUObjectUpdatesThisFrame), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDenseBuffer", offsetof(GPUSceneSystem, myDenseBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDenseBufferCPURepresentation", offsetof(GPUSceneSystem, myDenseBufferCPURepresentation), reflectionSystem.GetClass<List<uint, uint>>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<LandscapeRenderComponent>();
				currentClass->AddField(Field("myHeightfield", offsetof(LandscapeRenderComponent, myHeightfield), reflectionSystem.GetClass<Heightfield>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myVertexBuffer", offsetof(LandscapeRenderComponent, myVertexBuffer), reflectionSystem.GetClass<VertexBufferHandle *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIndexBuffer", offsetof(LandscapeRenderComponent, myIndexBuffer), reflectionSystem.GetClass<IndexBufferHandle *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMesh", offsetof(LandscapeRenderComponent, myMesh), reflectionSystem.GetClass<Mesh *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMeshInstance", offsetof(LandscapeRenderComponent, myMeshInstance), reflectionSystem.GetClass<MeshInstanceIndex>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMaterial", offsetof(LandscapeRenderComponent, myMaterial), reflectionSystem.GetClass<Material *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<MeshColliderComponent>();
				currentClass->AddField(Field("myModel", offsetof(MeshColliderComponent, myModel), reflectionSystem.GetClass<Model *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<PointLightComponent>();
				currentClass->AddField(Field("myColor", offsetof(PointLightComponent, myColor), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIntensity", offsetof(PointLightComponent, myIntensity), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myRange", offsetof(PointLightComponent, myRange), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<SinWaveMovementComponent>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<SphereColliderComponent>();
				currentClass->AddField(Field("myRadius", offsetof(SphereColliderComponent, myRadius), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<StaticMeshComponent>();
				currentClass->AddField(Field("myPath", offsetof(StaticMeshComponent, myPath), reflectionSystem.GetClass<std::filesystem::path>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMaterials", offsetof(StaticMeshComponent, myMaterials), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myModel", offsetof(StaticMeshComponent, myModel), reflectionSystem.GetClass<Model *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMeshInstances", offsetof(StaticMeshComponent, myMeshInstances), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<TransformComponent>();
				currentClass->AddField(Field("myPositionDirty", offsetof(TransformComponent, myPositionDirty), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myRotationDirty", offsetof(TransformComponent, myRotationDirty), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myScaleDirty", offsetof(TransformComponent, myScaleDirty), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPosition", offsetof(TransformComponent, myPosition), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myRotation", offsetof(TransformComponent, myRotation), reflectionSystem.GetClass<glm::quat>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myScale", offsetof(TransformComponent, myScale), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySkipPhysicsUpdate", offsetof(TransformComponent, mySkipPhysicsUpdate), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myParent", offsetof(TransformComponent, myParent), reflectionSystem.GetClass<TransformComponent *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myChildren", offsetof(TransformComponent, myChildren), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<AutoInit>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<AutoInitManager>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Input>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Input::KeyState>();
				currentClass->AddField(Field("myIsDown", offsetof(Input::KeyState, myIsDown), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIsPressed", offsetof(Input::KeyState, myIsPressed), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIsUp", offsetof(Input::KeyState, myIsUp), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Time>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Awaitable>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Awaitables::SwitchToThread>();
				currentClass->AddField(Field("myThreadType", offsetof(Awaitables::SwitchToThread, myThreadType), reflectionSystem.GetClass<ThreadType>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Awaitables::WaitForSeconds>();
				currentClass->AddField(Field("mySeconds", offsetof(Awaitables::WaitForSeconds, mySeconds), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<CoroutineManager>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<PhysicsErrorCallback>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<PhysicsListener>();
				currentClass->AddField(Field("myDequeueCollisionsDelegate", offsetof(PhysicsListener, myDequeueCollisionsDelegate), reflectionSystem.GetClass<MulticastDelegate<void ()>>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCollisionPairs", offsetof(PhysicsListener, myCollisionPairs), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myTriggerPairs", offsetof(PhysicsListener, myTriggerPairs), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<PhysicsListener::ContactPair>();
				currentClass->AddField(Field("myFirst", offsetof(PhysicsListener::ContactPair, myFirst), reflectionSystem.GetClass<GameObject *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySecond", offsetof(PhysicsListener::ContactPair, mySecond), reflectionSystem.GetClass<GameObject *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<RaycastHit>();
				currentClass->AddField(Field("myHitPosition", offsetof(RaycastHit, myHitPosition), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myHitNormal", offsetof(RaycastHit, myHitNormal), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myHitDistance", offsetof(RaycastHit, myHitDistance), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myHitGameObject", offsetof(RaycastHit, myHitGameObject), reflectionSystem.GetClass<class GameObject *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<PhysicsSystem>();
				currentClass->AddField(Field("myPhysicsCommands", offsetof(PhysicsSystem, myPhysicsCommands), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDefaultErrorCallback", offsetof(PhysicsSystem, myDefaultErrorCallback), reflectionSystem.GetClass<physx::PxErrorCallback *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDefaultAllocator", offsetof(PhysicsSystem, myDefaultAllocator), reflectionSystem.GetClass<physx::PxDefaultAllocator *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myToleranceScale", offsetof(PhysicsSystem, myToleranceScale), reflectionSystem.GetClass<physx::PxTolerancesScale *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFoundation", offsetof(PhysicsSystem, myFoundation), reflectionSystem.GetClass<physx::PxFoundation *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPvd", offsetof(PhysicsSystem, myPvd), reflectionSystem.GetClass<physx::PxPvd *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPvdTransport", offsetof(PhysicsSystem, myPvdTransport), reflectionSystem.GetClass<physx::PxPvdTransport *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDefaultMaterial", offsetof(PhysicsSystem, myDefaultMaterial), reflectionSystem.GetClass<physx::PxMaterial *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myListener", offsetof(PhysicsSystem, myListener), reflectionSystem.GetClass<PhysicsListener *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPhysics", offsetof(PhysicsSystem, myPhysics), reflectionSystem.GetClass<physx::PxPhysics *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myScene", offsetof(PhysicsSystem, myScene), reflectionSystem.GetClass<physx::PxScene *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myControllerManager", offsetof(PhysicsSystem, myControllerManager), reflectionSystem.GetClass<physx::PxControllerManager *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myHasActiveSimulation", offsetof(PhysicsSystem, myHasActiveSimulation), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Field>();
				currentClass->AddField(Field("myName", offsetof(Field, myName), reflectionSystem.GetClass<std::string>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myOffset", offsetof(Field, myOffset), reflectionSystem.GetClass<uint32_t>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myType", offsetof(Field, myType), reflectionSystem.GetClass<const Class *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIsPointer", offsetof(Field, myIsPointer), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIsReference", offsetof(Field, myIsReference), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Class>();
				currentClass->AddField(Field("myFullName", offsetof(Class, myFullName), reflectionSystem.GetClass<std::string>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myClassName", offsetof(Class, myClassName), reflectionSystem.GetClass<std::string>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myBaseClasses", offsetof(Class, myBaseClasses), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDerivedClasses", offsetof(Class, myDerivedClasses), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFields", offsetof(Class, myFields), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<ReflectionSystem>();
				currentClass->AddField(Field("myClasses", offsetof(ReflectionSystem, myClasses), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<DebugPipeline::FrameData>();
				currentClass->AddField(Field("myToView", offsetof(DebugPipeline::FrameData, myToView), reflectionSystem.GetClass<glm::mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myProjection", offsetof(DebugPipeline::FrameData, myProjection), reflectionSystem.GetClass<glm::mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCameraPosition", offsetof(DebugPipeline::FrameData, myCameraPosition), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<DebugPipeline::DebugVertex>();
				currentClass->AddField(Field("myPosition", offsetof(DebugPipeline::DebugVertex, myPosition), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myColor", offsetof(DebugPipeline::DebugVertex, myColor), reflectionSystem.GetClass<Color>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline::ComputePassResources>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline::FrameData>();
				currentClass->AddField(Field("myToView", offsetof(GDRPipeline::FrameData, myToView), reflectionSystem.GetClass<glm::mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myProjection", offsetof(GDRPipeline::FrameData, myProjection), reflectionSystem.GetClass<glm::mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCameraPosition", offsetof(GDRPipeline::FrameData, myCameraPosition), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCubemapIndex", offsetof(GDRPipeline::FrameData, myCubemapIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<GDRPipeline::DirectionalLightBuffer>();
				currentClass->AddField(Field("myColor", offsetof(GDRPipeline::DirectionalLightBuffer, myColor), reflectionSystem.GetClass<glm::vec4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDirection", offsetof(GDRPipeline::DirectionalLightBuffer, myDirection), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("padding", offsetof(GDRPipeline::DirectionalLightBuffer, padding), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myLightView", offsetof(GDRPipeline::DirectionalLightBuffer, myLightView), reflectionSystem.GetClass<glm::mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myLightProjection", offsetof(GDRPipeline::DirectionalLightBuffer, myLightProjection), reflectionSystem.GetClass<glm::mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferHandle>();
				currentClass->AddField(Field("myIndex", offsetof(IndexBufferHandle, myIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<MeshData>();
				currentClass->AddField(Field("myBoundingSphereModelSpace", offsetof(MeshData, myBoundingSphereModelSpace), reflectionSystem.GetClass<vec4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myVertexIndex", offsetof(MeshData, myVertexIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIndexDataIndex", offsetof(MeshData, myIndexDataIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMaterialIndex", offsetof(MeshData, myMaterialIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferData>();
				currentClass->AddField(Field("myOffset", offsetof(VertexBufferData, myOffset), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferData>();
				currentClass->AddField(Field("myOffset", offsetof(IndexBufferData, myOffset), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCount", offsetof(IndexBufferData, myCount), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<MeshInstanceData>();
				currentClass->AddField(Field("myToWorld", offsetof(MeshInstanceData, myToWorld), reflectionSystem.GetClass<mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMeshIndex", offsetof(MeshInstanceData, myMeshIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myAlbedoIndex", offsetof(MeshInstanceData, myAlbedoIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNormalIndex", offsetof(MeshInstanceData, myNormalIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMaterialIndex", offsetof(MeshInstanceData, myMaterialIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<PerDrawData>();
				currentClass->AddField(Field("myToWorld", offsetof(PerDrawData, myToWorld), reflectionSystem.GetClass<mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myAlbedoIndex", offsetof(PerDrawData, myAlbedoIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNormalIndex", offsetof(PerDrawData, myNormalIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMaterialIndex", offsetof(PerDrawData, myMaterialIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("padding", offsetof(PerDrawData, padding), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<PointLightData>();
				currentClass->AddField(Field("myColor", offsetof(PointLightData, myColor), reflectionSystem.GetClass<vec4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myPosition", offsetof(PointLightData, myPosition), reflectionSystem.GetClass<vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myRange", offsetof(PointLightData, myRange), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIntensity", offsetof(PointLightData, myIntensity), reflectionSystem.GetClass<float>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<IndexBufferSystem>();
				currentClass->AddField(Field("myUsedBufferSize", offsetof(IndexBufferSystem, myUsedBufferSize), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCurrentIndexOffset", offsetof(IndexBufferSystem, myCurrentIndexOffset), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myIndexBuffers", offsetof(IndexBufferSystem, myIndexBuffers), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myBuffer", offsetof(IndexBufferSystem, myBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySparseIndexDataBuffer", offsetof(IndexBufferSystem, mySparseIndexDataBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySparseIndexData_CPURepresentation", offsetof(IndexBufferSystem, mySparseIndexData_CPURepresentation), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFreeSparseIndices", offsetof(IndexBufferSystem, myFreeSparseIndices), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Mesh>();
				currentClass->AddField(Field("myAlbedoPath", offsetof(Mesh, myAlbedoPath), reflectionSystem.GetClass<std::string>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNormalPath", offsetof(Mesh, myNormalPath), reflectionSystem.GetClass<std::string>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMaterialPath", offsetof(Mesh, myMaterialPath), reflectionSystem.GetClass<std::string>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<MeshSystem>();
				currentClass->AddField(Field("myMeshes", offsetof(MeshSystem, myMeshes), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myBuffer", offsetof(MeshSystem, myBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNumObjects", offsetof(MeshSystem, myNumObjects), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<MeshUtils>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<SkyboxPipeline::FrameData>();
				currentClass->AddField(Field("myToView", offsetof(SkyboxPipeline::FrameData, myToView), reflectionSystem.GetClass<glm::mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myProjection", offsetof(SkyboxPipeline::FrameData, myProjection), reflectionSystem.GetClass<glm::mat4>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCameraPosition", offsetof(SkyboxPipeline::FrameData, myCameraPosition), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCubemapIndex", offsetof(SkyboxPipeline::FrameData, myCubemapIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<TextureSystem>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferHandle>();
				currentClass->AddField(Field("myIndex", offsetof(VertexBufferHandle, myIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VertexBufferSystem>();
				currentClass->AddField(Field("myUsedBufferSize", offsetof(VertexBufferSystem, myUsedBufferSize), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCurrentVertexOffset", offsetof(VertexBufferSystem, myCurrentVertexOffset), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myVertexBuffers", offsetof(VertexBufferSystem, myVertexBuffers), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myBuffer", offsetof(VertexBufferSystem, myBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySparseVertexDataBuffer", offsetof(VertexBufferSystem, mySparseVertexDataBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySparseVertexData_CPURepresentation", offsetof(VertexBufferSystem, mySparseVertexData_CPURepresentation), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFreeSparseIndices", offsetof(VertexBufferSystem, myFreeSparseIndices), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<BinaryReader>();
				currentClass->AddField(Field("myBuffer", offsetof(BinaryReader, myBuffer), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myReadOffset", offsetof(BinaryReader, myReadOffset), reflectionSystem.GetClass<size_t>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<BinaryWriter>();
				currentClass->AddField(Field("myStream", offsetof(BinaryWriter, myStream), reflectionSystem.GetClass<std::ofstream>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<LandscapeSystem>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<PointLightSystem>();
				currentClass->AddField(Field("myBuffer", offsetof(PointLightSystem, myBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myNumPointLights", offsetof(PointLightSystem, myNumPointLights), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Debug>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<Debug::DrawLineInfos>();
				currentClass->AddField(Field("myStart", offsetof(Debug::DrawLineInfos, myStart), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myEnd", offsetof(Debug::DrawLineInfos, myEnd), reflectionSystem.GetClass<glm::vec3>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myColor", offsetof(Debug::DrawLineInfos, myColor), reflectionSystem.GetClass<Color>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<ShaderDatabase>();
				currentClass->AddField(Field("m_shaderBinaries", offsetof(ShaderDatabase, m_shaderBinaries), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("m_shaderBinariesWithDebugInfo", offsetof(ShaderDatabase, m_shaderBinariesWithDebugInfo), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<NvidiaAftermathTracker>();
				currentClass->AddField(Field("m_initialized", offsetof(NvidiaAftermathTracker, m_initialized), reflectionSystem.GetClass<bool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("m_mutex", offsetof(NvidiaAftermathTracker, m_mutex), reflectionSystem.GetClass<std::mutex>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("m_shaderDebugInfo", offsetof(NvidiaAftermathTracker, m_shaderDebugInfo), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("m_shaderDatabase", offsetof(NvidiaAftermathTracker, m_shaderDatabase), reflectionSystem.GetClass<ShaderDatabase>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("m_markerMap", offsetof(NvidiaAftermathTracker, m_markerMap), reflectionSystem.GetClass<const AfterMathMarkerMap &>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("m_AftermathFlags", offsetof(NvidiaAftermathTracker, m_AftermathFlags), reflectionSystem.GetClass<uint32_t>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanSwapChain>();
				currentClass->AddField(Field("myDevice", offsetof(VulkanSwapChain, myDevice), reflectionSystem.GetClass<const VulkanDevice &>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myWindowSurface", offsetof(VulkanSwapChain, myWindowSurface), reflectionSystem.GetClass<vk::SurfaceKHR>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFormat", offsetof(VulkanSwapChain, myFormat), reflectionSystem.GetClass<vk::Format>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFences", offsetof(VulkanSwapChain, myFences), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myImageAcquiredSemaphores", offsetof(VulkanSwapChain, myImageAcquiredSemaphores), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myDrawCompleteSemaphores", offsetof(VulkanSwapChain, myDrawCompleteSemaphores), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myFrameIndex", offsetof(VulkanSwapChain, myFrameIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySyncIndex", offsetof(VulkanSwapChain, mySyncIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySwapChain", offsetof(VulkanSwapChain, mySwapChain), reflectionSystem.GetClass<vk::SwapchainKHR>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySwapChainWidth", offsetof(VulkanSwapChain, mySwapChainWidth), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySwapChainHeight", offsetof(VulkanSwapChain, mySwapChainHeight), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myMinImageCount", offsetof(VulkanSwapChain, myMinImageCount), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myImages", offsetof(VulkanSwapChain, myImages), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myImageViews", offsetof(VulkanSwapChain, myImageViews), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCommandPool", offsetof(VulkanSwapChain, myCommandPool), reflectionSystem.GetClass<vk::CommandPool>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCommandBuffers", offsetof(VulkanSwapChain, myCommandBuffers), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanUtils>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanImGui>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanShaderIncluder>();
				currentClass->AddField(Field("myActiveIncludes", offsetof(VulkanShaderIncluder, myActiveIncludes), reflectionSystem.GetClass<int>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<VulkanShaderIncluder::IncludeData>();
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<StagingBuffer>();
				currentClass->AddField(Field("myUnderlyingBuffer", offsetof(StagingBuffer, myUnderlyingBuffer), reflectionSystem.GetClass<VulkanBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("mySize", offsetof(StagingBuffer, mySize), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myOffset", offsetof(StagingBuffer, myOffset), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<StagingSystem>();
				currentClass->AddField(Field("myLastFrameIndex", offsetof(StagingSystem, myLastFrameIndex), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myCurrentStageData", offsetof(StagingSystem, myCurrentStageData), reflectionSystem.GetClass<StagingSystem::BufferData *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myStagingBuffers", offsetof(StagingSystem, myStagingBuffers), reflectionSystem.GetClass<std::array<StagingSystem::BufferData, VulkanContext::FrameLag>>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<StagingSystem::BufferData>();
				currentClass->AddField(Field("myBuffer", offsetof(StagingSystem::BufferData, myBuffer), reflectionSystem.GetClass<ResizableBuffer *>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				currentClass->AddField(Field("myOffset", offsetof(StagingSystem::BufferData, myOffset), reflectionSystem.GetClass<uint>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));
				// Base classes are not implemented yet.
			}
			{
				Class* currentClass = reflectionSystem.GetMutableClass<BinaryUtils>();
				// Base classes are not implemented yet.
			}
		}
	}
};
#undef private
#undef protected
