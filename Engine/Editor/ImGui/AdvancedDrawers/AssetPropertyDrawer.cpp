#include "EditorPch.h"
#include "AssetPropertyDrawer.h"

#include "Engine/Reflection/Type.h"
#include "ImGui/AssetPicker.h"
#include "Engine/AssetRegistry/Asset.h"

bool AssetPropertyDrawer::DrawsType(const Type* inType) const
{
    if (!inType->GetName().contains("shared_ptr"))
        return false;
    
    if (inType->GetTemplateArguments().IsEmpty())
        return false;

    return inType->GetTemplateArguments().First().myType->IsA<Asset>();
}

bool AssetPropertyDrawer::Draw(void* inInstance, const Field& inField) const
{
    SharedPtr<Asset>* asset = static_cast<SharedPtr<Asset>*>(inInstance);
    return ImGui::AssetPicker(*asset);
}
