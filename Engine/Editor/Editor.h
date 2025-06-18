#pragma once
#include "Engine/Delegates/Delegate.hpp"

class AssetRegistry;
class Class;

class Editor
{
public:
	Editor();
	~Editor();

	static void StaticTick();

	static AssetRegistry* GetAssetRegistry();
	
	static void RemoveWindow(class EditorWindow* inEditorWindow);

	void SetGameTickFunction(const Delegate<void()>& inTickFunction);

	template<typename WindowType>
	static void OpenEditorWindow()
	{
		WindowType* window = new WindowType();
		window->myID = myInstance->myNextID++;;
		myInstance->myWindows.Add(window);
	}

	template<typename SystemType>
	SystemType* GetSystem() const
	{
		for (EditorSystem* system : mySystems)
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

	void AddWindow(const Class* inWindowClass);

private:
	List<class EditorWindow*> myWindows{};
	List<class EditorSystem*> mySystems{};
	
	int myNextID = 1;

	Delegate<void()> myGameTickFunction;

	AssetRegistry* myAssetRegistry = nullptr;
	
private:
	inline static Editor* myInstance = nullptr;
};
