#pragma once
#include "EditorSystem.h"
#include "Engine/Delegates/Delegate.hpp"

class Method;

class EditorToolbar : public EditorSystem
{
public:
    EditorToolbar();

    void Tick() override;

    // Draws the custom title bar (menus, app name, window controls) into the
    // dockspace host's menu bar. Called by the editor while a taller frame
    // padding is active so the bar reads as a proper title bar.
    void RenderTitleBar();

    void AddToolbarButton(const std::string& inPath, const Delegate<void()>& inCallback);
    
private:
    void RenderMultipleParts(const List<std::string>& inPathParts, const Delegate<void()> inCallback);
    void RenderSinglePart(const List<std::string>& inPathParts, const Delegate<void()> inCallback);

    // Custom title bar pieces drawn into the menu bar.
    enum class WindowButton { Minimize, Maximize, Close };
    void DrawTitleBarBackground();
    void DrawWindowControls();
    void DrawWindowButton(const char* inId, WindowButton inButton, float inHeight, float inWidth);
    void UpdateDragRegion();

private:
    List<const Method*> myToolbarMethods{};

    struct ToolbarButtonInfo
    {
        std::string myPath ;
        Delegate<void()> myCallback;
    };

    List<ToolbarButtonInfo> myToolbarButtons{};
};
