#include "EditorPch.h"
#include "ImGuiPropertyDrawer.h"

#include "Engine/Engine.h"
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/ReflectionSystem.h"

void ImGuiPropertyDrawer::RegisterDrawers()
{
    ReflectionSystem& reflectionSystem = Engine::GetEngineSystem<ReflectionSystem>();
    
    RegisterDrawer<glm::vec2>(reflectionSystem.GetClass<glm::vec2>(), [](glm::vec2& inVal)
    {
        ImGui::InputFloat2("##vec2", &inVal.x);
    });

    RegisterDrawer<glm::vec3>(reflectionSystem.GetClass<glm::vec3>(), [](glm::vec3& inVal)
    {
        ImGui::InputFloat3("##vec3", &inVal.x);
    });

    RegisterDrawer<glm::vec4>(reflectionSystem.GetClass<glm::vec4>(), [](glm::vec4& inVal)
    {
       ImGui::InputFloat4("##vec4", &inVal.x);
    });
}

void ImGuiPropertyDrawer::DrawProperty(const Field& inField, void* inInstance)
{
    ImGui::PushID(inInstance);
    ImGui::Text(inField.GetName().c_str());
    ImGui::SameLine();
    ON_SCOPE_EXIT([](){ ImGui::PopID(); });

    auto it = myDrawers.find(inField.GetType());
    if (it != myDrawers.end())
    {
        void* valPtr = inField.GetPointerToValue(inInstance);
        it->second(valPtr);
    }
    else
    {
        ImGui::Text("No drawer registered for this type");
    }
}
