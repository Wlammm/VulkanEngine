#pragma once
#include "Engine/Delegates/Delegate.hpp"

class Type;

class Editor
{
public:
	Editor();
	~Editor();

	static void StaticTick();

	static void RemoveWindow(class EditorWindow* inEditorWindow, const bool inIsShutdown = false);

	void SetGameTickFunction(const Delegate<void()>& inTickFunction);

	static void TogglePIE();
	static bool IsPIE();

	void StartPIE();
	void StopPIE();
	
	template<typename WindowType>
	static void OpenEditorWindow()
	{
		WindowType* window = new WindowType();
		window->myID = myInstance->myNextID++;
		myInstance->myWindows.Add(window);
	}

	template<typename SystemType>
	static SystemType* GetSystem()
	{
		for (EditorSystem* system : myInstance->mySystems)
		{
			if (SystemType* castedSystem = dynamic_cast<SystemType*>(system))
				return castedSystem;
		}
		return nullptr;
	}
	
private:
	void Tick();
	void BeginMainDockSpace();

	void AddEditorWindows();
	void AddEditorSystems();

	void AddWindow(const Type* inWindowClass);

private:
	List<class EditorWindow*> myWindows{};
	List<class EditorSystem*> mySystems{};
	
	int myNextID = 1;

	Delegate<void()> myGameTickFunction;

	bool myIsPIE = false;
	
private:
	inline static Editor* myInstance = nullptr;
};
