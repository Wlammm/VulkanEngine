#include "EditorPch.h"
#include "ImGuiPropertyDrawer.h"

#include "Engine/Engine.h"
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/ReflectionSystem.h"

void ImGuiPropertyDrawer::RegisterDrawers()
{
    ReflectionSystem& reflectionSystem = Engine::GetEngineSystem<ReflectionSystem>();

    RegisterDrawer<float>(reflectionSystem.GetClass<float>(), [](float& inVal, const Field& inField)
    {
        return ImGui::InputFloat("##float", &inVal);
    });

    RegisterDrawer<int>(reflectionSystem.GetClass<int>(), [](int& inVal, const Field& inField)
    {
        return ImGui::InputInt("##int", &inVal);
    });

    RegisterDrawer<bool>(reflectionSystem.GetClass<bool>(), [](bool& inVal, const Field& inField)
    {
        return ImGui::Checkbox("##bool", &inVal);
    });
    
    RegisterDrawer<glm::vec2>(reflectionSystem.GetClass<glm::vec2>(), [](glm::vec2& inVal, const Field& inField)
    {
        return ImGui::InputFloat2("##vec2", &inVal.x);
    });

    RegisterDrawer<glm::vec3>(reflectionSystem.GetClass<glm::vec3>(), [](glm::vec3& inVal, const Field& inField)
    {
        if (inField.HasMetadata("ExposeAsColor"))
            return ImGui::ColorEdit3("##ColorEdit3", &inVal.x);
        return ImGui::DragFloat3("##vec3", &inVal.x);
    });

    RegisterDrawer<glm::vec4>(reflectionSystem.GetClass<glm::vec4>(), [](glm::vec4& inVal, const Field& inField)
    {
        if (inField.HasMetadata("ExposeAsColor"))
            return ImGui::ColorEdit3("##ColorEdit3", &inVal.x);
        return ImGui::InputFloat4("##vec4", &inVal.x);
    });

    RegisterDrawer<glm::quat>(reflectionSystem.GetClass<glm::quat>(), [](glm::quat& inVal, const Field& inField)
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

    auto it = myDrawers.find(inField.GetClass());
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
        std::string text = "No drawer registered for type: " + inField.GetClass()->GetName();
        ImGui::Text(text.c_str());
        return false;
    }
}
