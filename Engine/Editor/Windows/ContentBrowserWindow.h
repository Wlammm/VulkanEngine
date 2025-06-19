#pragma once
#include "EditorWindow.h"
#include "Editor/Utils/EditorConfirmPrompt.h"

class Texture;

struct ContentBrowserItem
{
    ContentBrowserItem(const std::filesystem::path& aPath, vk::DescriptorSet inTexture)
    {
        myTexture = inTexture;
        myPath = aPath;
    }

    std::filesystem::path myPath;
    vk::DescriptorSet myTexture;
    bool myIsSelected = false;
    bool myIsDirectory = false;
};

class ContentBrowserWindow : public EditorWindow
{
    
public:
    ContentBrowserWindow();

    void Tick() override;

    void ReloadDirectory();

private:
    void RenderContentView(const std::filesystem::path& inPath, const int inID);
    void RenderBreadcrumbs();
    void RenderFilewatcher();
    void UpdateSearch();
    void UpdateRightClickMenu();

    void BeginRenaming();
    void UpdateRenaming();

    void DeleteItem();

    void HandleDoubleClick(const ContentBrowserItem& inClickedItem);

    std::string GetPayloadType(const std::filesystem::path& inPath);

    void LoadDirectory(const std::filesystem::path& inPath);
    void LoadDirectories(const List<std::filesystem::path>& inPaths);
    void LoadAllDirectories(const std::filesystem::path& inPath);
    void EnterPath(const std::filesystem::path& inPath);

    bool IsExcludedPath(const std::filesystem::path& inPath);

    vk::DescriptorSet GetIconFromPath(const std::filesystem::path& inPath) const; 

    enum eFileType
    {
        FILETYPE_FOLDER,
        FILETYPE_FILE,
        FILETYPE_COUNT
    };
    
private:
    float myTooltipProgress = 0.0f;
    float myTooltipDuration = 0.5f;
	bool myCanSeeTooltip = false;

    std::filesystem::path myRoot = "Assets";
    std::filesystem::path myCurrentPath = "";
    std::filesystem::path mySelectedTreePath = "";

    std::string mySearchString{};
    List<std::filesystem::path> mySearchPaths;

    std::map<std::filesystem::path, List<std::filesystem::path>> myDirectories;

    List<ContentBrowserItem> myItems;

    std::deque<std::filesystem::path> myHistory;
    std::deque<std::filesystem::path> myForwardHistory;
    
    std::array<vk::DescriptorSet, eFileType::FILETYPE_COUNT> myFolderIcons;
    List<std::string> myExcludedExtensions = { ".dll", ".exe", ".pdb", ".ini" };
    
    std::string myDragDropPath = "";

    int mySelectedItem = -1;
    
    int myRenamingItem = -1;
    std::string myNewFileName = "";
    bool myRenamingInProgress = false;

    bool mySearchBarOpenedThisFrame = false;

    float myTextureSize = 64;
    float myTextureSizeMin = 48;
    float myTextureSizeMax = 140;

    uint32_t myFocusedResult = 0;

    EditorConfirmPrompt myConfirmPrompt;
};
