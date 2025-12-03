#pragma once
#include "AssetEditor.h"

class DefaultAssetEditor : public AssetEditor
{
public:
    bool EditsAsset(const Type* inType) const override;
    
    void DrawAssetEditor() override;
};
