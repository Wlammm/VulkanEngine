#include "EditorPch.h"
#include "EditorWindow.h"
#include "Editor/Editor.h"

EditorWindow::EditorWindow(const std::string& inWindowName, const bool inClosable)
	:myWindowName{ inWindowName }, myIsClosable{ inClosable }
{

}

void EditorWindow::DoTick()
{
	bool open = true;
	const std::string name = myWindowName + "##" + std::to_string(myID);
	if(myIsClosable)
		ImGui::Begin(name.c_str(), &open);
	else
		ImGui::Begin(name.c_str());

	Tick();
	
	if (ImGui::IsWindowFocused())
		TickInput();

	ImGui::End();

	if(!open)
		Editor::RemoveWindow(this);
}
