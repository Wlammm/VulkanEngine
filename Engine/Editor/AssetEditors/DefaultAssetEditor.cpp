#include "EditorPch.h"
#include "DefaultAssetEditor.h"

#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/Reflection/Type.h"
#include "ImGui/ImGuiPropertyDrawer.h"

bool DefaultAssetEditor::EditsAsset(const Type* inType) const
{
    return true;
}

void DefaultAssetEditor::DrawAssetEditor()
{
    for (const Field& field : GetType()->GetFieldsWithMetadataRecursive("SerializeField"))
    {
        if (ImGuiPropertyDrawer::DrawProperty(field, GetAsset().get()))
        {
            // TODO: Should we have a prompt to save instead? 
            AssetRegistry::Get()->SaveAsset(GetAsset());
        }
    }
}
