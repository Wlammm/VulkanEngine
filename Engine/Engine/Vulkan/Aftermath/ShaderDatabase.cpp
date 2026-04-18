#include "EnginePch.h"
#include "ShaderDatabase.h"

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

#include <fstream>
#include <iomanip>


//*********************************************************
// ShaderDatabase implementation
//*********************************************************

ShaderDatabase::ShaderDatabase()
    : myShaderBinaries()
{
    myInstance = this;
}

ShaderDatabase::~ShaderDatabase()
{
    myInstance = nullptr;
}

bool ShaderDatabase::FindShaderBinary(const GFSDK_Aftermath_ShaderBinaryHash& inShaderHash, List<uint8_t>& outShader) const
{
    std::lock_guard<std::mutex> lock(myMutex);

    auto it = myShaderBinaries.find(inShaderHash);
    if (it == myShaderBinaries.end())
        return false;

    outShader = it->second;
    return true;
}

bool ShaderDatabase::FindShaderBinaryWithDebugData(const GFSDK_Aftermath_ShaderDebugName& inShaderDebugName, List<uint8_t>& outShader) const
{
    std::lock_guard<std::mutex> lock(myMutex);

    auto it = myShaderBinariesWithDebugInfo.find(inShaderDebugName);
    if (it == myShaderBinariesWithDebugInfo.end())
        return false;

    outShader = it->second;
    return true;
}

void ShaderDatabase::AddShaderWithDebugInfo(const void* inSpirvData, size_t inSpirvSize, const char* inDebugName)
{
    std::lock_guard<std::mutex> lock(myMutex);

    List<uint8_t> binary;
    binary.Resize(static_cast<int>(inSpirvSize));
    memcpy(binary.data(), inSpirvData, inSpirvSize);

    GFSDK_Aftermath_SpirvCode spirvCode{};
    spirvCode.pData = inSpirvData;
    spirvCode.size  = static_cast<uint32_t>(inSpirvSize);

    // Key by the debug name Aftermath derives from the SPIR-V itself, so it
    // matches what Aftermath will pass to OnShaderSourceDebugInfoLookup.
    GFSDK_Aftermath_ShaderDebugName shaderDebugName{};
    if (GFSDK_Aftermath_SUCCEED(GFSDK_Aftermath_GetShaderDebugNameSpirv(
        GFSDK_Aftermath_Version_API, &spirvCode, nullptr, &shaderDebugName)))
    {
        myShaderBinariesWithDebugInfo[shaderDebugName] = binary;
    }

    GFSDK_Aftermath_ShaderBinaryHash shaderHash{};
    if (GFSDK_Aftermath_SUCCEED(GFSDK_Aftermath_GetShaderHashSpirv(GFSDK_Aftermath_Version_API, &spirvCode, &shaderHash)))
        myShaderBinaries[shaderHash] = std::move(binary);
}
