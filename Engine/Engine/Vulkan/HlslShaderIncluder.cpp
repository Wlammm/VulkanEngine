#include "EnginePch.h"
#include "HlslShaderIncluder.h"


HlslShaderIncluder::HlslShaderIncluder(IDxcUtils* utils, const std::filesystem::path& baseDir)
    : myUtils(utils), myBaseDir(baseDir)
{
    myUtils->AddRef();
}

HlslShaderIncluder::~HlslShaderIncluder()
{
    myUtils->Release();
}

HRESULT HlslShaderIncluder::QueryInterface(const IID& riid, void** ppvObject)
{
    if (riid == __uuidof(IDxcIncludeHandler) || riid == __uuidof(IUnknown))
    {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

ULONG HlslShaderIncluder::AddRef()
{
    return ++myRefCount;
}

ULONG HlslShaderIncluder::Release()
{
    ULONG count = --myRefCount;
    if (count == 0)
        delete this;
    return count;
}

HRESULT HlslShaderIncluder::LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource)
{
    std::filesystem::path includePath = myBaseDir / pFilename;

    if (!std::filesystem::exists(includePath))
        return E_FAIL;

    myIncludedFiles.Add(includePath);
    ComPtr<IDxcBlobEncoding> blob;
    HRESULT hr = myUtils->LoadFile(
        includePath.c_str(),
        nullptr,
        &blob
    );

    if (FAILED(hr))
        return hr;

    *ppIncludeSource = blob.Detach();
    return S_OK;
}

const List<std::filesystem::path>& HlslShaderIncluder::GetIncludedFiles() const
{
    return myIncludedFiles;
}
