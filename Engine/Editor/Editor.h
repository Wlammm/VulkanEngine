#pragma once
#include "Engine/Delegates/Delegate.hpp"

class Editor
{
public:
	Editor();
	~Editor();

	static void StaticTick();

	template<typename T, typename... Args>
	static T* AddWindow(Args&&... params)
	{
		T* window = new T(std::forward<Args>(params)...);
		window->myID = myInstance->myNextID++;
		myInstance->myWindows.Add(window);
		return window;
	}

	static void RemoveWindow(class EditorWindow* inEditorWindow);

	void SetGameTickFunction(const Delegate<void()>& inTickFunction);
	
private:
	void Tick();
	void BeginMainDockSpace();

private:
	List<class EditorWindow*> myWindows{};
	int myNextID = 1;

	Delegate<void()> myGameTickFunction;
	
private:
	inline static Editor* myInstance = nullptr;
};
