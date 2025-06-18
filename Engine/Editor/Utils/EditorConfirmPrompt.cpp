#include "EditorPch.h"
#include "EditorConfirmPrompt.h"

#include "Engine/Core/Input.h"

EditorConfirmPrompt::EditorConfirmPrompt(const std::string& inTitle, const std::string& inBody, const Delegate<void()>& inCallback)
{
    myTitle = inTitle;
    myBody = inBody;
    myCallback = inCallback;
}

void EditorConfirmPrompt::Show() const
{
    ImGui::OpenPopup((myTitle + "##prompt").c_str());
}

void EditorConfirmPrompt::Update()
{
    if(ImGui::BeginPopupModal((std::string(myTitle) + "##Prompt").c_str(), nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(myBody.c_str());

        if(ImGui::Button("Yes") || Input::IsKeyDown(KeyCode::Enter))
        {
            if(myCallback.IsValid())
            {
                myCallback();
            }

            ImGui::CloseCurrentPopup();

            myTitle.clear();
            myBody.clear();
            myCallback = nullptr;
        }

        ImGui::SameLine(50.f);

        if(ImGui::Button("No") || Input::IsKeyDown(KeyCode::Escape))
        {
            ImGui::CloseCurrentPopup();

            myTitle.clear();
            myBody.clear();
            myCallback = nullptr;
        }
	
        ImGui::EndPopup();
    }
}
