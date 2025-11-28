#include "EditorPch.h"
#include "ContentBrowserWindow.h"

#include "Editor/Utils/ImGuiTextureUtils.h"
#include "Engine/Engine.h"
#include "Engine/AssetRegistry/AssetRegistry2.h"
#include "Engine/Assets/Texture.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Time.h"

ContentBrowserWindow::ContentBrowserWindow()
    : EditorWindow("Content Browser", true)
{
	myContentBrowserTextures[FILETYPE_FOLDER] = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>("Editor/ContentBrowserIcons/Folder.png");
	myContentBrowserTextureDescriptors[FILETYPE_FOLDER] = ImGuiTextureUtils::CreateDescriptorSetForTexture(myContentBrowserTextures[FILETYPE_FOLDER]);
	
	myContentBrowserTextures[FILETYPE_FILE] = Engine::GetEngineSystem<AssetRegistry2>().GetAsset<Texture>("Editor/ContentBrowserIcons/File.png");
	myContentBrowserTextureDescriptors[FILETYPE_FILE] = ImGuiTextureUtils::CreateDescriptorSetForTexture(myContentBrowserTextures[FILETYPE_FILE]);

	LoadAllDirectories(myRoot);
	LoadDirectory(myRoot);
}

ContentBrowserWindow::~ContentBrowserWindow()
{
	for (int i = 0; i < eFileType::FILETYPE_COUNT; ++i)
	{
		ImGui_ImplVulkan_RemoveTexture(myContentBrowserTextureDescriptors[i]);
	}
}

void ContentBrowserWindow::Tick()
{
    auto& io = ImGui::GetIO();
    if (io.MouseDelta.x != 0.0f || 
        io.MouseDelta.y != 0.0f)
    {
        myTooltipProgress = myTooltipDuration;
    }
    else
    {
        myTooltipProgress -= Time::GetUnscaledDeltaTime();
    }
    myCanSeeTooltip = myTooltipProgress <= 0.0f;

    ImGui::BeginChild("##ContentBrowserOverview", ImVec2(200, 0), true);
    RenderContentView(myRoot, 1);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##ContentBrowserNestled");
    ImGui::BeginChild("##ContentBrowserControls", ImVec2(0, 25));

    RenderBreadcrumbs();

    ImGui::EndChild();

    ImGui::Separator();

    ImGui::BeginChild("##ContentBrowserWatcher", ImVec2(0, 0));

    RenderFilewatcher();

    ImGui::EndChild();
    ImGui::EndChild();

    UpdateSearch();
}

void ContentBrowserWindow::ReloadDirectory()
{
	LoadDirectory(myCurrentPath);
}

void ContentBrowserWindow::RenderContentView(const std::filesystem::path& inPath, const int inID)
{
    ImGui::PushID(inID);

    ImGuiTreeNodeFlags flags = ((mySelectedTreePath == inPath) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

    if (myDirectories[inPath].IsEmpty())
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    bool isOpen = ImGui::TreeNodeEx(inPath.filename().string().c_str(), flags);

    if (ImGui::BeginDragDropTarget())
    {
        if (!inPath.has_extension())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetPayloadType(myDragDropPath).c_str()))
            {
                std::filesystem::path from_path = myDragDropPath;
                std::filesystem::path to_path = inPath;
                to_path += "\\";
                to_path += std::filesystem::path(myDragDropPath).filename();
                if (from_path != to_path)
                {
                    std::filesystem::copy_file(from_path, to_path);
                    std::filesystem::remove(from_path);
                    ReloadDirectory();
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::IsItemClicked())
    {
        mySelectedTreePath = inPath;

        myHistory.clear();
        myForwardHistory.clear();

        std::filesystem::path fullPath{};
        for(auto it = mySelectedTreePath.begin(); it != mySelectedTreePath.end(); it++)
        {
            fullPath = fullPath / *it;

            if(fullPath == inPath)
                break;
		
            myHistory.push_back(fullPath);
        }

        LoadDirectory(inPath);
    }

    if (isOpen)
    {
        for (int i = 0; i < myDirectories[inPath].size(); ++i)
        {
            RenderContentView(myDirectories[inPath][i], inID + i + 1);
        }

        ImGui::TreePop();
    }

    ImGui::PopID();
}

void ContentBrowserWindow::RenderBreadcrumbs()
{
    bool backButtonEnabled = !myHistory.empty();
	bool forwardButtonEnabled = !myForwardHistory.empty();

	if (!backButtonEnabled)
	{
		ImGui::BeginDisabled();
	}

	if (ImGui::Button("<", ImVec2(20, 20)))
	{
		if (!myHistory.empty())
		{
			const std::filesystem::path& previousPath = myHistory.back();
			myForwardHistory.push_back(myCurrentPath);
			LoadDirectory(previousPath);
			myHistory.pop_back();
		}
	}

	if (!backButtonEnabled)
	{
		ImGui::EndDisabled();
	}

	ImGui::SameLine();

	if (!forwardButtonEnabled)
	{
		ImGui::BeginDisabled();
	}

	if (ImGui::Button(">", ImVec2(20, 20)))
	{
		if (!myForwardHistory.empty())
		{
			const std::filesystem::path& path = myForwardHistory.back();
			myHistory.push_back(myCurrentPath);
			LoadDirectory(path);
			myForwardHistory.pop_back();
		}
	}

	if (!forwardButtonEnabled)
	{
		ImGui::EndDisabled();
	}

	if (mySearchPaths.IsEmpty())
	{
		for (auto it = myHistory.begin(); it != myHistory.end(); it++)
		{
			const std::filesystem::path& path = *it;

			ImGui::SameLine();

			ImVec4 color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
			ImGui::TextColored(color, path.stem().string().c_str());

			if (ImGui::IsItemHovered())
			{
				auto drawList = ImGui::GetWindowDrawList();
				drawList->AddLine(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y), ImGui::GetItemRectMax(), ImGui::ColorConvertFloat4ToU32(ImVec4(0.7f, 0.7f, 0.7f, 1)));

				if (ImGui::GetIO().MouseClicked[0])
				{
					LoadDirectory(path);
					myHistory.erase(it, myHistory.end());
					break;
				}
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (!path.has_extension())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetPayloadType(myDragDropPath).c_str()))
					{
						std::filesystem::path from_path = myDragDropPath;
						std::filesystem::path to_path = path;
						to_path += "\\";
						to_path += std::filesystem::path(myDragDropPath).filename();
						if (from_path != to_path)
						{
							std::filesystem::copy_file(from_path, to_path);
							std::filesystem::remove(from_path);
							ReloadDirectory();
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();
			ImGui::Text(">");
		}

		ImGui::SameLine();
		ImGui::Text(myCurrentPath.stem().string().c_str());
	}
	else
	{
		ImGui::SameLine();
		ImGui::Text("Displaying search results");
	}

	ImGui::SameLine();

	std::string text;
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 460.0f - 4.0f);
	ImGui::SetNextItemWidth(300.0f);
	if (ImGui::InputTextWithHint("##ContentBrowserSearchField", "Search", &mySearchString))
	{
		mySearchPaths.Clear();

		std::string searchToLower = String::ToLowerCopy(mySearchString);
		for (auto& directory : std::filesystem::recursive_directory_iterator(myRoot))
		{
			auto& path = directory.path();

			std::string scripts = String::ToLowerCopy(path.filename().string());
			if (mySearchString != "" && scripts.find(searchToLower) == std::string::npos)
				continue;

			mySearchPaths.Add(path);
		}

		if (mySearchString != "")
		{
			LoadDirectories(mySearchPaths);
		}
		else
		{
			LoadDirectory(myRoot);
		}
	}

	ImGui::SameLine();

	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 150.0f - 4.0f);
	ImGui::SetNextItemWidth(150.0f);
	if (ImGui::SliderFloat("##ContentBrowserTextureSize", &myTextureSize, myTextureSizeMin, myTextureSizeMax, "%.0f"))
	{
		myTextureSize = std::clamp(myTextureSize, myTextureSizeMin, myTextureSizeMax);
	}
}

void ContentBrowserWindow::RenderFilewatcher()
{
	UpdateRightClickMenu();
	float padding = 20;
	float cellWidth = myTextureSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = static_cast<int>(panelWidth / cellWidth);

	if (columnCount)
	{
		ImGui::Columns(columnCount, 0, false);
		for (int i = 0; i < myItems.size(); ++i)
		{
			bool wasSelected = false;
			if (i == mySelectedItem)
			{
				wasSelected = true;
				ImGui::PushStyleColor(ImGuiCol_Border, { 0, 0, 1, 1 });
			}

			ImGui::PushID(i);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			if (ImGui::ImageButton(myItems[i].myTexture, { myTextureSize, myTextureSize }))
			{
				mySelectedItem = i;
			}
			ImGui::PopStyleVar();

			if (ImGui::IsItemHovered() && myCanSeeTooltip)
			{
				ImGui::SetTooltip("%s", myItems[i].myPath.filename().string().c_str());
			}

			if (wasSelected)
			{
				ImGui::PopStyleColor();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (std::filesystem::is_directory(myItems[i].myPath))
				{
					mySearchString = "";
					mySearchPaths.Clear();

					EnterPath(myItems[i].myPath.string());
					ImGui::SetScrollY(0);
					ImGui::PopID();
					return;
				}

				HandleDoubleClick(myItems[i]);
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (!myItems[i].myPath.has_extension())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetPayloadType(myDragDropPath).c_str()))
					{
						std::filesystem::path from_path = myDragDropPath;
						std::filesystem::path to_path = myItems[i].myPath;
						to_path += "\\";
						to_path += std::filesystem::path(myDragDropPath).filename();
						if (from_path != to_path)
						{
							std::filesystem::copy_file(from_path, to_path);
							std::filesystem::remove(from_path);
							ReloadDirectory();
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (!myItems[i].myIsDirectory)
			{
				if (ImGui::BeginDragDropSource())
				{
					ImGui::Text(myItems[i].myPath.string().c_str());
					myDragDropPath = myItems[i].myPath.string();
					ImGui::SetDragDropPayload(GetPayloadType(myItems[i].myPath).c_str(), &myDragDropPath, sizeof(myDragDropPath));
					ImGui::EndDragDropSource();
				}
			}

			ImGui::PopID();

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			if (i != myRenamingItem)
			{
				ImGui::Text(myItems[i].myPath.stem().string().c_str());

				if (ImGui::IsItemHovered() && myCanSeeTooltip)
				{
					ImGui::SetTooltip("%s", myItems[i].myPath.filename().string().c_str());
				}
			}
			else
			{
				ImGui::InputText("##renamingfilecontentbrowser", &myNewFileName);
				ImGui::SetKeyboardFocusHere(0);
			}

			ImGui::NextColumn();
		}
	}

	UpdateRenaming();

	myConfirmPrompt.Update();
}

void ContentBrowserWindow::UpdateSearch()
{
	if(ImGui::BeginPopup("Search"))
	{
		struct SearchResult
		{
			std::filesystem::path myPath{};
			int32_t myScore = 0;

			bool operator<(const SearchResult& other)
			{
				return other.myScore < myScore;
			}
		};

		static std::deque<SearchResult> results{};

		const glm::vec<2, unsigned int> displaySize = Engine::GetRenderResolution();
		const ImVec2 windowSize = ImGui::GetWindowSize();
		glm::vec2 windowPosition{displaySize.x * .5f - windowSize.x * .5f, displaySize.y * .5f - windowSize.y * .5f};
		ImGui::SetWindowPos({windowPosition.x, windowPosition.y});

		// FIXME(Ajlin): This line causes mouse clicking selectables to fail
		// but I kept it because it immediately lets us write in the search bar
		ImGui::SetKeyboardFocusHere();

		if(ImGui::InputTextWithHint("##SearchBar", "Search", &mySearchString))
		{
			results.clear();

			for(auto it = std::filesystem::recursive_directory_iterator(myCurrentPath); it != std::filesystem::recursive_directory_iterator(); it++)
			{
				std::string parentPath = myCurrentPath.string();
				std::string path = it->path().string();

				path.erase(path.find(parentPath), parentPath.size() + 1);

				SearchResult result{};
				result.myPath = std::filesystem::path(path);

				results.push_back(result);
			}

			std::string lowerCaseSearch = String::ToLower(mySearchString);

			for(auto it = results.begin(); it != results.end(); it++)
			{
				std::string file = String::ToLowerCopy(it->myPath.filename().string());
				
				size_t index = file.find(lowerCaseSearch);
				if(index != std::string::npos)
				{
					uint32_t scoreIncrement = 1;
					for(size_t cIndex = index; cIndex < index + lowerCaseSearch.size(); cIndex++)
					{							
						it->myScore += scoreIncrement;
						scoreIncrement++;
					}
				}

				for(auto searchChar = lowerCaseSearch.rbegin(); searchChar != lowerCaseSearch.rend(); searchChar++)
				{
					uint32_t adjacencyScore = 1;

					for(auto resultChar = file.rbegin(); resultChar != file.rend(); resultChar++)
					{
						if(*resultChar == *searchChar)
						{
							it->myScore += adjacencyScore;
							adjacencyScore += 2;
						}
						else
						{
							adjacencyScore = 1;
						}
					}
				}
			}
		}

		std::sort(results.begin(), results.end());

		constexpr uint32_t maxDirectories = 20;
		for(auto it = results.begin(); it != results.end(); it++)
		{
			ImGui::PushID(static_cast<int>(it - results.begin()));
			if(ImGui::Selectable(it->myPath.string().c_str(), (it - results.begin()) == myFocusedResult))
			{
				EnterPath(myCurrentPath / it->myPath);
				ImGui::PopID();
				ImGui::CloseCurrentPopup();
				break;
			}

			ImGui::PopID();

			if(it - results.begin() >= maxDirectories)
				break;
		}

		if(Input::IsKeyDown(KeyCode::Enter))
		{
			EnterPath(myCurrentPath / results[myFocusedResult].myPath);
			ImGui::CloseCurrentPopup();
		}

		if(Input::IsKeyDown(KeyCode::Down))
		{
			myFocusedResult++;
			if(myFocusedResult > maxDirectories)
			{
				myFocusedResult = 0;
			}
		}

		if(Input::IsKeyDown(KeyCode::Up))
		{
			myFocusedResult--;
			if(myFocusedResult >= maxDirectories)
			{
				myFocusedResult = maxDirectories;
			}
		}

		if(Input::IsKeyDown(KeyCode::Escape))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	mySearchBarOpenedThisFrame = false;
}

void ContentBrowserWindow::UpdateRightClickMenu()
{
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("ContentBrowserPopup");
	}

	bool pressedDelete = false;

	if (ImGui::BeginPopup("ContentBrowserPopup"))
	{
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::MenuItem("Folder"))
			{
				std::filesystem::path dirPath = myCurrentPath;
				dirPath.append("Folder");
				std::filesystem::create_directory(dirPath);
				LoadDirectory(myCurrentPath);

				for (int i = 0; i < myItems.size(); ++i)
				{
					if (myItems[i].myPath == dirPath)
					{
						mySelectedItem = i;
						BeginRenaming();
						break;
					}
				}
			}

			ImGui::Separator();
			/*
			if (ImGui::MenuItem("Material"))
			{
				std::filesystem::path matPath = myCurrentPath;
				matPath.append("new material.dynmaterial");
				matPath = Dyn::FileUtils::GetFreePath(matPath);

				Dyn::MaterialFactory::CreateMaterial(matPath);
				LoadDirectory(myCurrentPath);
				for (int i = 0; i < myItems.sizeI(); ++i)
				{
					if (myItems[i].myPath == matPath)
					{
						mySelectedItem = i;
						BeginRenaming();
						break;
					}
				}
			}*/

			ImGui::EndMenu();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Open In File Explorer"))
		{
			ShellExecuteA(NULL, "open", std::filesystem::absolute(myCurrentPath).string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
		}

		if (ImGui::MenuItem("Refresh", "F5"))
		{
			ReloadDirectory();
		}

		ImGui::Separator();


		if (ImGui::MenuItem("Rename", "F2", false, mySelectedItem != -1))
		{
			BeginRenaming();
		}

		if (ImGui::MenuItem("Delete", "Delete", false, mySelectedItem != -1))
		{
			pressedDelete = true;
		}

		ImGui::EndPopup();
	}

	if(pressedDelete)
	{
		DeleteItem();
	}
}

void ContentBrowserWindow::BeginRenaming()
{
	if (mySelectedItem != -1)
	{
		ImGui::NotifyWarning("Renaming", "We should not rename files that have editors open. This should be implemented.");
		
		myRenamingItem = mySelectedItem;
		myNewFileName = myItems[mySelectedItem].myPath.stem().string();
		myRenamingInProgress = true;
	}
}

void ContentBrowserWindow::UpdateRenaming()
{
	if (myRenamingInProgress)
	{
		if (Input::IsKeyDown(KeyCode::Enter))
		{
			std::filesystem::path newPath = myItems[myRenamingItem].myPath;
		
			if (myNewFileName == "")
				myNewFileName = "Unnamed";

			for (auto& path : std::filesystem::recursive_directory_iterator(myRoot))
			{
				if (path == myItems[mySelectedItem].myPath)
					continue;

				if (path.path().stem() == myNewFileName)
				{
					ImGui::NotifyError("Rename failed", "Another file with that name already exists.");
					return;
				}
			}

			std::string extension = "";
			if (newPath.has_extension())
			{
				extension = newPath.extension().string();
			}
			newPath.remove_filename();
			myNewFileName.append(extension);
			newPath.append(myNewFileName);

			std::filesystem::rename(myItems[myRenamingItem].myPath, newPath);

			myItems[myRenamingItem].myPath = newPath;
			myRenamingItem = -1;
			myRenamingInProgress = false;
			LoadDirectory(myCurrentPath);
		}
		else if(Input::IsKeyDown(KeyCode::Escape))
		{
			myRenamingInProgress = false;
			myRenamingItem = -1;
			LoadDirectory(myCurrentPath);
		}
	}
}

void ContentBrowserWindow::DeleteItem()
{
	myConfirmPrompt = EditorConfirmPrompt("Delete file", "Are you sure you want to delete this file?", Delegate<void()>([this]()
	{
		std::filesystem::remove(myItems[mySelectedItem].myPath);
		LoadDirectory(myCurrentPath);
	}));

	myConfirmPrompt.Show();
}

void ContentBrowserWindow::HandleDoubleClick(const ContentBrowserItem& inClickedItem)
{
	check(false && "Not implemented yet.");
}

std::string ContentBrowserWindow::GetPayloadType(const std::filesystem::path& inPath)
{
	return String::ToLowerCopy(inPath.extension().string());
}

void ContentBrowserWindow::LoadDirectory(const std::filesystem::path& inPath)
{
	mySelectedItem = -1;
	myCurrentPath = inPath;

	myItems.Clear();

	for (auto entry : std::filesystem::directory_iterator(inPath))
	{
		if (entry.is_directory())
		{
			myItems.Add({ entry.path(), myContentBrowserTextureDescriptors[FILETYPE_FOLDER] });
			myItems.Last().myIsDirectory = true;
		}
	}

	for (auto entry : std::filesystem::directory_iterator(inPath))
	{
		if (entry.is_directory())
			continue;

		if (IsExcludedPath(entry.path()))
			continue;

		myItems.Add({ entry.path(), GetIconFromPath(entry.path()) });
	}
}

void ContentBrowserWindow::LoadDirectories(const List<std::filesystem::path>& inPaths)
{
	mySelectedItem = -1;
	myItems.Clear();

	for (auto& path : inPaths)
	{
		if (path.has_extension())
		{
			myItems.Add({ path, GetIconFromPath(path) });
			myItems.Last().myIsDirectory = false;
		}
		else
		{
			myItems.Add({ path, myContentBrowserTextureDescriptors[FILETYPE_FOLDER] });
			myItems.Last().myIsDirectory = true;
		}
	}
}

void ContentBrowserWindow::LoadAllDirectories(const std::filesystem::path& inPath)
{
	for (auto entry : std::filesystem::recursive_directory_iterator(inPath))
	{
		if(entry.is_directory())
			myDirectories[entry.path().parent_path()].Add(entry.path());
	}
}

void ContentBrowserWindow::EnterPath(const std::filesystem::path& inPath)
{
	if(std::filesystem::is_directory(inPath))
	{
		if(inPath != myCurrentPath)
		{
			myHistory.push_back(myCurrentPath);
			myForwardHistory.clear();
		}

		LoadDirectory(inPath);
		return;
	}

	const std::filesystem::path& parentPath = inPath.parent_path();
	if(parentPath != myCurrentPath)
	{
		myHistory.push_back(myCurrentPath);
		myForwardHistory.clear();
	}

	LoadDirectory(parentPath);

	uint32_t index = 0;
	for(const ContentBrowserItem& item : myItems)
	{
		if(inPath == item.myPath)
		{
			mySelectedItem = index;
			break;
		}

		index++;
	}
}

bool ContentBrowserWindow::IsExcludedPath(const std::filesystem::path& inPath)
{
	std::filesystem::path extension = inPath.extension();
	std::string lowExtension = String::ToLowerCopy(extension.string());
	if (myExcludedExtensions.Contains(lowExtension))
		return true;
	return false;
}

vk::DescriptorSet ContentBrowserWindow::GetIconFromPath(const std::filesystem::path& inPath) const
{
	if (std::filesystem::is_directory(inPath))
		return myContentBrowserTextureDescriptors[FILETYPE_FOLDER];

	return myContentBrowserTextureDescriptors[FILETYPE_FILE];
}
