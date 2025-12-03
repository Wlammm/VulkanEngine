#pragma once

class Asset;

class AssetEditor
{
public:
    virtual ~AssetEditor() = default;
    
    virtual bool EditsAsset(const Type* inType) const = 0;
    void DoDrawAssetEditor();
    virtual void DrawAssetEditor() = 0;
    
    SharedPtr<Asset> GetAsset() const { return myAsset; }
    const Type* GetType() const { return myType;}
    
    void SetAsset(SharedPtr<Asset> inAsset) { myAsset = inAsset; }
    void SetType(const Type* inType) { myType = inType; }
    
    bool ShouldClose() const { return !myIsOpen; }
    
private:
    SharedPtr<Asset> myAsset;
    const Type* myType;
    
    bool myIsOpen = true;
};
