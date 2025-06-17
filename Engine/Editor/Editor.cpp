#include "EditorPch.h"
#include "Editor.h"

#include "EditorSystem/EditorSystem.h"
#include "Engine/Engine.h"
#include "Engine/Reflection/ReflectionSystem.h"
#include "Windows/EditorWindow.h"
#include "Windows/HierarchyWindow.h"
#include "Windows/ImGuiPropertyDrawer.h"
#include "Windows/InspectorWindow.h"
#include "Windows/Viewport.h"

Editor::Editor()
{
	check(myInstance == nullptr && "There is already an instance of this class. Only one allowed!");
	myInstance = this;

	ImGuiPropertyDrawer::RegisterDrawers();

	AddEditorWindows();
	AddEditorSystems();
}

Editor::~Editor()
{
	for (int i = (int)myWindows.size() - 1; i >= 0; --i)
	{
		RemoveWindow(myWindows[i]);
	}

	myInstance = nullptr;
}

void Editor::StaticTick()
{
	myInstance->Tick();
}

void Editor::Tick()
{
	// TODO: This should probably be removed whenever we implement play in editor.
	check(myGameTickFunction.IsValid());
	myGameTickFunction.Invoke();

	BeginMainDockSpace();
	
	for(const auto& window : myWindows)
	{
		window->DoTick();
	}

	for (const auto& system : mySystems)
	{
		system->Tick();
	}

	ImGui::End();
}

void Editor::RemoveWindow(EditorWindow* inEditorWindow)
{
	myInstance->myWindows.Remove(inEditorWindow);
	del(inEditorWindow);
}

void Editor::SetGameTickFunction(const Delegate<void()>& inTickFunction)
{
	myGameTickFunction = inTickFunction;
}

void Editor::BeginMainDockSpace()
{
	static bool open = true;

	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

	ImGui::Begin("EditorInterface", &open, window_flags);

	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
}

void Editor::AddEditorWindows()
{
	ReflectionSystem& reflectionSystem = Engine::GetEngineSystem<ReflectionSystem>();

	const Class* editorWindowClass = reflectionSystem.GetClass<EditorWindow>();

	for (const Class* entry : editorWindowClass->GetDerivedClasses())
	{
		EditorWindow* window = entry->CreateInstance<EditorWindow>();
		window->myID = myNextID++;
		myWindows.Add(window);
	}
}

void Editor::AddEditorSystems()
{
	ReflectionSystem& reflectionSystem = Engine::GetEngineSystem<ReflectionSystem>();

	const Class* editorSystemClass = reflectionSystem.GetClass<EditorSystem>();

	for (const Class* entry : editorSystemClass->GetDerivedClasses())
	{
		EditorSystem* system = entry->CreateInstance<EditorSystem>();
		system->myID = myNextID++;
		mySystems.Add(system);
	}
}
