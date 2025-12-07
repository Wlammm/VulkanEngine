#include "EditorPch.h"
#include "FileDialog.h"

#include "Engine/Engine.h"
#include "Engine/Windows/WindowHandler.h"

std::string FileDialog::OpenFileDialog(const LPCSTR& aFilter)
{
    std::string path = std::filesystem::absolute("Assets/").string();
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = Engine::GetWindowHandler().GetHWND();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = aFilter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrInitialDir = LPSTR(path.c_str());
    if (GetOpenFileNameA(&ofn) == TRUE)
    {
        return ofn.lpstrFile;
    }

    return std::string();
}

std::string FileDialog::SaveFileDialog(const LPCSTR& aFilter)
{
    std::string path = std::filesystem::absolute("Assets/").string();
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = Engine::GetWindowHandler().GetHWND();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = aFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = LPSTR(path.c_str());
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetSaveFileNameA(&ofn))
    {
        return ofn.lpstrFile;
    }

    return std::string();
}
