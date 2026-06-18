#pragma once
#include "Engine/Engine.h"
#include "Engine/Vulkan/VulkanImGui.h"

class EditorThemes
{
public:
    META(EditorMenuItem("Themes/Default"))
    static void DefaultTheme()
    {
        // JetBrains Rider "New UI" inspired dark theme: neutral surfaces, a single
        // blue accent, subtle borders and soft consistent rounding. Authored at 100%
        // scale - DPI scaling is re-applied at the end.
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // Convert 0xRRGGBB to ImVec4 so the palette reads like design tokens.
        const auto Col = [](unsigned int inRGB, float inAlpha = 1.0f) -> ImVec4
        {
            return ImVec4(
                ((inRGB >> 16) & 0xFF) / 255.0f,
                ((inRGB >> 8) & 0xFF) / 255.0f,
                (inRGB & 0xFF) / 255.0f,
                inAlpha);
        };

        // --- Palette ---------------------------------------------------------
        // A genuinely dark neutral scale. Tool windows sit dark (L1); lighter greys
        // are reserved for raised/hover/active states so the big surfaces read dark
        // and the structure reads through contrast rather than brightness.
        const ImVec4 bgEmpty    = Col(0x161718); // deepest: dock void / canvas behind all
        const ImVec4 bgWindow   = Col(0x1B1C1E); // L1: tool windows / panels / main bg
        const ImVec4 bgRaised   = Col(0x202226); // L2: menu/title/tab bars, table header
        const ImVec4 bgButton   = Col(0x282A2E); // L3: buttons, raised surfaces
        const ImVec4 bgHover    = Col(0x31343A); // L4: hovered surface
        const ImVec4 bgActive   = Col(0x3B3E45); // L5: pressed surface

        const ImVec4 fieldBg    = Col(0x141517); // recessed input wells (adds depth)
        const ImVec4 fieldHover = Col(0x1C1E21);
        const ImVec4 fieldActive= Col(0x222529);

        const ImVec4 borderCol  = Col(0x303236); // visible-but-subtle borders / separators
        const ImVec4 borderHi   = Col(0x3C3F46);

        const ImVec4 text       = Col(0xDCDEE3);
        const ImVec4 textDim    = Col(0x9398A0);
        const ImVec4 textOff    = Col(0x65686E);

        const ImVec4 accent     = Col(0x3574F0); // primary blue
        const ImVec4 accentHov  = Col(0x4C84F4);
        const ImVec4 selection  = Col(0x2A4574); // selected row fill
        const ImVec4 selectHov  = Col(0x26303D); // subtle hover on rows
        const ImVec4 grab       = Col(0x3C3F46);
        const ImVec4 grabHov    = Col(0x4A4D55);

        // --- Colors ----------------------------------------------------------
        colors[ImGuiCol_Text]                   = text;
        colors[ImGuiCol_TextDisabled]           = textOff;
        colors[ImGuiCol_WindowBg]               = bgWindow;
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = bgRaised;
        colors[ImGuiCol_Border]                 = borderCol;
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                = fieldBg;
        colors[ImGuiCol_FrameBgHovered]         = fieldHover;
        colors[ImGuiCol_FrameBgActive]          = fieldActive;
        colors[ImGuiCol_TitleBg]                = bgWindow;
        colors[ImGuiCol_TitleBgActive]          = bgWindow;
        colors[ImGuiCol_TitleBgCollapsed]       = bgWindow;
        colors[ImGuiCol_MenuBarBg]              = bgRaised;
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_ScrollbarGrab]          = grab;
        colors[ImGuiCol_ScrollbarGrabHovered]   = grabHov;
        colors[ImGuiCol_ScrollbarGrabActive]    = borderHi;
        colors[ImGuiCol_CheckMark]              = accent;
        colors[ImGuiCol_SliderGrab]             = accent;
        colors[ImGuiCol_SliderGrabActive]       = accentHov;
        colors[ImGuiCol_Button]                 = bgButton;
        colors[ImGuiCol_ButtonHovered]          = bgHover;
        colors[ImGuiCol_ButtonActive]           = bgActive;
        colors[ImGuiCol_Header]                 = selection;
        colors[ImGuiCol_HeaderHovered]          = selectHov;
        colors[ImGuiCol_HeaderActive]           = selection;
        colors[ImGuiCol_Separator]              = borderCol;
        colors[ImGuiCol_SeparatorHovered]       = accent;
        colors[ImGuiCol_SeparatorActive]        = accentHov;
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_ResizeGripHovered]      = accent;
        colors[ImGuiCol_ResizeGripActive]       = accentHov;
        colors[ImGuiCol_Tab]                    = bgRaised;
        colors[ImGuiCol_TabHovered]             = bgHover;
        colors[ImGuiCol_TabActive]              = bgWindow;
        colors[ImGuiCol_TabUnfocused]           = bgRaised;
        colors[ImGuiCol_TabUnfocusedActive]     = bgWindow;
        colors[ImGuiCol_DockingPreview]         = ImVec4(accent.x, accent.y, accent.z, 0.40f);
        colors[ImGuiCol_DockingEmptyBg]         = bgEmpty;
        colors[ImGuiCol_PlotLines]              = accent;
        colors[ImGuiCol_PlotLinesHovered]       = accentHov;
        colors[ImGuiCol_PlotHistogram]          = accent;
        colors[ImGuiCol_PlotHistogramHovered]   = accentHov;
        colors[ImGuiCol_TableHeaderBg]          = bgRaised;
        colors[ImGuiCol_TableBorderStrong]      = borderHi;
        colors[ImGuiCol_TableBorderLight]       = borderCol;
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.030f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(accent.x, accent.y, accent.z, 0.35f);
        colors[ImGuiCol_DragDropTarget]         = accentHov;
        colors[ImGuiCol_NavHighlight]           = accent;
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.08f, 0.08f, 0.09f, 0.50f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.05f, 0.05f, 0.06f, 0.60f);

        // --- Geometry: generous padding (fixes cramped/clipped text) + soft,
        //     consistent rounding for a sleek, minimal feel ---------------------
        style.WindowPadding         = ImVec2(10.0f, 10.0f);
        style.WindowMinSize         = ImVec2(32.0f, 32.0f);
        style.FramePadding          = ImVec2(9.0f, 5.0f);
        style.CellPadding           = ImVec2(8.0f, 5.0f);
        style.ItemSpacing           = ImVec2(8.0f, 6.0f);
        style.ItemInnerSpacing      = ImVec2(7.0f, 5.0f);
        style.IndentSpacing         = 20.0f;
        style.ScrollbarSize         = 12.0f;
        style.GrabMinSize           = 11.0f;

        style.WindowBorderSize      = 1.0f;
        style.ChildBorderSize       = 1.0f;
        style.PopupBorderSize       = 1.0f;
        style.FrameBorderSize       = 1.0f; // crisp edges on inputs/buttons (less "flat")
        style.TabBorderSize         = 0.0f;

        style.WindowRounding        = 6.0f;
        style.ChildRounding         = 6.0f;
        style.FrameRounding         = 5.0f;
        style.PopupRounding         = 6.0f;
        style.ScrollbarRounding     = 9.0f;
        style.GrabRounding          = 5.0f;
        style.TabRounding           = 5.0f;

        style.WindowTitleAlign      = ImVec2(0.0f, 0.5f);
        style.ButtonTextAlign       = ImVec2(0.5f, 0.5f);
        style.SeparatorTextBorderSize = 1.0f;
        style.SeparatorTextAlign    = ImVec2(0.0f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_None;
        style.ColorButtonPosition   = ImGuiDir_Right;
        style.DisabledAlpha         = 0.5f;

        // The sizes above are authored at 100% scale. Start() scales the *default*
        // sizes before this theme overrides them, so re-apply DPI scaling here.
        style.ScaleAllSizes(VulkanImGui::GetCurrentDpiScale());
    }

private:
    static unsigned int FromHex(const unsigned int inHex)
    {
        const short a = 0xFF;
        const short r = (inHex >> 16) & 0xFF;
        const short g = (inHex >> 8) & 0xFF;
        const short b = (inHex >> 0) & 0xFF;

        return(a << 24)
           | (r << 0)
           | (g << 8)
           | (b << 16);
    }
};
