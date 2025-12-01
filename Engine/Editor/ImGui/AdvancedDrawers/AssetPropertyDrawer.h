#pragma once
#include "Editor/ImGui/ImGuiPropertyDrawer.h"

class AssetPropertyDrawer : public AdvancedPropertyDrawer
{
public:
    bool DrawsType(const Type* inType) const override;
    
    bool Draw(void* inInstance, const Field& inField) const override;
};
