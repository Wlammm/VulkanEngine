#pragma once
#include "Editor/EditorSystem/EditorSystem.h"

class Asset;
class AssetEditor;

class AssetEditorSystem : public EditorSystem
{
public:
    AssetEditorSystem();
    ~AssetEditorSystem() override;
    
    AssetEditor* OpenAssetEditor(SharedPtr<Asset> inAsset, const Type* inAssetType);
    void CloseAssetEditor(AssetEditor* inEditor);
    
    void Tick() override;
    
private:
    List<AssetEditor*> myOpenEditors{};
    
    List<AssetEditor*> myAssetEditors{};
    AssetEditor* myDefaultAssetEditor = nullptr;
};
