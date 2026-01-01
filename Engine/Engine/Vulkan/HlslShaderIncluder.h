#pragma once

class HlslShaderIncluder final : public IDxcIncludeHandler
{
public:
    HlslShaderIncluder(IDxcUtils* utils, const std::filesystem::path& baseDir);
    ~HlslShaderIncluder();
    
    HRESULT QueryInterface(const IID& riid, void** ppvObject) override;
    ULONG AddRef() override;
    ULONG Release() override;
    HRESULT LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource) override;
    
    const List<std::filesystem::path>& GetIncludedFiles() const;
    
private:
    std::atomic<ULONG> myRefCount{1};
    IDxcUtils* myUtils;
    std::filesystem::path myBaseDir;
    
    List<std::filesystem::path> myIncludedFiles{};
};
