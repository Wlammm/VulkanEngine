#pragma once

class EditorWindow
{
public:
	EditorWindow() = delete;
	EditorWindow(const std::string& inWindowName, const bool inClosable);
	virtual ~EditorWindow() {};
	void DoTick();

protected:
	virtual void Tick() = 0;

private:
	std::string myWindowName;
	bool myIsClosable = true;

private:
	friend class Editor;
	int myID = 0;

};