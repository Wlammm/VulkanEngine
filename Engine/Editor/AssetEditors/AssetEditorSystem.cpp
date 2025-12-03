#include "EditorPch.h"
#include "AssetEditorSystem.h"

#include "AssetEditor.h"
#include "DefaultAssetEditor.h"
#include "Engine/Reflection/ReflectionSystem.h"

AssetEditorSystem::AssetEditorSystem()
{
    const Type* assetEditorType = ReflectionSystem::GetType<AssetEditor>();
    
    for (const Type* derivedType : assetEditorType->GetDerivedTypes())
    {
        if (derivedType->IsA<DefaultAssetEditor>())
            continue;
        
        myAssetEditors.Add(derivedType->CreateInstance<AssetEditor>());
    }
    
    myDefaultAssetEditor = new DefaultAssetEditor();
}

AssetEditorSystem::~AssetEditorSystem()
{
    for (AssetEditor* editor : myAssetEditors)
    {
        del(editor);
    }
    myAssetEditors.Clear();
    myOpenEditors.Clear();
    
    del(myDefaultAssetEditor);
}

AssetEditor* AssetEditorSystem::OpenAssetEditor(SharedPtr<Asset> inAsset, const Type* inAssetType)
{
    const Type* pickedEditorType = ReflectionSystem::GetType(myDefaultAssetEditor);
    
    for (AssetEditor* editor : myAssetEditors)
    {
        if (editor->EditsAsset(inAssetType))
        {
            pickedEditorType = ReflectionSystem::GetType(editor);
            break;
        }
    }

    AssetEditor* editorInstance = pickedEditorType->CreateInstance<AssetEditor>();
    editorInstance->SetAsset(inAsset);
    editorInstance->SetType(inAssetType);
    myOpenEditors.Add(editorInstance);
    return editorInstance;
}

void AssetEditorSystem::CloseAssetEditor(AssetEditor* inEditor)
{
    myOpenEditors.Remove(inEditor);
    del(inEditor);
}

void AssetEditorSystem::Tick()
{
    for (int i = myOpenEditors.size() - 1; i >= 0; --i)
    {
        AssetEditor* editor = myOpenEditors[i];
        editor->DoDrawAssetEditor();
        
        if (editor->ShouldClose())
            CloseAssetEditor(editor);
    }
}
