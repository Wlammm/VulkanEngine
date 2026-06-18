#include "EditorPch.h"
#include "EditorToolbar.h"

#include "Engine/Engine.h"
#include "Engine/Reflection/ReflectionSystem.h"
#include "Engine/Windows/WindowHandler.h"

EditorToolbar::EditorToolbar()
{
    for (const Type* entry : ReflectionSystem::GetAllTypes())
    {
        for (const Method* method : entry->GetMethodsWithMetadata("EditorMenuItem"))
        {
            myToolbarMethods.Add(method);
        }
    }
}

List<std::string> SplitPath(const std::string& inInput)
{
    List<std::string> result;

    // Strip one level of outer quotes if present
    size_t actualStart = 0;
    size_t actualEnd = inInput.length();

    if (inInput.length() >= 2 && inInput.front() == '"' && inInput.back() == '"') {
        actualStart = 1;
        actualEnd = inInput.length() - 1;
    }

    std::string trimmed = inInput.substr(actualStart, actualEnd - actualStart);

    size_t start = 0;
    size_t end;

    while ((end = trimmed.find('/', start)) != std::string::npos) {
        result.Add(trimmed.substr(start, end - start));
        start = end + 1;
    }

    if (start < trimmed.size()) {
        result.Add(trimmed.substr(start));
    }

    return result;
}


void EditorToolbar::Tick()
{
    // The title bar is rendered from Editor::BeginMainDockSpace via RenderTitleBar()
    // (inside a taller frame-padding scope), so nothing to do in the systems tick.
}

void EditorToolbar::RenderTitleBar()
{
    if (ImGui::BeginMenuBar())
    {
        // Drawn first so the menus + buttons render on top of the gradient.
        DrawTitleBarBackground();

        for (const Method* method : myToolbarMethods)
        {
            List<std::string> args = method->GetMetadataArgs("EditorMenuItem");
            check(args.size() >= 1);

            std::string pathArg = args[0];
            const List<std::string> pathParts = SplitPath(pathArg);

           if (pathParts.size() > 1)
               RenderMultipleParts(pathParts, [method](){ method->Invoke(nullptr);} );
           else
               RenderSinglePart(pathParts, [method](){ method->Invoke(nullptr);} );
        }

        for (const ToolbarButtonInfo& button : myToolbarButtons)
        {
            const List<std::string> pathParts = SplitPath(button.myPath);

            if (pathParts.size() > 1)
                RenderMultipleParts(pathParts, [button](){ button.myCallback(); });
            else
                RenderSinglePart(pathParts, [button](){ button.myCallback(); });
        }

        DrawWindowControls();
        UpdateDragRegion();

        ImGui::EndMenuBar();
    }
}

void EditorToolbar::DrawTitleBarBackground()
{
    // Subtle vertical fade so the bar melts into the dark editor below it.
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    const ImVec2 winPos = ImGui::GetWindowPos();
    const float width = ImGui::GetWindowWidth();
    const float barHeight = ImGui::GetFrameHeight();

    const ImU32 topColor = IM_COL32(0x28, 0x2A, 0x30, 255);
    const ImU32 bottomColor = IM_COL32(0x1B, 0x1C, 0x1E, 255);
    drawList->AddRectFilledMultiColor(
        winPos,
        ImVec2(winPos.x + width, winPos.y + barHeight),
        topColor, topColor, bottomColor, bottomColor);

    // Centered application name (subtle, like an IDE).
    const std::wstring& wideTitle = Engine::GetEngineProperties().Title;
    const std::string title(wideTitle.begin(), wideTitle.end());
    if (!title.empty())
    {
        const ImVec2 textSize = ImGui::CalcTextSize(title.c_str());
        const ImVec2 textPos(
            winPos.x + (width - textSize.x) * 0.5f,
            winPos.y + (barHeight - textSize.y) * 0.5f);
        drawList->AddText(textPos, IM_COL32(0x93, 0x98, 0xA0, 255), title.c_str());
    }
}

void EditorToolbar::DrawWindowControls()
{
    const float barHeight = ImGui::GetFrameHeight();
    const float buttonWidth = barHeight * 1.4f;

    // Right-align the three window buttons.
    ImGui::SameLine(ImGui::GetWindowWidth() - buttonWidth * 3.0f);

    DrawWindowButton("##win_minimize", WindowButton::Minimize, barHeight, buttonWidth);
    ImGui::SameLine(0.0f, 0.0f);
    DrawWindowButton("##win_maximize", WindowButton::Maximize, barHeight, buttonWidth);
    ImGui::SameLine(0.0f, 0.0f);
    DrawWindowButton("##win_close", WindowButton::Close, barHeight, buttonWidth);
}

void EditorToolbar::DrawWindowButton(const char* inId, WindowButton inButton, float inHeight, float inWidth)
{
    const ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::InvisibleButton(inId, ImVec2(inWidth, inHeight));
    const bool hovered = ImGui::IsItemHovered();
    const bool held = ImGui::IsItemActive();

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    if (hovered)
    {
        ImU32 bg;
        if (inButton == WindowButton::Close)
            bg = held ? IM_COL32(0xB3, 0x0F, 0x18, 255) : IM_COL32(0xE8, 0x11, 0x23, 255);
        else
            bg = held ? IM_COL32(0x3B, 0x3E, 0x45, 255) : IM_COL32(0x31, 0x34, 0x3A, 255);
        drawList->AddRectFilled(pos, ImVec2(pos.x + inWidth, pos.y + inHeight), bg);
    }

    const ImU32 fg = (hovered && inButton == WindowButton::Close)
        ? IM_COL32(255, 255, 255, 255)
        : IM_COL32(0xDC, 0xDE, 0xE3, 255);

    const float cx = pos.x + inWidth * 0.5f;
    const float cy = pos.y + inHeight * 0.5f;
    const float s = ImGui::GetFontSize() * 0.30f;          // glyph half-size
    float th = ImGui::GetFontSize() * 0.07f;               // glyph stroke thickness
    if (th < 1.0f)
        th = 1.0f;

    switch (inButton)
    {
    case WindowButton::Minimize:
        drawList->AddLine(ImVec2(cx - s, cy), ImVec2(cx + s, cy), fg, th);
        break;
    case WindowButton::Maximize:
        if (WindowHandler::IsWindowMaximized())
        {
            // Classic "restore" glyph: a front square (lower-left) plus the visible
            // top-right corner of a square behind it.
            const float o = s * 0.5f;
            // Back square: only the top edge and right edge peek out behind the front.
            drawList->AddLine(ImVec2(cx - s + o, cy - s), ImVec2(cx + s, cy - s), fg, th);     // top
            drawList->AddLine(ImVec2(cx + s, cy - s), ImVec2(cx + s, cy + s - o), fg, th);     // right
            // Front square: full outline.
            drawList->AddRect(ImVec2(cx - s, cy - s + o), ImVec2(cx + s - o, cy + s), fg, 0.0f, 0, th);
        }
        else
        {
            drawList->AddRect(ImVec2(cx - s, cy - s), ImVec2(cx + s, cy + s), fg, 0.0f, 0, th);
        }
        break;
    case WindowButton::Close:
        drawList->AddLine(ImVec2(cx - s, cy - s), ImVec2(cx + s, cy + s), fg, th);
        drawList->AddLine(ImVec2(cx - s, cy + s), ImVec2(cx + s, cy - s), fg, th);
        break;
    }

    if (ImGui::IsItemClicked())
    {
        switch (inButton)
        {
        case WindowButton::Minimize: WindowHandler::Minimize(); break;
        case WindowButton::Maximize: WindowHandler::ToggleMaximize(); break;
        case WindowButton::Close:    WindowHandler::RequestClose(); break;
        }
    }
}

void EditorToolbar::UpdateDragRegion()
{
    // Tell the Win32 layer which part of the bar is empty (and therefore draggable).
    const ImGuiIO& io = ImGui::GetIO();
    const ImVec2 winPos = ImGui::GetWindowPos();
    const float width = ImGui::GetWindowWidth();
    const float barHeight = ImGui::GetFrameHeight();

    const bool inBar =
        io.MousePos.x >= winPos.x && io.MousePos.x < winPos.x + width &&
        io.MousePos.y >= winPos.y && io.MousePos.y < winPos.y + barHeight;

    const bool overDrag = inBar && !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemActive();

    WindowHandler::SetTitleBarHeight(static_cast<int>(barHeight));
    WindowHandler::SetTitleBarDragHovered(overDrag);
}

void EditorToolbar::AddToolbarButton(const std::string& inPath, const Delegate<void()>& inCallback)
{
    myToolbarButtons.Emplace(inPath, inCallback);
}

void EditorToolbar::RenderMultipleParts(const List<std::string>& inPathParts, const Delegate<void()> inCallback)
{
    if (!ImGui::BeginMenu(inPathParts[0].c_str()))
        return;
        
    int numMenusOpen = 0;
    for (int i = 1; i < inPathParts.size(); ++i)
    {
        const std::string part = inPathParts[i];
        const bool isLastPart = i == inPathParts.size() - 1;
                
        if (isLastPart)
        {
            ImGui::PushID(&inCallback);
            if (ImGui::MenuItem(part.c_str()))
            {
                inCallback();
            }
            ImGui::PopID();
        }
        else
        {
            if (ImGui::BeginMenu(part.c_str()))
            {
                numMenusOpen++;
            }
            else
            {
                break;
            }
        }
    }

    for (int j = 0; j < numMenusOpen; ++j)
    {
        ImGui::EndMenu();
    }
        
    ImGui::EndMenu();
}

void EditorToolbar::RenderSinglePart(const List<std::string>& inPathParts, const Delegate<void()> inCallback)
{
    if (ImGui::MenuItem(inPathParts[0].c_str()))
    {
        inCallback();
    }
}
