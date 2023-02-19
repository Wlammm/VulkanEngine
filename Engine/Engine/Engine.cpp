#include "EnginePch.h"
#include "Engine.h"

#include "Core/Time.h"
#include "Windows/WindowHandler.h"
#include "Vulkan/VulkanContext.h"
#include "ECS/SystemDispatcher.h"
#include "ECS/Systems/RenderSystem.h"
#include "Core\Input.h"

#include "Containers/List.hpp"

Engine::Engine(const EngineProperties inEngineProperties)
	: myEngineProperties { inEngineProperties }
{
	check(!myInstance && "Cant have multiple Engine instances.");
	myInstance = this;

	myConsole = new Console();
	myWindowHandler = new WindowHandler();
	myVulkanContext = new VulkanContext();
	mySystemDispatcher = new SystemDispatcher();

	CreateSystems();

	auto itemcount = 100000;
	auto seconds = Time::GetSeconds();
	List<int> list{};
	for (int i = 0; i < itemcount; ++i)
	{
		list.Add(i);
	}

	//long long val = 0;
	//for (const auto& value : list)
	//{
	//	val += value;
	//}

	//for (int i = 0; i < itemcount / 2 - 1; ++i)
	//{
	//	list.RemoveIndex(i);
	//}

	auto totSeconds = Time::GetSeconds() - seconds;
	std::cout << "List time: " <<  totSeconds << " capacity: " << list.GetCapacity() << std::endl;

	seconds = Time::GetSeconds();
	std::vector<int> vector{};
	for (int i = 0; i < itemcount; ++i)
	{
		vector.push_back(i);
	}

	//val = 0;
	//for (const auto& value : vector)
	//{
	//	val += value;
	//}

	//for (int i = 0; i < itemcount / 2 - 1; ++i)
	//{
	//	vector.erase(vector.begin() + i);
	//}
	totSeconds = Time::GetSeconds() - seconds;
	std::cout << "Vector time: " << totSeconds << " capacity: " << vector.capacity() << std::endl;
}

Engine::~Engine()
{
	del(mySystemDispatcher);
	del(myVulkanContext);
	del(myWindowHandler);
	del(myConsole);

	myInstance = nullptr;
}

void Engine::Tick()
{
	Time::Tick();
	myWindowHandler->Tick();

	if (Input::IsKeyDown(KeyCode::Escape))
		PostQuitMessage(0);

	VulkanContext::BeginFrame();

	mySystemDispatcher->DispatchSystems();

	VulkanContext::EndFrame();
	Input::EndFrame();
}

bool Engine::ShouldRun() const
{
	return myIsRunning;
}

void Engine::SetIsRunning(const bool inIsRunning)
{
	myInstance->myIsRunning = inIsRunning;
}

const EngineProperties& Engine::GetEngineProperties()
{
	return myInstance->myEngineProperties;
}

const SystemDispatcher& Engine::GetSystemDispatcher()
{
	return *myInstance->mySystemDispatcher;
}

const World& Engine::GetWorld()
{
	return *myInstance->myWorld;
}

void Engine::SetWorld(World* inWorld)
{
	myInstance->myWorld = inWorld;
}

void Engine::CreateSystems()
{
	mySystemDispatcher->AddSystem<RenderSystem>();
}
