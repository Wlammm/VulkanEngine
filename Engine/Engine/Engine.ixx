module;

export module Engine;
import EngineProperties;

export class Engine
{
public:
	Engine(const EngineProperties inEngineProperties);
	~Engine();

	void Tick();

	bool ShouldRun() const;
	
	static void SetIsRunning(const bool inIsRunning);
	static const EngineProperties& GetEngineProperties();

	static const class SystemDispatcher& GetSystemDispatcher();

	static const class World& GetWorld();
	static void SetWorld(World* inWorld);

private:
	void CreateSystems();

private:
	bool myIsRunning = true;

	EngineProperties myEngineProperties;

	class Console* myConsole = nullptr;
	class WindowHandler* myWindowHandler = nullptr;
	class VulkanContext* myVulkanContext = nullptr;
	class SystemDispatcher* mySystemDispatcher = nullptr;

	class World* myWorld = nullptr;

private:
	inline static Engine* myInstance = nullptr;
};