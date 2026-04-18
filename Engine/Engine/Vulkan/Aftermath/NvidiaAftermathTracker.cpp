#include "EnginePch.h"
#include "NvidiaAftermathTracker.h"
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <filesystem>

//*********************************************************
// GpuCrashTracker implementation
// MODIFIED SOURCE FROM NVIDIA'S EXAMPLE CODE.
//*********************************************************

NvidiaAftermathTracker::NvidiaAftermathTracker(const AfterMathMarkerMap& markerMap)
    : m_initialized(false)
    , m_mutex()
    , m_shaderDebugInfo()
    , m_shaderDatabase()
    , m_markerMap(markerMap)
{
}

NvidiaAftermathTracker::~NvidiaAftermathTracker()
{
    // If initialized, disable GPU crash dumps
    if (m_initialized)
    {
        GFSDK_Aftermath_DisableGpuCrashDumps();
    }
}

// Initialize the GPU Crash Dump Tracker
void NvidiaAftermathTracker::Initialize()
{
    // Enable GPU crash dumps and set up the callbacks for crash dump notifications,
    // shader debug information notifications, and providing additional crash
    // dump description data. Only the crash dump callback is mandatory. The other two
    // callbacks are optional and can be omitted, by passing nullptr, if the corresponding
    // functionality is not used.
    // We use NONE (not DeferDebugInfoCallbacks) so that ShaderDebugInfoCallback fires
    // eagerly for every shader as it's compiled. This ensures debug info is always
    // available at crash time — deferred delivery can miss shaders for some crash types.
    
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_EnableGpuCrashDumps(
        GFSDK_Aftermath_Version_API,
        GFSDK_Aftermath_GpuCrashDumpWatchedApiFlags_Vulkan,
        GFSDK_Aftermath_GpuCrashDumpFeatureFlags_Default,                           // Collect shader debug info eagerly for every shader, not deferred to crash time.
        GpuCrashDumpCallback,                                                       // Register callback for GPU crash dumps.
        ShaderDebugInfoCallback,                                                    // Register callback for shader debug information.
        CrashDumpDescriptionCallback,                                               // Register callback for GPU crash dump description.
        ResolveMarkerCallback,                                                      // Register callback for resolving application-managed markers.
        this));                                                           // Set the GpuCrashTracker object as user data for the above callbacks.

    m_initialized = true;
}

uint32_t NvidiaAftermathTracker::GetAftermathFlags() const
{
    return m_AftermathFlags;
}

// Handler for GPU crash dump callbacks from Nsight Aftermath
void NvidiaAftermathTracker::OnCrashDump(const void* pGpuCrashDump, const uint32_t gpuCrashDumpSize)
{
    // Make sure only one thread at a time...
    std::lock_guard<std::mutex> lock(m_mutex);

    LOG_ERROR("[Aftermath] GPU crash dump received (%u bytes). ShaderDatabase has %zu binaries, %zu debug info entries. Driver debug info blobs: %zu",
        gpuCrashDumpSize, m_shaderDatabase.GetShaderBinaryCount(), m_shaderDatabase.GetShaderDebugInfoCount(), m_shaderDebugInfo.size());

    // Write to file for later in-depth analysis with Nsight Graphics.
    WriteGpuCrashDumpToFile(pGpuCrashDump, gpuCrashDumpSize);
}

// Handler for shader debug information callbacks
void NvidiaAftermathTracker::OnShaderDebugInfo(const void* pShaderDebugInfo, const uint32_t shaderDebugInfoSize)
{
    // Make sure only one thread at a time...
    std::lock_guard<std::mutex> lock(m_mutex);

    // Get shader debug information identifier
    GFSDK_Aftermath_ShaderDebugInfoIdentifier identifier = {};
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GetShaderDebugInfoIdentifier(
        GFSDK_Aftermath_Version_API,
        pShaderDebugInfo,
        shaderDebugInfoSize,
        &identifier));

    // Store information for decoding of GPU crash dumps with shader address mapping
    // from within the application.
    std::vector<uint8_t> data((uint8_t*)pShaderDebugInfo, (uint8_t*)pShaderDebugInfo + shaderDebugInfoSize);
    m_shaderDebugInfo[identifier].swap(data);

    LOG("[Aftermath] Received shader debug info: %s (%u bytes)", std::to_string(identifier).c_str(), shaderDebugInfoSize);

    // Write to file for later in-depth analysis of crash dumps with Nsight Graphics
    WriteShaderDebugInformationToFile(identifier, pShaderDebugInfo, shaderDebugInfoSize);
}

// Handler for GPU crash dump description callbacks
void NvidiaAftermathTracker::OnDescription(PFN_GFSDK_Aftermath_AddGpuCrashDumpDescription addDescription)
{
    // Add some basic description about the crash. This is called after the GPU crash happens, but before
    // the actual GPU crash dump callback. The provided data is included in the crash dump and can be
    // retrieved using GFSDK_Aftermath_GpuCrashDump_GetDescription().
    addDescription(GFSDK_Aftermath_GpuCrashDumpDescriptionKey_ApplicationName, "VulkanEngine");
    addDescription(GFSDK_Aftermath_GpuCrashDumpDescriptionKey_ApplicationVersion, "1.0");

#if !SHIPPING
    // Helpful toggles visible directly in the dump.
    addDescription(GFSDK_Aftermath_GpuCrashDumpDescriptionKey_UserDefined, "StartupArg:-aftermath=1");
#endif
}

// Handler for app-managed marker resolve callback
void NvidiaAftermathTracker::OnResolveMarker(const void* pMarkerData, const uint32_t markerDataSize, void** ppResolvedMarkerData, uint32_t* pResolvedMarkerDataSize)
{
    // Important: the pointer passed back via ppResolvedMarkerData must remain valid after this function returns
    // using references for all of the m_markerMap accesses ensures that the pointers refer to the persistent data
    for (auto& map : m_markerMap)
    {
        const auto& foundMarker = map.find((uint64_t)pMarkerData);
        if (foundMarker != map.end())
        {
            const std::string& foundMarkerData = foundMarker->second;
            *ppResolvedMarkerData = (void*)foundMarkerData.data();
            *pResolvedMarkerDataSize = (uint32_t)foundMarkerData.length();
            return;
        }
    }

    // Fall back: treat as a null-terminated string (used for static pass name pointers
    // set via vkCmdSetCheckpointNV directly with a const char*).
    if (pMarkerData)
    {
        *ppResolvedMarkerData = const_cast<void*>(pMarkerData);
        *pResolvedMarkerDataSize = (uint32_t)strlen(static_cast<const char*>(pMarkerData));
    }
}

// Helper for writing a GPU crash dump to a file
void NvidiaAftermathTracker::WriteGpuCrashDumpToFile(const void* pGpuCrashDump, const uint32_t gpuCrashDumpSize)
{
    // Prefer writing to a dedicated folder to keep working directory clean.
    const std::filesystem::path outDir = std::filesystem::path("Bin") / "Aftermath";
    std::error_code ec;
    std::filesystem::create_directories(outDir, ec);

    // Create a GPU crash dump decoder object for the GPU crash dump.
    GFSDK_Aftermath_GpuCrashDump_Decoder decoder = {};
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_CreateDecoder(
        GFSDK_Aftermath_Version_API,
        pGpuCrashDump,
        gpuCrashDumpSize,
        &decoder));

    // Use the decoder object to read basic information, like application
    // name, PID, etc. from the GPU crash dump.
    GFSDK_Aftermath_GpuCrashDump_BaseInfo baseInfo = {};
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GetBaseInfo(decoder, &baseInfo));

    // Use the decoder object to query the application name that was set
    // in the GPU crash dump description.
    uint32_t applicationNameLength = 0;
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GetDescriptionSize(
        decoder,
        GFSDK_Aftermath_GpuCrashDumpDescriptionKey_ApplicationName,
        &applicationNameLength));

    std::vector<char> applicationName(applicationNameLength, '\0');

    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GetDescription(
        decoder,
        GFSDK_Aftermath_GpuCrashDumpDescriptionKey_ApplicationName,
        uint32_t(applicationName.size()),
        applicationName.data()));

    // Create a unique file name for writing the crash dump data to a file.
    // Note: due to an Nsight Aftermath bug (will be fixed in an upcoming
    // driver release) we may see redundant crash dumps. As a workaround,
    // attach a unique count to each generated file name.
    static int count = 0;
    const std::string baseFileName =
        std::string(applicationName.data())
        + "-"
        + std::to_string(baseInfo.pid)
        + "-"
        + std::to_string(++count);

    // Write the crash dump data to a file using the .nv-gpudmp extension
    // registered with Nsight Graphics.
    const std::filesystem::path crashDumpFilePath = outDir / (baseFileName + ".nv-gpudmp");
    std::ofstream dumpFile(crashDumpFilePath, std::ios::out | std::ios::binary);
    if (dumpFile)
    {
        dumpFile.write((const char*)pGpuCrashDump, gpuCrashDumpSize);
        dumpFile.close();
    }
    LOG_ERROR("[Aftermath] GPU crash dump written to: %s", crashDumpFilePath.string().c_str());

    const std::string augmentationPath = crashDumpFilePath.string() + ".shader-resolution.txt";
    WriteCrashDumpAugmentationReport(decoder, augmentationPath);
    m_shaderDatabase.WriteRegisteredSnapshot((outDir / (baseFileName + ".registered-shaders.txt")).string());
    LOG_ERROR("[Aftermath] Shader resolution report: %s", augmentationPath.c_str());

    // Decode the crash dump to a JSON string.
    // Step 1: Generate the JSON and get the size.
    uint32_t jsonSize = 0;
    GFSDK_Aftermath_Result jsonResult = GFSDK_Aftermath_GpuCrashDump_GenerateJSON(
        decoder,
        GFSDK_Aftermath_GpuCrashDumpDecoderFlags_ALL_INFO,
        GFSDK_Aftermath_GpuCrashDumpFormatterFlags_UTF8_OUTPUT,
        ShaderDebugInfoLookupCallback,
        ShaderLookupCallback,
        ShaderSourceDebugInfoLookupCallback,
        this,
        &jsonSize);

    if (GFSDK_Aftermath_SUCCEED(jsonResult))
    {
        // Step 2: Allocate a buffer and fetch the generated JSON.
        std::vector<char> json(jsonSize);
        AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GetJSON(
            decoder,
            uint32_t(json.size()),
            json.data()));

        // Write the crash dump data as JSON to a file.
        const std::filesystem::path jsonFilePath = crashDumpFilePath.string() + ".json";
        std::ofstream jsonFile(jsonFilePath, std::ios::out | std::ios::binary);
        if (jsonFile)
        {
           jsonFile.write(json.data(), json.size() - 1);
           jsonFile.close();
        }
        LOG_ERROR("[Aftermath] JSON crash dump written to: %s (%u bytes)", jsonFilePath.string().c_str(), jsonSize);
    }
    else
    {
        LOG_ERROR("[Aftermath] Failed to generate JSON from crash dump (result: %s)", std::to_string(jsonResult).c_str());
    }

    // Destroy the GPU crash dump decoder object.
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_DestroyDecoder(decoder));
}

void NvidiaAftermathTracker::WriteCrashDumpAugmentationReport(GFSDK_Aftermath_GpuCrashDump_Decoder decoder, const std::string& reportPathUtf8) const
{
    std::ofstream f(reportPathUtf8, std::ios::out);
    if (!f)
        return;

    f << "Aftermath: shader resolution vs ShaderDatabase\n";
    f << "If JSON shows only a name like compute_01 @ 0x..., PC -> line mapping still needs\n";
    f << "matching SPIR-V (shaderLookupCb) + driver shader debug blobs (ShaderDebugInfoLookup),\n";
    f << "and DXC debug mode that emits line tables Aftermath understands (see Shader.cpp / -aftermath).\n\n";

    f << "--- Page fault / candidate resources (often pinpoints bad buffer/image) ---\n";
    GFSDK_Aftermath_GpuCrashDump_PageFaultInfo pageFault{};
    if (GFSDK_Aftermath_SUCCEED(GFSDK_Aftermath_GpuCrashDump_GetPageFaultInfo(decoder, &pageFault)))
    {
        f << "faultingGpuVA=0x" << std::hex << pageFault.faultingGpuVA << std::dec
          << " faultType=" << static_cast<int>(pageFault.faultType)
          << " accessType=" << static_cast<int>(pageFault.accessType)
          << " engine=" << static_cast<int>(pageFault.engine)
          << " client=" << static_cast<int>(pageFault.client)
          << " resourceInfoCount=" << pageFault.resourceInfoCount << "\n";

        if (pageFault.resourceInfoCount > 0u)
        {
            std::vector<GFSDK_Aftermath_GpuCrashDump_ResourceInfo> resources(pageFault.resourceInfoCount);
            if (GFSDK_Aftermath_SUCCEED(GFSDK_Aftermath_GpuCrashDump_GetPageFaultResourceInfo(
                    decoder, pageFault.resourceInfoCount, resources.data())))
            {
                for (uint32_t ri = 0; ri < pageFault.resourceInfoCount; ++ri)
                {
                    const GFSDK_Aftermath_GpuCrashDump_ResourceInfo& r = resources[ri];
                    f << "  [" << ri << "] gpuVa=0x" << std::hex << r.gpuVa << std::dec
                      << " size=" << r.size
                      << " WxHxD=" << r.width << "x" << r.height << "x" << r.depth
                      << " mips=" << r.mipLevels
                      << " VkFormat=" << r.format
                      << " apiResource=0x" << std::hex << r.apiResource << std::dec
                      << " debugName=\"" << r.debugName << "\""
                      << " bufferHeap=" << (r.bIsBufferHeap ? 1 : 0)
                      << " destroyed=" << (r.bWasDestroyed ? 1 : 0)
                      << "\n";
                }
            }
        }
    }
    else
        f << "(no page fault block in dump or query failed — not all faults include this)\n";

    f << "\n";

    uint32_t shaderCount = 0;
    if (GFSDK_Aftermath_SUCCEED(GFSDK_Aftermath_GpuCrashDump_GetActiveShadersInfoCount(decoder, &shaderCount)) && shaderCount > 0)
    {
        std::vector<GFSDK_Aftermath_GpuCrashDump_ShaderInfo> infos(shaderCount);
        if (GFSDK_Aftermath_SUCCEED(GFSDK_Aftermath_GpuCrashDump_GetActiveShadersInfo(decoder, shaderCount, infos.data())))
        {
            f << "Active shaders in dump: " << shaderCount << "\n\n";
            for (uint32_t i = 0; i < shaderCount; ++i)
            {
                GFSDK_Aftermath_ShaderBinaryHash binaryHash{};
                const GFSDK_Aftermath_Result hashRes = GFSDK_Aftermath_GetShaderHashForShaderInfo(decoder, &infos[i], &binaryHash);
                const std::string path = GFSDK_Aftermath_SUCCEED(hashRes) ? m_shaderDatabase.FindShaderSourcePath(binaryHash) : std::string{};
                f << "[" << i << "] shaderType=" << static_cast<int>(infos[i].shaderType)
                  << " shaderHashField=0x" << std::hex << infos[i].shaderHash << std::dec
                  << " instance=0x" << std::hex << infos[i].shaderInstance << std::dec;
                if (GFSDK_Aftermath_SUCCEED(hashRes))
                    f << " GetShaderHashForShaderInfo=" << std::to_string(binaryHash)
                      << " database=" << (path.empty() ? "MISSING" : "OK")
                      << " source=" << (path.empty() ? "(unknown)" : path);
                else
                    f << " GetShaderHashForShaderInfo=FAILED(" << std::to_string(hashRes) << ")";
                f << "\n";
            }
        }
        else
            f << "GetActiveShadersInfo failed.\n";
    }
    else
        f << "No active shader info (or GetActiveShadersInfoCount failed).\n";

    f << "\n--- Checkpoints / markers ---\n";
    uint32_t markerCount = 0;
    if (GFSDK_Aftermath_SUCCEED(GFSDK_Aftermath_GpuCrashDump_GetEventMarkersInfoCount(decoder, &markerCount)) && markerCount > 0)
    {
        std::vector<GFSDK_Aftermath_GpuCrashDump_EventMarkerInfo> markers(markerCount);
        if (GFSDK_Aftermath_SUCCEED(GFSDK_Aftermath_GpuCrashDump_GetEventMarkersInfo(decoder, markerCount, markers.data())))
        {
            for (uint32_t i = 0; i < markerCount; ++i)
            {
                f << "[" << i << "] contextStatus=" << static_cast<int>(markers[i].contextStatus)
                  << " markerSize=" << markers[i].markerDataSize;
                if (markers[i].markerData && markers[i].markerDataSize > 0)
                {
                    f << " ";
                    const auto* p = static_cast<const unsigned char*>(markers[i].markerData);
                    const uint32_t n = markers[i].markerDataSize < 256u ? markers[i].markerDataSize : 256u;
                    for (uint32_t j = 0; j < n; ++j)
                    {
                        const unsigned char c = p[j];
                        f << (c >= 32u && c < 127u ? static_cast<char>(c) : '.');
                    }
                }
                f << "\n";
            }
        }
    }
    else
        f << "(none or query failed)\n";
}

// Helper for writing shader debug information to a file
void NvidiaAftermathTracker::WriteShaderDebugInformationToFile(
    GFSDK_Aftermath_ShaderDebugInfoIdentifier identifier,
    const void* pShaderDebugInfo,
    const uint32_t shaderDebugInfoSize)
{
    const std::filesystem::path outDir = std::filesystem::path("Bin") / "Aftermath" / "Shaders";
    std::error_code ec;
    std::filesystem::create_directories(outDir, ec);

    // Create a unique file name.
    const std::filesystem::path filePath = outDir / ("shader-" + std::to_string(identifier) + ".nvdbg");

    std::ofstream f(filePath, std::ios::out | std::ios::binary);
    if (f)
    {
        f.write((const char*)pShaderDebugInfo, shaderDebugInfoSize);
    }
}

// Handler for shader debug information lookup callbacks.
// This is used by the JSON decoder for mapping shader instruction
// addresses to SPIR-V IL lines or GLSL source lines.
void NvidiaAftermathTracker::OnShaderDebugInfoLookup(
    const GFSDK_Aftermath_ShaderDebugInfoIdentifier& identifier,
    PFN_GFSDK_Aftermath_SetData setShaderDebugInfo) const
{
    // Search the list of shader debug information blobs received earlier.
    auto i_debugInfo = m_shaderDebugInfo.find(identifier);
    if (i_debugInfo == m_shaderDebugInfo.end())
    {
        LOG_ERROR("[Aftermath] Shader debug info lookup FAILED for identifier %s (have %zu entries)",
            std::to_string(identifier).c_str(), m_shaderDebugInfo.size());
        return;
    }

    LOG("[Aftermath] Shader debug info lookup succeeded for identifier %s (%u bytes)",
        std::to_string(identifier).c_str(), uint32_t(i_debugInfo->second.size()));
    setShaderDebugInfo(i_debugInfo->second.data(), uint32_t(i_debugInfo->second.size()));
}

// Handler for shader lookup callbacks.
// This is used by the JSON decoder for mapping shader instruction
// addresses to SPIR-V IL lines or GLSL source lines.
// NOTE: If the application loads stripped shader binaries (ie; --strip-all in spirv-remap),
// Aftermath will require access to both the stripped and the not stripped
// shader binaries.
void NvidiaAftermathTracker::OnShaderLookup(
    const GFSDK_Aftermath_ShaderBinaryHash& shaderHash,
    PFN_GFSDK_Aftermath_SetData setShaderBinary) const
{
    List<uint8_t> shaderBinary;
    if (!m_shaderDatabase.FindShaderBinary(shaderHash, shaderBinary))
    {
        LOG_ERROR("[Aftermath] Shader binary lookup FAILED for hash %s (have %zu registered shaders)",
            std::to_string(shaderHash).c_str(), m_shaderDatabase.GetShaderBinaryCount());
        return;
    }

    LOG("[Aftermath] Shader binary lookup succeeded for hash %s (%u bytes)",
        std::to_string(shaderHash).c_str(), uint32_t(shaderBinary.size()));
    setShaderBinary(shaderBinary.data(), uint32_t(shaderBinary.size()));
}

// Handler for shader source debug info lookup callbacks.
// This is used by the JSON decoder for mapping shader instruction addresses to
// GLSL source lines, if the shaders used by the application were compiled with
// separate debug info data files.
void NvidiaAftermathTracker::OnShaderSourceDebugInfoLookup(
    const GFSDK_Aftermath_ShaderDebugName& shaderDebugName,
    PFN_GFSDK_Aftermath_SetData setShaderBinary) const
{
    // Find source debug info for the shader DebugName in the shader database.
    List<uint8_t> shaderBinary;
    if (!m_shaderDatabase.FindShaderBinaryWithDebugData(shaderDebugName, shaderBinary))
    {
        LOG_ERROR("[Aftermath] Shader source debug info lookup FAILED for debug name '%s' (have %zu entries)",
            shaderDebugName.name, m_shaderDatabase.GetShaderDebugInfoCount());
        return;
    }

    LOG("[Aftermath] Shader source debug info lookup succeeded for debug name '%s' (%u bytes)",
        shaderDebugName.name, uint32_t(shaderBinary.size()));
    setShaderBinary(shaderBinary.data(), uint32_t(shaderBinary.size()));
}

// Static callback wrapper for OnCrashDump
void NvidiaAftermathTracker::GpuCrashDumpCallback(
    const void* pGpuCrashDump,
    const uint32_t gpuCrashDumpSize,
    void* pUserData)
{
    NvidiaAftermathTracker* pGpuCrashTracker = reinterpret_cast<NvidiaAftermathTracker*>(pUserData);
    pGpuCrashTracker->OnCrashDump(pGpuCrashDump, gpuCrashDumpSize);
}

// Static callback wrapper for OnShaderDebugInfo
void NvidiaAftermathTracker::ShaderDebugInfoCallback(
    const void* pShaderDebugInfo,
    const uint32_t shaderDebugInfoSize,
    void* pUserData)
{
    NvidiaAftermathTracker* pGpuCrashTracker = reinterpret_cast<NvidiaAftermathTracker*>(pUserData);
    pGpuCrashTracker->OnShaderDebugInfo(pShaderDebugInfo, shaderDebugInfoSize);
}

// Static callback wrapper for OnDescription
void NvidiaAftermathTracker::CrashDumpDescriptionCallback(
    PFN_GFSDK_Aftermath_AddGpuCrashDumpDescription addDescription,
    void* pUserData)
{
    NvidiaAftermathTracker* pGpuCrashTracker = reinterpret_cast<NvidiaAftermathTracker*>(pUserData);
    pGpuCrashTracker->OnDescription(addDescription);
}

// Static callback wrapper for OnResolveMarker
void NvidiaAftermathTracker::ResolveMarkerCallback(
    const void* pMarkerData,
    const uint32_t markerDataSize,
    void* pUserData,
    void** ppResolvedMarkerData,
    uint32_t* pResolvedMarkerDataSize)
{
    NvidiaAftermathTracker* pGpuCrashTracker = reinterpret_cast<NvidiaAftermathTracker*>(pUserData);
    pGpuCrashTracker->OnResolveMarker(pMarkerData, markerDataSize, ppResolvedMarkerData, pResolvedMarkerDataSize);
}

// Static callback wrapper for OnShaderDebugInfoLookup
void NvidiaAftermathTracker::ShaderDebugInfoLookupCallback(
    const GFSDK_Aftermath_ShaderDebugInfoIdentifier* pIdentifier,
    PFN_GFSDK_Aftermath_SetData setShaderDebugInfo,
    void* pUserData)
{
    NvidiaAftermathTracker* pGpuCrashTracker = reinterpret_cast<NvidiaAftermathTracker*>(pUserData);
    pGpuCrashTracker->OnShaderDebugInfoLookup(*pIdentifier, setShaderDebugInfo);
}

// Static callback wrapper for OnShaderLookup
void NvidiaAftermathTracker::ShaderLookupCallback(
    const GFSDK_Aftermath_ShaderBinaryHash* pShaderHash,
    PFN_GFSDK_Aftermath_SetData setShaderBinary,
    void* pUserData)
{
    NvidiaAftermathTracker* pGpuCrashTracker = reinterpret_cast<NvidiaAftermathTracker*>(pUserData);
    pGpuCrashTracker->OnShaderLookup(*pShaderHash, setShaderBinary);
}

// Static callback wrapper for OnShaderSourceDebugInfoLookup
void NvidiaAftermathTracker::ShaderSourceDebugInfoLookupCallback(
    const GFSDK_Aftermath_ShaderDebugName* pShaderDebugName,
    PFN_GFSDK_Aftermath_SetData setShaderBinary,
    void* pUserData)
{
    NvidiaAftermathTracker* pGpuCrashTracker = reinterpret_cast<NvidiaAftermathTracker*>(pUserData);
    pGpuCrashTracker->OnShaderSourceDebugInfoLookup(*pShaderDebugName, setShaderBinary);
}
