#pragma once

#include <shaderc/shaderc.hpp>

// Interface for shaderc compisler to use to find include files in shaders.
class VulkanShaderIncluder : public shaderc::CompileOptions::IncluderInterface
{
public:
	VulkanShaderIncluder();
	~VulkanShaderIncluder();

	// Inherited via IncluderInterface
	virtual shaderc_include_result* GetInclude(const char* inRequestedSource, shaderc_include_type inType, const char* inRequrestingSource, size_t inIncludeDepth) override final;
	virtual void ReleaseInclude(shaderc_include_result* inData) override final;

	List<std::filesystem::path> GetIncludedFiles() const;
	
	std::filesystem::path ResolvePath(const char* inRequestedSource);
private:
	struct IncludeData
	{
		std::string sourceName;
		std::string content;
		shaderc_include_result* ptr;

		IncludeData() = default;
		IncludeData(shaderc_include_result* inPtr) : ptr{ inPtr } {};
		
		bool operator==(const IncludeData& inOther) const
		{
			return ptr == inOther.ptr;
		}
	};

	List<IncludeData> myActiveIncludes{};
	
	const List<std::filesystem::path> myExternalIncludePaths 
	{
		"./Shaders/",
		"../Engine/Engine/Rendering/SharedWithShaders/"
	};
};