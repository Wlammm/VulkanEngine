#include "EnginePch.h"
#include "NvidiaAftermathTracker.h"
#include <fstream>
#include <iomanip>
#include <string>
#include <array>

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
    // dump description data.Only the crash dump callback is mandatory. The other two
    // callbacks are optional and can be omitted, by passing nullptr, if the corresponding
    // functionality is not used.
    // The DeferDebugInfoCallbacks flag enables caching of shader debug information data
    // in memory. If the flag is set, ShaderDebugInfoCallback will be called only
    // in the event of a crash, right before GpuCrashDumpCallback. If the flag is not set,
    // ShaderDebugInfoCallback will be called for every shader that is compiled..
    
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_EnableGpuCrashDumps(
        GFSDK_Aftermath_Version_API,
        GFSDK_Aftermath_GpuCrashDumpWatchedApiFlags_Vulkan,
        GFSDK_Aftermath_GpuCrashDumpFeatureFlags_DeferDebugInfoCallbacks,           // Let the Nsight Aftermath library cache shader debug information.
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
    const std::string crashDumpFileName = baseFileName + ".nv-gpudmp";
    std::ofstream dumpFile(crashDumpFileName, std::ios::out | std::ios::binary);
    if (dumpFile)
    {
        dumpFile.write((const char*)pGpuCrashDump, gpuCrashDumpSize);
        dumpFile.close();
    }
    LOG_ERROR("[Aftermath] GPU crash dump written to: %s", crashDumpFileName.c_str());

    // Decode the crash dump to a JSON string.
    // Step 1: Generate the JSON and get the size.
    uint32_t jsonSize = 0;
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GenerateJSON(
        decoder,
        GFSDK_Aftermath_GpuCrashDumpDecoderFlags_ALL_INFO,
        GFSDK_Aftermath_GpuCrashDumpFormatterFlags_NONE,
        ShaderDebugInfoLookupCallback,
        ShaderLookupCallback,
        ShaderSourceDebugInfoLookupCallback,
        this,
        &jsonSize));
    // Step 2: Allocate a buffer and fetch the generated JSON.
    std::vector<char> json(jsonSize);
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_GetJSON(
        decoder,
        uint32_t(json.size()),
        json.data()));

    // Write the crash dump data as JSON to a file.
    const std::string jsonFileName = crashDumpFileName + ".json";
    std::ofstream jsonFile(jsonFileName, std::ios::out | std::ios::binary);
    if (jsonFile)
    {
       // Write the JSON to the file (excluding string termination)
       jsonFile.write(json.data(), json.size() - 1);
       jsonFile.close();
    }

    // Destroy the GPU crash dump decoder object.
    AFTERMATH_CHECK_ERROR(GFSDK_Aftermath_GpuCrashDump_DestroyDecoder(decoder));
}

// Helper for writing shader debug information to a file
void NvidiaAftermathTracker::WriteShaderDebugInformationToFile(
    GFSDK_Aftermath_ShaderDebugInfoIdentifier identifier,
    const void* pShaderDebugInfo,
    const uint32_t shaderDebugInfoSize)
{
    // Create a unique file name.
    const std::string filePath = "shader-" + std::to_string(identifier) + ".nvdbg";

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
        // Early exit, nothing found. No need to call setShaderDebugInfo.
        return;
    }

    // Let the GPU crash dump decoder know about the shader debug information
    // that was found.
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
        return;
    }

    // Let the GPU crash dump decoder know about the shader data
    // that was found.
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
        // Early exit, nothing found. No need to call setShaderBinary.
        return;
    }

    // Let the GPU crash dump decoder know about the shader debug data that was
    // found.
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
