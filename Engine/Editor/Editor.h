#pragma once

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

private:
	void Tick();
	void BeginMainDockSpace();

private:
	List<class EditorWindow*> myWindows{};
	int myNextID = 1;

private:
	inline static Editor* myInstance = nullptr;
};