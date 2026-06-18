#include "EditorPch.h"
#include "Editor.h"

#include "EditorSystem/EditorSystem.h"
#include "EditorSystem/EditorToolbar.h"
#include "EditorSystem/SelectionSystem.h"
#include "Engine/Engine.h"
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/Vulkan/VulkanImGui.h"
#include "Engine/Windows/WindowHandler.h"
#include "Engine/World/GameWorld.h"
#include "ImGui/ImGuiPropertyDrawer.h"
#include "Toolbar/Themes/EditorThemes.h"
#include "Windows/EditorWindow.h"
#include "World/EditorWorld.h"

Editor::Editor()
{
    check(myInstance == nullptr && "There is already an instance of this class. Only one allowed!");
    myInstance = this;

    ImGuiPropertyDrawer::RegisterDrawers();

    AddEditorSystems();
    AddEditorWindows();

    EditorThemes::DefaultTheme();

    // Go borderless so the editor draws its own Rider-style title bar (menus +
    // window controls live in the menu bar). The game build keeps OS chrome.
    WindowHandler::EnableCustomTitleBar(true);

    Engine::SetWorld(AssetRegistry::Get()->CreateNewAsset<EditorWorld>(World::EmptyWorldPath));
}

Editor::~Editor()
{
    for (int i = (int)myWindows.size() - 1; i >= 0; --i)
    {
        RemoveWindow(myWindows[i], true);
    }
    
    for (int i = mySystems.size() - 1; i >= 0; --i)
    {
        del(mySystems[i]);
    }
    mySystems.Clear();

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

    HandleZoomShortcuts();

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

void Editor::HandleZoomShortcuts()
{
    // Ctrl + '=' / '-' zooms the whole UI; Ctrl + '0' resets to 100%. Like an IDE.
    const ImGuiIO& io = ImGui::GetIO();
    if (!io.KeyCtrl)
        return;

    const float step = 0.1f;
    const float user = VulkanImGui::GetUserScale();

    if (ImGui::IsKeyPressed(ImGuiKey_Equal, false) || ImGui::IsKeyPressed(ImGuiKey_KeypadAdd, false))
        VulkanImGui::RequestUserScale(user + step);
    else if (ImGui::IsKeyPressed(ImGuiKey_Minus, false) || ImGui::IsKeyPressed(ImGuiKey_KeypadSubtract, false))
        VulkanImGui::RequestUserScale(user - step);
    else if (ImGui::IsKeyPressed(ImGuiKey_0, false) || ImGui::IsKeyPressed(ImGuiKey_Keypad0, false))
        VulkanImGui::RequestUserScale(1.0f);
}

void Editor::RemoveWindow(EditorWindow* inEditorWindow, const bool inIsShutdown)
{
    if (!inIsShutdown)
    {
        Engine::TickNextFrame.Bind([inEditorWindow]()
        {
            myInstance->myWindows.Remove(inEditorWindow);
            delete inEditorWindow;
        });
        
        return;
    }
    
    myInstance->myWindows.Remove(inEditorWindow);
    delete inEditorWindow;
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
    LOG("Start PIE");
    if (!Engine::GetWorld())
    {
        ImGui::NotifyError("Play In Editor", "Cannot start play in editor without a world open.");
        return;
    }
    myIsPIE = true;
    
    Engine::SetWorld(AssetRegistry::Get()->CreateAssetInstance<GameWorld>(Engine::GetWorld()->GetSourcePath()));
}

void Editor::StopPIE()
{
    LOG("Stop PIE");
    myIsPIE = false;

    // TODO: This should probably be done at first initialization also..
    Engine::SetWorld(AssetRegistry::Get()->CreateAssetInstance<EditorWorld>(Engine::GetWorld()->GetSourcePath()));
}

SharedPtr<EditorWorld> Editor::GetEditorWorld()
{
    return std::static_pointer_cast<EditorWorld>(Engine::GetWorld());
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
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    const ImGuiStyle& style = ImGui::GetStyle();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    // Taller title bar: a bigger frame padding makes Begin() reserve more menu-bar
    // height. Kept active only across Begin + the title-bar render so the extra
    // height doesn't leak into the docked windows' widgets.
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, ImGui::GetFontSize() * 0.62f));

    ImGui::Begin("EditorInterface", &open, window_flags);

    if (EditorToolbar* toolbar = GetSystem<EditorToolbar>())
        toolbar->RenderTitleBar();

    ImGui::PopStyleVar(4);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}

void Editor::AddEditorWindows()
{
    const Type* editorWindowClass = ReflectionSystem::GetType<EditorWindow>();

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
    const Type* editorSystemClass = ReflectionSystem::GetType<EditorSystem>();

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
