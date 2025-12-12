#pragma once
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/AssetRegistry/AssetUtils.h"
#include "Engine/Utils/String.hpp"

class Asset;

namespace ImGui
{
    static bool AssetPicker(SharedPtr<Asset>& inOutAsset, const Type* inAssetType)
    {
        ImGui::PushID("AssetPicker");
        ON_SCOPE_EXIT([](){ ImGui::PopID(); });
        
        // If inOutAsset is nullptr we need a way to call the virtual function that is in the asset to figure out the associated extensions. Thats why we create a temporary instance.
        // TODO: Fix this hacky create instance solution.
        List<std::string> associatedExtensions;
        if (inOutAsset)
        {
            associatedExtensions = inAssetType->GetMethod("GetAssetExtensions")->Invoke<List<std::string>>(inOutAsset.get());
        }
        else
        {
            Asset* instance = inAssetType->CreateInstance<Asset>();
            associatedExtensions = inAssetType->GetMethod("GetAssetExtensions")->Invoke<List<std::string>>(instance);
            del(instance);
        }
        
        const List<SourcePath> assets = AssetUtils::GetAllSourcePathsWithExtensions(associatedExtensions);
        
        std::string assetName = "None";
        if (inOutAsset)
            assetName = inOutAsset->GetSourcePath().filename().string();
        
        if (ImGui::Button(assetName.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
        {
            ImGui::OpenPopup("AssetPicker");
        }
        
        if (ImGui::BeginDragDropTarget())
        {
            ON_SCOPE_EXIT([](){ ImGui::EndDragDropTarget(); });
            for (const std::string& extension : associatedExtensions)
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(extension.c_str()))
                {
                    const SourcePath assetPath = *(std::string*)payload->Data;
                    inOutAsset = AssetRegistry::Get()->GetAssetSynchronous(assetPath, inAssetType);
                    return true;
                }
            }
        }
        
        if (ImGui::BeginPopupContextItem("AssetPicker"))
        {
            std::string searchField = "";
            ImGui::InputTextWithHint("##assetpickerSearchField", "Search", &searchField);
            
            ImGui::Separator();

            ON_SCOPE_EXIT([](){ ImGui::EndPopup(); });
            
            if (ImGui::Selectable("None", inOutAsset == nullptr))
            {
                inOutAsset = nullptr;
            }
            
            std::string lowerCaseSearch = String::ToLowerCopy(searchField);
            for (const SourcePath& asset : assets)
            {
                if (!String::ToLowerCopy(asset.filename().string()).contains(lowerCaseSearch))
                    continue;
                
                if (ImGui::Selectable(asset.filename().string().c_str(), (inOutAsset ? inOutAsset->GetSourcePath() : "") == asset))
                {
                    inOutAsset = AssetRegistry::Get()->GetAssetSynchronous(asset, inAssetType);
                    return true;
                }
            }
        }
        
        return false;
    }
};