#include "EditorPch.h"
#include "ImGuiPropertyDrawer.h"

#include "Engine/Engine.h"
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/ReflectionSystem.h"

void ImGuiPropertyDrawer::RegisterDrawers()
{
    ReflectionSystem& reflectionSystem = Engine::GetEngineSystem<ReflectionSystem>();

    RegisterDrawer<float>(reflectionSystem.GetClass<float>(), [](float& inVal)
    {
        ImGui::InputFloat2("##float", &inVal);
    });

    RegisterDrawer<int>(reflectionSystem.GetClass<int>(), [](int& inVal)
    {
        ImGui::InputInt("##int", &inVal);
    });

    RegisterDrawer<bool>(reflectionSystem.GetClass<bool>(), [](bool& inVal)
    {
        ImGui::Checkbox("##bool", &inVal);
    });
    
    RegisterDrawer<glm::vec2>(reflectionSystem.GetClass<glm::vec2>(), [](glm::vec2& inVal)
    {
        ImGui::InputFloat2("##vec2", &inVal.x);
    });

    RegisterDrawer<glm::vec3>(reflectionSystem.GetClass<glm::vec3>(), [](glm::vec3& inVal)
    {
        ImGui::DragFloat3("##vec3", &inVal.x);
    });

    RegisterDrawer<glm::vec4>(reflectionSystem.GetClass<glm::vec4>(), [](glm::vec4& inVal)
    {
        ImGui::InputFloat4("##vec4", &inVal.x);
    });

    RegisterDrawer<glm::quat>(reflectionSystem.GetClass<glm::quat>(), [](glm::quat& inVal)
    {
        glm::vec3 rotation = glm::degrees(glm::eulerAngles(inVal));
        
        if (ImGui::InputFloat3("##quat", &rotation.x))
        {
            inVal = glm::quat(glm::radians(rotation));
        }
    });
}

void ImGuiPropertyDrawer::DrawProperty(const Field& inField, void* inInstance)
{
    ImGui::PushID(inInstance);
    ImGui::Text(inField.GetName().c_str());
    ImGui::SameLine(0.0f, 40);
    ON_SCOPE_EXIT([](){ ImGui::PopID(); });

    auto it = myDrawers.find(inField.GetType());
    if (it != myDrawers.end())
    {
        void* valPtr = inField.GetPointerToValue(inInstance);
        ImGui::PushID(valPtr);
        it->second(valPtr);
        ImGui::PopID();
    }
    else
    {
        std::string text = "No drawer registered for type: " + inField.GetType()->GetName();
        ImGui::Text(text.c_str());
    }
}
