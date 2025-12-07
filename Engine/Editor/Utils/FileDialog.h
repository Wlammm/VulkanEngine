#pragma once

class FileDialog
{
public:
    static std::string OpenFileDialog(const LPCSTR& aFilter);

    static std::string SaveFileDialog(const LPCSTR& aFilter);
};