#include "EditorPch.h"
#include "EditorToolbar.h"

#include "Engine/Engine.h"
#include "Engine/Reflection/ReflectionSystem.h"

EditorToolbar::EditorToolbar()
{
    ReflectionSystem& reflectionSystem = Engine::GetEngineSystem<ReflectionSystem>();

    for (const Class* entry : reflectionSystem.GetAllClasses())
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
    if (ImGui::BeginMenuBar())
    {
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
        
        ImGui::EndMenuBar();
    }
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
