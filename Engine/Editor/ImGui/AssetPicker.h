#pragma once
#include "Engine/AssetRegistry/AssetRegistry.h"
#include "Engine/AssetRegistry/AssetUtils.h"
#include "Engine/Utils/String.hpp"

class Asset;

namespace ImGui
{
    template<typename AssetType>
    static bool AssetPicker(SharedPtr<AssetType>& inOutAsset)
    {
        ImGui::PushID("AssetPicker");
        ON_SCOPE_EXIT([](){ ImGui::PopID(); });
        
        const List<SourcePath> assets = AssetUtils::GetAllSourcePathsWithExtensions({".fbx", ".gltf"});
        
        std::string assetName = "None";
        if (inOutAsset)
            assetName = inOutAsset->GetSourcePath().filename().string();
        
        if (ImGui::Button(assetName.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
        {
            ImGui::OpenPopup("AssetPicker");
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
                    inOutAsset = AssetRegistry::Get()->GetAsset(asset, ReflectionSystem::GetType(inOutAsset.get()));
                    return true;
                }
            }
        }
        
        
        //Or click this button
        return false;
    }
};