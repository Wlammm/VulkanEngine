#pragma once


class DXCompiler
{
public:
    inline static ComPtr<IDxcUtils> gDxcUtils;
    inline static ComPtr<IDxcCompiler3> gDxcCompiler;
    
    static void InitDXC()
    {
        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&gDxcUtils));
        DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&gDxcCompiler));
    }
};
