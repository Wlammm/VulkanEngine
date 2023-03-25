#include "EditorPch.h"
#include "Editor.h"

Editor::Editor()
{
	check(myInstance == nullptr && "There is already an instance of this class. Only one allowed!");
	myInstance = this;
}

Editor::~Editor()
{
	myInstance = nullptr;
}
