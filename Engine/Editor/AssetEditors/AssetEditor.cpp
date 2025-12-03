#include "EditorPch.h"
#include "AssetEditor.h"

#include "Engine/AssetRegistry/Asset.h"

void AssetEditor::DoDrawAssetEditor()
{
    check(myAsset && myType && "These needs to be set.");
    
    ImGui::Begin(myAsset->GetName().c_str(), &myIsOpen);
    DrawAssetEditor();
    ImGui::End();
}
