#include "EditorPch.h"
#include "ImGuiPropertyDrawer.h"

#include "AdvancedDrawers/AssetPropertyDrawer.h"
#include "Engine/Engine.h"
#include "Engine/Reflection/Type.h"
#include "Engine/Reflection/ReflectionSystem.h"

ImGuiPropertyDrawer::~ImGuiPropertyDrawer()
{
    for (AdvancedPropertyDrawer* drawer : myAdvancedDrawers)
    {
        del(drawer);
    }
    myAdvancedDrawers.Clear();
}

void ImGuiPropertyDrawer::RegisterDrawers()
{
    RegisterSimpleDrawer<float>(ReflectionSystem::GetType<float>(), [](float& inVal, const Field& inField)
    {
        return ImGui::InputFloat("##float", &inVal);
    });

    RegisterSimpleDrawer<int>(ReflectionSystem::GetType<int>(), [](int& inVal, const Field& inField)
    {
        return ImGui::InputInt("##int", &inVal);
    });

    RegisterSimpleDrawer<bool>(ReflectionSystem::GetType<bool>(), [](bool& inVal, const Field& inField)
    {
        return ImGui::Checkbox("##bool", &inVal);
    });
    
    RegisterSimpleDrawer<glm::vec2>(ReflectionSystem::GetType<glm::vec2>(), [](glm::vec2& inVal, const Field& inField)
    {
        return ImGui::InputFloat2("##vec2", &inVal.x);
    });

    RegisterSimpleDrawer<glm::vec3>(ReflectionSystem::GetType<glm::vec3>(), [](glm::vec3& inVal, const Field& inField)
    {
        if (inField.HasMetadata("ExposeAsColor"))
            return ImGui::ColorEdit3("##ColorEdit3", &inVal.x);
        return ImGui::DragFloat3("##vec3", &inVal.x);
    });

    RegisterSimpleDrawer<glm::vec4>(ReflectionSystem::GetType<glm::vec4>(), [](glm::vec4& inVal, const Field& inField)
    {
        if (inField.HasMetadata("ExposeAsColor"))
            return ImGui::ColorEdit3("##ColorEdit3", &inVal.x);
        return ImGui::InputFloat4("##vec4", &inVal.x);
    });

    RegisterSimpleDrawer<glm::quat>(ReflectionSystem::GetType<glm::quat>(), [](glm::quat& inVal, const Field& inField)
    {
        glm::vec3 rotation = glm::degrees(glm::eulerAngles(inVal));
        
        if (ImGui::InputFloat3("##quat", &rotation.x))
        {
            inVal = glm::quat(glm::radians(rotation));
            return true;
        }
        return false;
    });
    
    RegisterAdvancedDrawer<AssetPropertyDrawer>();
}

bool ImGuiPropertyDrawer::DrawProperty(const Field& inField, void* inInstance)
{
    if (inField.HasMetadata("HideInEditor"))
        return false;
    
    ImGui::PushID(inInstance);
    
    // Remove the my prefix on member variables.
    const std::string prettifiedFieldName = inField.GetName().starts_with("my") ? inField.GetName().substr(2) : inField.GetName();
    
    ImGui::Text(prettifiedFieldName.c_str());

    float windowWidth = ImGui::GetWindowSize().x;
    
    const float propertyTitleWidthPercentage = 0.35f;
    ImGui::SameLine(windowWidth * propertyTitleWidthPercentage, 0);
    ON_SCOPE_EXIT([](){ ImGui::PopID(); });

    auto it = mySimpleDrawers.find(inField.GetType());
    if (it != mySimpleDrawers.end())
    {
        void* valPtr = inField.GetPointerToValue(inInstance);
        ImGui::PushID(valPtr);
        const bool returnVal =  it->second(valPtr, inField);
        ImGui::PopID();
        return returnVal;
    }
    else
    {
        for (const AdvancedPropertyDrawer* drawer : myAdvancedDrawers)
        {
            if (drawer->DrawsType(inField.GetType()))
            {
                void* valuePointer = inField.GetPointerToValue(inInstance);
                ImGui::PushID(valuePointer);
                bool returnVal = drawer->Draw(valuePointer, inField);
                ImGui::PopID();
                return returnVal;
            }
        }
        
        std::string text = "No drawer found for type: " + inField.GetType()->GetName();
        ImGui::Text(text.c_str());
        return false;
    }
}
