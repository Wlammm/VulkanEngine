#include "EditorPch.h"
#include "ImGuiPropertyDrawer.h"

#include "Engine/Engine.h"
#include "Engine/Reflection/Type.h"
#include "Engine/Reflection/ReflectionSystem.h"

void ImGuiPropertyDrawer::RegisterDrawers()
{
    RegisterDrawer<float>(ReflectionSystem::GetType<float>(), [](float& inVal, const Field& inField)
    {
        return ImGui::InputFloat("##float", &inVal);
    });

    RegisterDrawer<int>(ReflectionSystem::GetType<int>(), [](int& inVal, const Field& inField)
    {
        return ImGui::InputInt("##int", &inVal);
    });

    RegisterDrawer<bool>(ReflectionSystem::GetType<bool>(), [](bool& inVal, const Field& inField)
    {
        return ImGui::Checkbox("##bool", &inVal);
    });
    
    RegisterDrawer<glm::vec2>(ReflectionSystem::GetType<glm::vec2>(), [](glm::vec2& inVal, const Field& inField)
    {
        return ImGui::InputFloat2("##vec2", &inVal.x);
    });

    RegisterDrawer<glm::vec3>(ReflectionSystem::GetType<glm::vec3>(), [](glm::vec3& inVal, const Field& inField)
    {
        if (inField.HasMetadata("ExposeAsColor"))
            return ImGui::ColorEdit3("##ColorEdit3", &inVal.x);
        return ImGui::DragFloat3("##vec3", &inVal.x);
    });

    RegisterDrawer<glm::vec4>(ReflectionSystem::GetType<glm::vec4>(), [](glm::vec4& inVal, const Field& inField)
    {
        if (inField.HasMetadata("ExposeAsColor"))
            return ImGui::ColorEdit3("##ColorEdit3", &inVal.x);
        return ImGui::InputFloat4("##vec4", &inVal.x);
    });

    RegisterDrawer<glm::quat>(ReflectionSystem::GetType<glm::quat>(), [](glm::quat& inVal, const Field& inField)
    {
        glm::vec3 rotation = glm::degrees(glm::eulerAngles(inVal));
        
        if (ImGui::InputFloat3("##quat", &rotation.x))
        {
            inVal = glm::quat(glm::radians(rotation));
            return true;
        }
        return false;
    });
}

bool ImGuiPropertyDrawer::DrawProperty(const Field& inField, void* inInstance)
{
    ImGui::PushID(inInstance);
    ImGui::Text(inField.GetName().c_str());

    float windowWidth = ImGui::GetWindowSize().x;
    
    ImGui::SameLine(windowWidth * 0.35f, 0);
    ON_SCOPE_EXIT([](){ ImGui::PopID(); });

    auto it = myDrawers.find(inField.GetType());
    if (it != myDrawers.end())
    {
        void* valPtr = inField.GetPointerToValue(inInstance);
        ImGui::PushID(valPtr);
        const bool returnVal =  it->second(valPtr, inField);
        ImGui::PopID();
        return returnVal;
    }
    else
    {
        std::string text = "No drawer registered for type: " + inField.GetType()->GetName();
        ImGui::Text(text.c_str());
        return false;
    }
}
