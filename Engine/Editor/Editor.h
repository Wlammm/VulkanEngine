#pragma once
#include "Engine/Delegates/Delegate.hpp"

class Editor
{
public:
	Editor();
	~Editor();

	static void StaticTick();

	static void RemoveWindow(class EditorWindow* inEditorWindow);

	void SetGameTickFunction(const Delegate<void()>& inTickFunction);

	template<typename WindowType>
	static void OpenEditorWindow()
	{
		WindowType* window = new WindowType();
		window->myID = myInstance->myNextID++;;
		myInstance->myWindows.Add(window);
	}
	
private:
	void Tick();
	void BeginMainDockSpace();

	void AddEditorWindows();
	void AddEditorSystems();

private:
	List<class EditorWindow*> myWindows{};
	List<class EditorSystem*> mySystems{};
	
	int myNextID = 1;

	Delegate<void()> myGameTickFunction;
	
private:
	inline static Editor* myInstance = nullptr;
};
