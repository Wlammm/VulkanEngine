#include "EditorPch.h"
#include "Editor.h"

#include "EditorSystem/EditorSystem.h"
#include "EditorSystem/EditorToolbar.h"
#include "EditorSystem/SelectionSystem.h"
#include "Engine/Engine.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/World/GameWorld.h"
#include "Engine/World/World.h"
#include "Toolbar/Themes/EditorThemes.h"
#include "Windows/EditorWindow.h"
#include "Windows/HierarchyWindow.h"
#include "Windows/ImGuiPropertyDrawer.h"
#include "Windows/InspectorWindow.h"
#include "Windows/Viewport.h"
#include "World/EditorWorld.h"

Editor::Editor()
{
    check(myInstance == nullptr && "There is already an instance of this class. Only one allowed!");
    myInstance = this;

    myAssetRegistry = new AssetRegistry();
    
    ImGuiPropertyDrawer::RegisterDrawers();

    AddEditorSystems();
    AddEditorWindows();

    EditorThemes::DefaultTheme();

    Engine::SetWorld(new EditorWorld());
}

Editor::~Editor()
{
    for (int i = (int)myWindows.size() - 1; i >= 0; --i)
    {
        RemoveWindow(myWindows[i]);
    }

    del(myAssetRegistry);
    myInstance = nullptr;
}

void Editor::StaticTick()
{
    myInstance->Tick();
}

AssetRegistry* Editor::GetAssetRegistry()
{
    return myInstance->myAssetRegistry;
}

void Editor::Tick()
{
    // TODO: This should probably be removed whenever we implement play in editor.
    check(myGameTickFunction.IsValid());
    
    BeginMainDockSpace();

    for (const auto& window : myWindows)
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
    Engine::TickNextFrame.Bind([inEditorWindow]()
    {
        myInstance->myWindows.Remove(inEditorWindow);
        delete inEditorWindow;
    });
}

void Editor::SetGameTickFunction(const Delegate<void()>& inTickFunction)
{
    myGameTickFunction = inTickFunction;
}

void Editor::TogglePIE()
{
    SelectionSystem::ClearSelection();
    if (myInstance->myIsPIE)
    {
        myInstance->StopPIE();
    }
    else
    {
        myInstance->StartPIE();    
    }
}

bool Editor::IsPIE()
{
    return myInstance->myIsPIE;
}

void Editor::StartPIE()
{
    myIsPIE = true;

    World* currentWorld = Engine::GetWorld();
    if (currentWorld != nullptr)
        del(currentWorld);
    
    Engine::SetWorld(new GameWorld());
}

void Editor::StopPIE()
{
    myIsPIE = false;

    World* currentWorld = Engine::GetWorld();
    if (currentWorld != nullptr)
        del(currentWorld);
    
    Engine::SetWorld(new EditorWorld());
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
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove;
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
    const Type* editorWindowClass = ReflectionSystem::GetClass<EditorWindow>();

    EditorToolbar* toolbar = GetSystem<EditorToolbar>();
    for (const Type* entry : editorWindowClass->GetDerivedTypes())
    {
        AddWindow(entry);

        std::string path = "Windows/" + entry->GetName();
        toolbar->AddToolbarButton(path, [entry, this]()
        {
            AddWindow(entry);
        });
    }
}

void Editor::AddEditorSystems()
{
    const Type* editorSystemClass = ReflectionSystem::GetClass<EditorSystem>();

    for (const Type* entry : editorSystemClass->GetDerivedTypes())
    {
        EditorSystem* system = entry->CreateInstance<EditorSystem>();
        system->myID = myNextID++;
        mySystems.Add(system);
    }
}

void Editor::AddWindow(const Type* inWindowClass)
{
    EditorWindow* window = inWindowClass->CreateInstance<EditorWindow>();
    window->myID = myNextID++;
    myWindows.Add(window);
}
