//*********************************************************
//
// Copyright (c) 2019-2022, NVIDIA CORPORATION. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//
//*********************************************************

#pragma once

#include <vector>
#include <map>
#include <mutex>

#include "aftermath/NsightAftermathHelpers.h"

class ShaderDatabase
{
    inline static ShaderDatabase* myInstance = nullptr;

public:
    static ShaderDatabase* Get() { return myInstance; }
    ShaderDatabase();
    ~ShaderDatabase();
    
    bool FindShaderBinary(const GFSDK_Aftermath_ShaderBinaryHash& inShaderHash, List<uint8_t>& outShader) const;
    bool FindShaderBinaryWithDebugData(const GFSDK_Aftermath_ShaderDebugName& inShaderDebugName, List<uint8_t>& outShader) const;
    
    void AddShaderWithDebugInfo(const void* inSpirvData, size_t inSpirvSize, const char* inDebugName);
    
private:
    mutable std::mutex myMutex;
    std::map<GFSDK_Aftermath_ShaderBinaryHash, List<uint8_t>> myShaderBinaries;
    std::map<GFSDK_Aftermath_ShaderDebugName, List<uint8_t>> myShaderBinariesWithDebugInfo;
};
