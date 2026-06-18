#pragma once
#include "EditorSystem.h"
#include "Engine/Delegates/Delegate.hpp"

class Method;

class EditorToolbar : public EditorSystem
{
public:
    EditorToolbar();
    
    void Tick() override;

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
